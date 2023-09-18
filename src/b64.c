#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
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

// Max line length of encoded string
static const int max_line_length = 76;

// Characters for new line
#define CHAR_CR '\x0d'
#define CHAR_LF '\x0a'

// Null character
#define CHAR_NULL '\0'

static char encode_to_1st_char(const uint8_t byte) {
    return encoding_table[(byte & 0xfc) >> 2];
}

static char encode_to_2nd_char(const uint8_t byte1, const uint8_t byte2) {
    return encoding_table[((byte1 & 0x03) << 4) | ((byte2 & 0xf0) >> 4)];
}

static char encode_to_3rd_char(const uint8_t byte1, const uint8_t byte2) {
    return encoding_table[((byte1 & 0x0f) << 2) | ((byte2 & 0xc0) >> 6)];
}

static char encode_to_4th_char(const uint8_t byte) {
    return encoding_table[byte & 0x3f];
}

static int encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes, const bool use_padding, const bool insert_crlf) {
    int input_index = 0;
    int encoded_index = 0;

    int num_encoded_chars = 0;

    int num_remaining_inputs = (int)input_size_in_bytes;

    while (num_remaining_inputs > 0) {
        // Convert 3 input characters to 4 base64-encoded characters
        const uint8_t* current_input = &input_bytes[input_index];
        char* current_output = &encoded_str[encoded_index];

        current_output[0] = encode_to_1st_char(current_input[0]);
        switch (num_remaining_inputs) {
            case 1:
                current_output[1] = encode_to_2nd_char(current_input[0], 0x00);
                if (use_padding) {
                    current_output[2] = padding;
                    current_output[3] = padding;
                    encoded_index += 4;
                } else {
                    encoded_index += 2;
                }
                break;
            case 2:
                current_output[1] = encode_to_2nd_char(current_input[0], current_input[1]);
                current_output[2] = encode_to_3rd_char(current_input[1], 0x00);
                if (use_padding) {
                    current_output[3] = padding;
                    encoded_index += 4;
                } else {
                    encoded_index += 3;
                }
                break;
            default:
                current_output[1] = encode_to_2nd_char(current_input[0], current_input[1]);
                current_output[2] = encode_to_3rd_char(current_input[1], current_input[2]);
                current_output[3] = encode_to_4th_char(current_input[2]);
                encoded_index += 4;
                num_encoded_chars += 4;
                break;
        }

        input_index += 3;

        num_remaining_inputs -= 3;

        // Insert CR+LF
        if (insert_crlf) {
            if (num_remaining_inputs > 0) {
                if ((num_encoded_chars % max_line_length) == 0) {
                    encoded_str[encoded_index] = CHAR_CR;
                    encoded_str[encoded_index + 1] = CHAR_LF;
                    encoded_index += 2;
                }
            }
        }
    }

    // Terminate encoded string
    encoded_str[encoded_index] = CHAR_NULL;

    return encoded_index;
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

int b64_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes) {
    set_standard_encoding_chars();
    return encode(encoded_str, input_bytes, input_size_in_bytes, true, false);
}

int b64_url_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes) {
    set_url_encoding_chars();
    return encode(encoded_str, input_bytes, input_size_in_bytes, false, false);
}

int b64_mime_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes) {
    set_standard_encoding_chars();
    return encode(encoded_str, input_bytes, input_size_in_bytes, true, true);
}

// Convert a input character to an index of the base64 encoding table
static uint8_t decode_b64_char(const char c) {
    if ((c >= 'A') && (c <= 'Z')) {
        return c - 'A';
    } else if ((c >= 'a') && (c<= 'z')) {
        return c - 'a' + 26;
    } else if ((c >= '0') && (c <= '9')) {
        return c - '0' + 52;
    } else if (c == encoding_table[62]) {
        return 62;
    } else if (c == encoding_table[63]) {
        return 63;
    }

    return 0x00;
}

static bool is_valid_b64_char(const char c) {
    if (isalnum(c) || (c == encoding_table[62]) || (c == encoding_table[63]) || (c == padding)) {
        return true;
    }

    return false;
}

static uint8_t decode_to_1st_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x3f) << 2) | ((decode_b64_char(char2) & 0x30) >> 4);
}

static uint8_t decode_to_2nd_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x0f) << 4) | ((decode_b64_char(char2) & 0x3c) >> 2);
}

static uint8_t decode_to_3rd_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x03) << 6) | (decode_b64_char(char2) & 0x3f);
}

static int decode(uint8_t* decoded_bytes, const char* input_string, const bool skip_non_encoding_character) {
    int input_index = 0;
    int output_index = 0;

    int num_remaining_inputs = (int)strlen(input_string);

    char current_input[4];
    while (num_remaining_inputs > 0) {
        const int max_num_to_decode = (num_remaining_inputs >= 4) ? 4 : num_remaining_inputs;
        int num_to_decode = 0;
        while (num_to_decode < max_num_to_decode) {
            // Finish decoding when reached to NULL character
            if (input_string[input_index] == CHAR_NULL) {
                break;
            }
            if ((input_string[input_index] == CHAR_CR) ||
                (input_string[input_index] == CHAR_LF)) {
                ++input_index;
                continue;
            }
            // Finish decoding when reached to the padding character ('=')
            if (input_string[input_index] == padding) {
                break;
            }
            // Decoding fails when an input string contains invalid character
            if (is_valid_b64_char(input_string[input_index])) {
                current_input[num_to_decode] = input_string[input_index];
                ++input_index;
                ++num_to_decode;
            } else {
                if (skip_non_encoding_character) {
                    ++input_index;
                    continue;
                }
                return B64_ERROR_INVALID_CHAR;
            }
        }

        if (num_to_decode > 0) {
            // Convert 4 input characters to 3 base64-decoded bytes
            uint8_t* current_output = &decoded_bytes[output_index];
            switch (num_to_decode) {
                case 1:
                    // If num_to_decode == 1, the length of the original input bytes is less than 1byte
                    // and the encoding fails
                    return B64_ERROR_REMAINING_BITS;
                    break;
                case 2:
                    current_output[0] = decode_to_1st_byte(current_input[0], current_input[1]);
                    output_index += 1;
                    break;
                case 3:
                    current_output[0] = decode_to_1st_byte(current_input[0], current_input[1]);
                    current_output[1] = decode_to_2nd_byte(current_input[1], current_input[2]);
                    output_index += 2;
                    break;
                case 4:
                    current_output[0] = decode_to_1st_byte(current_input[0], current_input[1]);
                    current_output[1] = decode_to_2nd_byte(current_input[1], current_input[2]);
                    current_output[2] = decode_to_3rd_byte(current_input[2], current_input[3]);
                    output_index += 3;
                    break;
                default:
                    return 0;
                    break;
            }
        }

        num_remaining_inputs -= 4;
    }

    return output_index;
}

int b64_decode(uint8_t* decoded_bytes, const char* input_string) {
    set_standard_encoding_chars();
    return decode(decoded_bytes, input_string, false);
}

int b64_url_decode(uint8_t* decoded_bytes, const char* input_string) {
    set_url_encoding_chars();
    return decode(decoded_bytes, input_string, false);
}

int b64_mime_decode(uint8_t* decoded_bytes, const char* input_string) {
    set_standard_encoding_chars();
    return decode(decoded_bytes, input_string, true);
}
