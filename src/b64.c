#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "b64.h"

// Base64 encoding table by 6-bit index and encoded character
static char encoding_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '+', '/'
};

static char encode_1st_byte(const uint8_t byte) {
    return encoding_table[(byte & 0xfc) >> 2];
}

static char encode_2nd_byte(const uint8_t byte1, const uint8_t byte2) {
    return encoding_table[((byte1 & 0x03) << 4) | ((byte2 & 0xf0) >> 4)];
}

static char encode_3rd_byte(const uint8_t byte1, const uint8_t byte2) {
    return encoding_table[((byte1 & 0x0f) << 2) | ((byte2 & 0xc0) >> 6)];
}

static char encode_4th_byte(const uint8_t byte) {
    return encoding_table[byte & 0x3f];
}

size_t b64_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes) {
    size_t input_index = 0;
    size_t encoded_index = 0;

    size_t remaining_inputs = input_size_in_bytes;

    while (input_index < input_size_in_bytes) {
        // Convert 3 input characters to 4 base64-encoded characters
        char b64_chars[4] = { '\0' };
        b64_chars[0] = encode_1st_byte(input_bytes[input_index]);
        switch (remaining_inputs) {
            case 1:
                b64_chars[1] = encode_2nd_byte(input_bytes[input_index], 0x00);
                b64_chars[2] = '=';
                b64_chars[3] = '=';
                break;
            case 2:
                b64_chars[1] = encode_2nd_byte(input_bytes[input_index], input_bytes[input_index + 1]);
                b64_chars[2] = encode_3rd_byte(input_bytes[input_index + 1], 0x00);
                b64_chars[3] = '=';
                break;
            default:
                b64_chars[1] = encode_2nd_byte(input_bytes[input_index], input_bytes[input_index + 1]);
                b64_chars[2] = encode_3rd_byte(input_bytes[input_index + 1], input_bytes[input_index + 2]);
                b64_chars[3] = encode_4th_byte(input_bytes[input_index + 2]);
                break;
        }

        encoded_str[encoded_index]     = b64_chars[0];
        encoded_str[encoded_index + 1] = b64_chars[1];
        encoded_str[encoded_index + 2] = b64_chars[2];
        encoded_str[encoded_index + 3] = b64_chars[3];

        input_index += 3;
        encoded_index += 4;

        remaining_inputs -= 3;
    }

    // Terminate encoded string
    encoded_str[encoded_index] = '\0';

    return encoded_index;
}

// Convert a input character to an index of the base64 encoding table
static uint8_t decode_b64_char(const char c) {
    if ((c >= 'A') && (c <= 'Z')) {
        return c - 'A';
    } else if ((c >= 'a') && (c<= 'z')) {
        return c - 'a' + 26;
    } else if ((c >= '0') && (c <= '9')) {
        return c - '0' + 52;
    } else if (c == '+') {
        return c - '+' + 62;
    } else if (c == '/') {
        return c - '/' + 63;
    }

    return 0x00;
}

static bool is_valid_b64_char(const char c) {
    if (isalnum(c) || (c == '+') || (c == '/') || (c == '=')) {
        return true;
    }

    return false;
}

static uint8_t decode_1st_char(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x3f) << 2) | ((decode_b64_char(char2) & 0x30) >> 4);
}

static uint8_t decode_2nd_char(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x0f) << 4) | ((decode_b64_char(char2) & 0x3c) >> 2);
}

static uint8_t decode_3rd_char(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x03) << 6) | (decode_b64_char(char2) & 0x3f);
}

size_t b64_decode(uint8_t* decoded_bytes, const char* input_str, const size_t input_size_in_bytes) {
    size_t input_index = 0;
    size_t decoded_index = 0;

    size_t remaining_inputs = input_size_in_bytes;
    bool input_is_terminated = false;

    while (input_index < input_size_in_bytes) {
        uint8_t* cur_input = &input_str[input_index];
        if ((*cur_input == '\0') || input_is_terminated) {
            break;
        }

        const int num_to_decode = (remaining_inputs >= 4) ? 4 : remaining_inputs;
        int num_decode;
        for (num_decode = 0; num_decode < num_to_decode; ++num_decode) {
            // Decoding fails when an input string contains invalid character
            if (!is_valid_b64_char(cur_input[num_decode])) {
                return 0;
            }
            // Byte stream is finished when an input string contains '='
            if (cur_input[num_decode] == '=') {
                input_is_terminated = true;
                break;
            }
        }

        // Convert 4 input characters to 3 base64-decoded bytes
        uint8_t* out = &decoded_bytes[decoded_index];
        switch (num_decode) {
            case 1:
                out[0] = decode_1st_char(input_str[input_index], '\0');
                decoded_index += 1;
                break;
            case 2:
                out[0] = decode_1st_char(input_str[input_index], input_str[input_index + 1]);
                out[1] = decode_2nd_char(input_str[input_index + 1], '\0');
                decoded_index += 2;
                break;
            case 3:
                out[0] = decode_1st_char(input_str[input_index], input_str[input_index + 1]);
                out[1] = decode_2nd_char(input_str[input_index + 1], input_str[input_index + 2]);
                out[2] = decode_3rd_char(input_str[input_index + 2], '\0');
                decoded_index += 3;
                break;
            default:
                out[0] = decode_1st_char(input_str[input_index], input_str[input_index + 1]);
                out[1] = decode_2nd_char(input_str[input_index + 1], input_str[input_index + 2]);
                out[2] = decode_3rd_char(input_str[input_index + 2], input_str[input_index + 3]);
                decoded_index += 3;
                break;
        }

        input_index += 4;

        remaining_inputs -= 4;
    }

    return decoded_index;
}
