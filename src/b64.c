#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "b64.h"

// Base64 encoding table by 6-bit index and encoded character
static char encoding_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/'
};

// Padding character
static const char padding = '=';

// Characters for new line
#define CHAR_CR '\x0d'
#define CHAR_LF '\x0a'

// Null character
#define CHAR_NULL '\0'

// Get Base64 encoded byte size
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

    // Add CR+LF if required
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

// Encode each 6 bit unit in an encoding block
static inline char encode_to_1st_char(const uint8_t byte) {
    return encoding_table[(byte & 0xfc) >> 2];
}

static inline char encode_to_2nd_char(const uint8_t byte1, const uint8_t byte2) {
    return encoding_table[((byte1 & 0x03) << 4) | ((byte2 & 0xf0) >> 4)];
}

static inline char encode_to_3rd_char(const uint8_t byte1, const uint8_t byte2) {
    return encoding_table[((byte1 & 0x0f) << 2) | ((byte2 & 0xc0) >> 6)];
}

static inline char encode_to_4th_char(const uint8_t byte) {
    return encoding_table[byte & 0x3f];
}

static int encode_to_4chars(char* dest, const uint8_t* src, const int num_remaining_bytes, const bool use_padding) {
    int num_encoded;
    dest[0] = encode_to_1st_char(src[0]);
    switch (num_remaining_bytes) {
        case 1:
            dest[1] = encode_to_2nd_char(src[0], 0x00);
            if (use_padding) {
                dest[2] = padding;
                dest[3] = padding;
            }
            num_encoded = 2;
            break;
        case 2:
            dest[1] = encode_to_2nd_char(src[0], src[1]);
            dest[2] = encode_to_3rd_char(src[1], 0x00);
            if (use_padding) {
                dest[3] = padding;
            }
            num_encoded = 3;
            break;
        default:
            dest[1] = encode_to_2nd_char(src[0], src[1]);
            dest[2] = encode_to_3rd_char(src[1], src[2]);
            dest[3] = encode_to_4th_char(src[2]);
            num_encoded = 4;
            break;
    }

    return num_encoded;
}

// Encode bytes to Base64 string
static char* encode(size_t* length, const uint8_t* src, const size_t src_size, const bool use_padding, const size_t line_length) {
    size_t encoded_byte_size = get_encoded_byte_size(src_size, use_padding, line_length);
    if (encoded_byte_size == 0) {
        return NULL;
    }

    char* buf = malloc(sizeof(char) * encoded_byte_size);
    if (buf == NULL) {
        return NULL;
    }

    int src_index = 0;
    int dest_index = 0;

    int num_encoded_chars = 0;
    int num_remaining_bytes = (int)src_size;

    while (num_remaining_bytes > 0) {
        // Convert 3 input characters to 4 base64-encoded characters
        char encoded_chars[4 + 1] = { '\0' };
        int num_encoded = encode_to_4chars(encoded_chars, &src[src_index], num_remaining_bytes, use_padding);

        int i = 0;
        while (encoded_chars[i] != '\0') {
            buf[dest_index] = encoded_chars[i];
            dest_index++;
            // Insert CR+LF if required
            if (line_length > 0) {
                if (num_remaining_bytes > 0) {
                    if ((dest_index % line_length) == 0) {
                        buf[dest_index] = CHAR_CR;
                        buf[dest_index + 1] = CHAR_LF;
                        dest_index += 2;
                    }
                }
            }
            i++;
        }

        num_encoded_chars += num_encoded;

        if (use_padding || (num_remaining_bytes >= 3)) {
            src_index += 3;

            num_remaining_bytes -= 3;
            // num_encoded_chars += 4;
        } else {
            src_index += num_remaining_bytes;
            // dest_index += (num_remaining_bytes + 1);

            num_remaining_bytes -= num_remaining_bytes;
            // num_encoded_chars += (num_remaining_bytes + 1);
        }
    }

    // Terminate encoded string
    buf[dest_index] = CHAR_NULL;

    *length = strlen(buf);

    return buf;
}

// Set the last 2 (62nd and 63rd) encoding character
static inline void set_last2_encoding_chars(const char encoding_char_62nd, const char encoding_char_63rd) {
    encoding_table[62] = encoding_char_62nd;
    encoding_table[63] = encoding_char_63rd;
}

// Set the last 2 encoding characters for standard encoding
static inline void set_standard_encoding_chars(void) {
    set_last2_encoding_chars('+', '/');
}

// Set the last 2 encoding characters for URL-safe encoding
static inline void set_url_encoding_chars(void) {
    set_last2_encoding_chars('-', '_');
}

char* b64_encode(size_t* length, const void* src, const size_t src_size, char last_2_encoding_chars[2], const bool use_padding, const size_t line_length) {
    set_last2_encoding_chars(last_2_encoding_chars[0], last_2_encoding_chars[1]);

    return encode(length, src, src_size, use_padding, line_length);
}

char* b64_std_encode(size_t* length, const void* src, const size_t src_size) {
    set_standard_encoding_chars();

    return encode(length, src, src_size, true, 0);
}

char* b64_url_encode(size_t* length, const void* src, const size_t src_size) {
    set_url_encoding_chars();

    return encode(length, src, src_size, false, 0);
}

char* b64_mime_encode(size_t* length, const void* src, const size_t src_size) {
    set_standard_encoding_chars();

    return encode(length, src, src_size, true, 76);
}


// Verify that the character is as Base64
static inline bool is_valid_b64_char(const char c) {
    if (isalnum(c) || (c == encoding_table[62]) || (c == encoding_table[63])) {
        return true;
    }

    return false;
}

// Verify that the string is valid as Base64
static bool is_valid_b64_string(const char* str) {
    size_t length = strlen(str);

    for (size_t i = 0; i < length; ++i) {
        const char c = str[i];
        if (!is_valid_b64_char(c) && (c != CHAR_CR) && (c != CHAR_LF) && (c != padding)) {
            return false;
        }
    }

    return true;
}

// Get decoded size
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

// Convert a input character to an index of the base64 encoding table
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

// Decode each Base64 characters in a group of 4 chars
static inline uint8_t decode_to_1st_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x3f) << 2) | ((decode_b64_char(char2) & 0x30) >> 4);
}

static inline uint8_t decode_to_2nd_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x0f) << 4) | ((decode_b64_char(char2) & 0x3c) >> 2);
}

static inline uint8_t decode_to_3rd_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x03) << 6) | (decode_b64_char(char2) & 0x3f);
}

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

// Decode Base64 string to bytes
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

    int src_index = 0;
    int dest_index = 0;

    int num_remaining_chars = (int)strlen(src);

    char decoding_chars[4];
    while (num_remaining_chars > 0) {
        const int max_num_to_decode = (num_remaining_chars >= 4) ? 4 : num_remaining_chars;
        int num_to_decode = 0;
        while (num_to_decode < max_num_to_decode) {
            // Finish decoding when reached to null or padding character
            if ((src[src_index] == CHAR_NULL) || (src[src_index] == padding)) {
                break;
            }
            // Put a character to be decoded into temporal buffer
            if (is_valid_b64_char(src[src_index])) {
                decoding_chars[num_to_decode] = src[src_index];
                ++src_index;
                ++num_to_decode;
            } else {
                // Skip non encoding characters or CRLF
                ++src_index;
                continue;
            }
        }

        if (num_to_decode > 0) {
            // Convert 4 input characters to 3 base64-decoded bytes
            decode_to_3bytes(&buf[dest_index], decoding_chars, num_to_decode);

            dest_index += (num_to_decode - 1);
        }

        num_remaining_chars -= 4;
    }

    *size = decoded_size;

    return (void*)buf;
}

void* b64_std_decode(size_t* size, const char* src) {
    set_standard_encoding_chars();

    return decode(size, src, true);
}

void* b64_url_decode(size_t* size, const char* src) {
    set_url_encoding_chars();

    return decode(size, src, true);
}

void* b64_mime_decode(size_t* size, const char* src) {
    set_standard_encoding_chars();

    return decode(size, src, false);
}
