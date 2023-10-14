/**
 * @file b64.h
 * @brief Base64 encoding/decoding
*/
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "b64.h"

/** Base64 encoding table with 6-bit index and encoded character */
static char encoding_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/'
};

/**
 * @brief Padding
*/
#define PADDING '='

/**
 * @brief Carriage return
*/
#define CHAR_CR '\x0d'
/**
 * @brief Line feed
*/
#define CHAR_LF '\x0a'
/**
 * @brief Null character
*/
#define CHAR_NULL '\0'

/**
 * @brief Get Base64 encoded byte size
 *
 * @param[in] src_size Byte size of input byte array
 * @param[in] use_padding Use padding
 * @param[in] line_length Length to insert linebreak, if 0, no linebreaks
 * @return Byte size of the encoded string, including a NULL character
*/
static size_t get_encoded_byte_size(const size_t src_size, const bool use_padding, const size_t line_length) {
    if (src_size == 0) {
        return 0;
    }

    size_t num_6bit_blocks = (src_size * 8 + 5) / 6;
    size_t encoded_size = (num_6bit_blocks + 3) / 4 * 4;

    // Reduce padding size if not required
    if (!use_padding) {
        size_t num_padding = 4 - (num_6bit_blocks % 4);
        if (num_padding <= 3) {
            encoded_size -= num_padding;
        }
    }

    // Add size of CRLF if required
    if (line_length > 0) {
        int num_lines = encoded_size / line_length;
        if ((encoded_size % line_length) == 0) {
            num_lines--;
        }
        encoded_size += (num_lines * 2);
    }

    // Consider null charecter
    return encoded_size + 1;
}

/**
 * @brief Encode the first 6 bits in the 3 byte block
 *
 * @param[in] byte First input byte
 * @return Base64 encoded character
*/
static inline char encode_to_1st_char(const uint8_t byte) {
    return encoding_table[(byte & 0xfc) >> 2];
}

/**
 * @brief Encode the second 6 bits in the 3 byte block
 *
 * @param[in] byte1 First input byte in the block
 * @param[in] byte2 Second input byte in the block
 * @return Base64 encoded character
*/
static inline char encode_to_2nd_char(const uint8_t byte1, const uint8_t byte2) {
    return encoding_table[((byte1 & 0x03) << 4) | ((byte2 & 0xf0) >> 4)];
}

/**
 * @brief Encode the third 6 bits in the 3 byte block
 *
 * @param[in] byte1 Second input byte in the block
 * @param[in] byte2 Third input byte in the block
 * @return Base64 encoded character
*/
static inline char encode_to_3rd_char(const uint8_t byte1, const uint8_t byte2) {
    return encoding_table[((byte1 & 0x0f) << 2) | ((byte2 & 0xc0) >> 6)];
}

/**
 * @brief Encode the fourth 6 bits in the 3 byte block
 *
 * @param[in] byte Third input byte in the block
 * @return Base64 encoded character
*/
static inline char encode_to_4th_char(const uint8_t byte) {
    return encoding_table[byte & 0x3f];
}

/**
 * @brief Encode 3 bytes of the input to Base64 encoding characters
 *
 * @param[out] dest Pointer to the Base64 encoded characters
 * @param[in] src Pointer to the input bytes
 * @param[in] num_remaining_bytes The number of remaining bytes in the input
 * @param[in] use_padding Use padding
*/
static void encode_to_4chars(char* dest, const uint8_t* src, const int num_remaining_bytes, const bool use_padding) {
    dest[0] = encode_to_1st_char(src[0]);
    switch (num_remaining_bytes) {
        case 1:
            dest[1] = encode_to_2nd_char(src[0], 0x00);
            if (use_padding) {
                dest[2] = PADDING;
                dest[3] = PADDING;
            } else {
                dest[2] = CHAR_NULL;
                dest[3] = CHAR_NULL;
            }
            break;
        case 2:
            dest[1] = encode_to_2nd_char(src[0], src[1]);
            dest[2] = encode_to_3rd_char(src[1], 0x00);
            if (use_padding) {
                dest[3] = PADDING;
            } else {
                dest[3] = CHAR_NULL;
            }
            break;
        default:
            dest[1] = encode_to_2nd_char(src[0], src[1]);
            dest[2] = encode_to_3rd_char(src[1], src[2]);
            dest[3] = encode_to_4th_char(src[2]);
            break;
    }
}

/**
 * @brief Encode input bytes to Base64 encoded string
 *
 * @param[out] length Length of the encoded string
 * @param[in] src Pointer to the input bytes
 * @param[in] src_size Byte size of the input
 * @param[in] use_padding Use padding
 * @param[in] line_length Length to insert linebreak, if 0, no linebreaks
 * @return Pointer to the encoded string
 * @retval NULL if encoding failed
*/
static char* encode(size_t* length, const uint8_t* src, const size_t src_size, const bool use_padding, const size_t line_length) {
    size_t encoded_byte_size = get_encoded_byte_size(src_size, use_padding, line_length);
    if (encoded_byte_size == 0) {
        return NULL;
    }

    bool insert_crlf = (line_length > 0) ? true : false;

    char* buf = malloc(sizeof(char) * encoded_byte_size);
    if (buf == NULL) {
        return NULL;
    }

    const uint8_t* input_bytes = src;
    size_t buf_index = 0;

    int num_encoded_chars = 0;
    int64_t num_remaining_bytes = (int64_t)src_size;

    char encoded_chars[4];
    while (num_remaining_bytes > 0) {
        // Convert 3 input characters to 4 base64-encoded characters
        encode_to_4chars(encoded_chars, input_bytes, num_remaining_bytes, use_padding);

        input_bytes += 3;
        num_remaining_bytes -= 3;

        size_t num_chars = strlen(encoded_chars);
        for (size_t i = 0; i < num_chars; ++i) {
            buf[buf_index] = encoded_chars[i];
            ++num_encoded_chars;
            ++buf_index;
            // Insert CRLF if required
            // Skip at the end of encoded string
            if (insert_crlf && (buf_index < (encoded_byte_size - 1))) {
                if ((num_encoded_chars % line_length) == 0) {
                    buf[buf_index] = CHAR_CR;
                    buf[buf_index + 1] = CHAR_LF;
                    buf_index += 2;
                }
            }
        }
    }

    // Terminate encoded string
    buf[buf_index] = CHAR_NULL;

    *length = strlen(buf);

    return buf;
}

/**
 * @brief Set the last 2 (62nd and 63rd) characters in the encoding table
 *
 * @param[in] encoding_char_62nd 62nd encoding character
 * @param[in] encoding_char_63rd 63rd encoding character
*/
static inline void set_last2_encoding_chars(const char encoding_char_62nd, const char encoding_char_63rd) {
    encoding_table[62] = encoding_char_62nd;
    encoding_table[63] = encoding_char_63rd;
}

/** Last2 encoding characters for the standard encoding */
static char standard_encoding_chars[] = { '+', '/' };

/** Last 2 encoding characters for the URL-safe encoding */
static char url_safe_encoding_chars[] = { '-', '_' };

char* b64_encode(size_t* length, const void* src, const size_t src_size, char last_2_encoding_chars[2], const bool use_padding, const size_t line_length) {
    set_last2_encoding_chars(last_2_encoding_chars[0], last_2_encoding_chars[1]);

    return encode(length, src, src_size, use_padding, line_length);
}

char* b64_std_encode(size_t* length, const void* src, const size_t src_size) {
    return b64_encode(length, src, src_size, standard_encoding_chars, true, 0);
}

char* b64_url_encode(size_t* length, const void* src, const size_t src_size) {
    return b64_encode(length, src, src_size, url_safe_encoding_chars, false, 0);
}

char* b64_mime_encode(size_t* length, const void* src, const size_t src_size) {
    return b64_encode(length, src, src_size, standard_encoding_chars, true, 76);
}


/**
 * @brief Verify the input character
 *
 * @param[in] c Input character
 * @retval true if the character is valid as Base64 character
 * @retval false if the character is not valid
*/
static inline bool is_valid_b64_char(const char c) {
    if (isalnum(c) || (c == encoding_table[62]) || (c == encoding_table[63])) {
        return true;
    }

    return false;
}

/**
 * @brief Verify the input string
 *
 * @param[in] str Input string
 * @retval true if the string is valid as Base64 string
 * @retval false if the string is not valid
*/
static bool is_valid_b64_string(const char* str) {
    size_t length = strlen(str);

    for (size_t i = 0; i < length; ++i) {
        const char c = str[i];
        if (!is_valid_b64_char(c) && (c != CHAR_CR) && (c != CHAR_LF) && (c != PADDING)) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Get Base64 decoded byte size
 *
 * @param[in] src Pointer to the input string
 * @return Byte size of the decoded bytes
*/
static size_t get_decoded_size(const char* src) {
    size_t src_size = strlen(src);
    if (src_size == 0) {
        return 0;
    }

    size_t num_encoding_chars = 0;
    for (size_t i = 0; i < src_size; ++i) {
        if (is_valid_b64_char(src[i])) {
            num_encoding_chars++;
        }
    }

    size_t decoded_size = num_encoding_chars / 4 * 3;
    size_t remaining_bytes = num_encoding_chars % 4;
    if (remaining_bytes > 0) {
        if (remaining_bytes == 1) {
            // It means that the remaining length of the input is less than
            // 1 byte (6bit), therefore decoding fails
            return 0;
        }
        decoded_size += (remaining_bytes - 1);
    }

    return decoded_size;
}

/**
 * @brief Decode a input character with the encoding table
 *
 * @param[in] c Input character
 * @return Decoded byte value, 0 to 63
 * @retval 64 if decoding failed
*/
static uint8_t decode_b64_char(const char c) {
    uint8_t index;
    for (index = 0; index < sizeof(encoding_table); ++index) {
        if (c == encoding_table[index]) {
            return index;
        }
    }

    // Not reached here,
    // because only valid character is passed
    return sizeof(encoding_table);
}

/**
 * @brief Decode the first 8 bits in the 4 encoded characters
 *
 * @param[in] char1 First input character in the block
 * @param[in] char2 Second input character in the block
 * @return Base64 decoded byte
*/
static inline uint8_t decode_to_1st_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x3f) << 2) | ((decode_b64_char(char2) & 0x30) >> 4);
}

/**
 * @brief Decode the second 8 bits in the 4 encoded characters
 *
 * @param[in] char1 Second input character in the block
 * @param[in] char2 Third input character in the block
 * @return Base64 decoded byte
*/
static inline uint8_t decode_to_2nd_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x0f) << 4) | ((decode_b64_char(char2) & 0x3c) >> 2);
}

/**
 * @brief Decode the third 8 bits in the 4 encoded characters
 *
 * @param[in] char1 Third input character in the block
 * @param[in] char2 Fourth input character in the block
 * @return Base64 decoded byte
*/
static inline uint8_t decode_to_3rd_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x03) << 6) | (decode_b64_char(char2) & 0x3f);
}

/**
 * @brief Encode 4 characters of the input string to original bytes
 *
 * @param[out] dest Pointer to the decoded bytes
 * @param[in] src Pointer to the input string
 * @param[in] num_to_decode The number of remaining bytes in the input
*/
static void decode_to_3bytes(uint8_t* dest, const char* src, const int num_to_decode) {
    switch (num_to_decode) {
        case 2:
            dest[0] = decode_to_1st_byte(src[0], src[1]);
            break;
        case 3:
            dest[0] = decode_to_1st_byte(src[0], src[1]);
            dest[1] = decode_to_2nd_byte(src[1], src[2]);
            break;
        case 4:
            dest[0] = decode_to_1st_byte(src[0], src[1]);
            dest[1] = decode_to_2nd_byte(src[1], src[2]);
            dest[2] = decode_to_3rd_byte(src[2], src[3]);
            break;
        default:
            break;
    }
}

/**
 * @brief Decode input Base64 string to byte array
 *
 * @param[out] size Byte size of the decoded byte array
 * @param[in] src Pointer to the input Base64 string
 * @param[in] validate Validate the input string
 * @return Pointer to the decoded byte array
 * @retval NULL if decoding failed
*/
static void* decode(size_t* size, const char* src, const bool validate) {
    if (validate) {
        if (!is_valid_b64_string(src)) {
            return NULL;
        }
    }

    size_t decoded_size = get_decoded_size(src);
    if (decoded_size == 0) {
        return NULL;
    }

    uint8_t* buf = malloc(sizeof(uint8_t) * decoded_size);
    if (buf == NULL) {
        return NULL;
    }

    const char* input_char = src;
    size_t buf_index = 0;

    int64_t num_remaining_chars = (int64_t)strlen(src);

    char decoding_chars[4];
    while (num_remaining_chars > 0) {
        const int max_num_to_decode = (num_remaining_chars >= 4) ? 4 : num_remaining_chars;
        int num_to_decode = 0;
        while (num_to_decode < max_num_to_decode) {
            // Finish decoding when reached to null or padding character
            if ((*input_char == CHAR_NULL) || (*input_char == PADDING)) {
                break;
            }
            // Put a character to be decoded into temporal buffer
            if (is_valid_b64_char(*input_char)) {
                decoding_chars[num_to_decode] = *input_char;
                ++input_char;
                ++num_to_decode;
            } else {
                // Skip non encoding characters or CRLF
                ++input_char;
                continue;
            }
        }

        // Convert 4 input characters to 3 base64-decoded bytes
        decode_to_3bytes(&buf[buf_index], decoding_chars, num_to_decode);

        buf_index += (num_to_decode - 1);

        num_remaining_chars -= 4;
    }

    *size = decoded_size;

    return (void*)buf;
}

void* b64_decode(size_t* size, const char* src, char last_2_encoding_chars[2], const bool validate) {
    set_last2_encoding_chars(last_2_encoding_chars[0], last_2_encoding_chars[1]);

    return decode(size, src, validate);
}

void* b64_std_decode(size_t* size, const char* src) {
    return b64_decode(size, src, standard_encoding_chars, true);
}

void* b64_url_decode(size_t* size, const char* src) {
    return b64_decode(size, src, url_safe_encoding_chars, true);
}

void* b64_mime_decode(size_t* size, const char* src) {
    return b64_decode(size, src, standard_encoding_chars, false);
}
