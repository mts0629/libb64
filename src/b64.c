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

size_t b64_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes) {
    size_t input_index = 0;
    size_t encoded_index = 0;

    size_t remaining_inputs = input_size_in_bytes;

    while (input_index < input_size_in_bytes) {
        // Convert 3 input characters to 4 base64-encoded characters
        const uint8_t* current_input = &input_bytes[input_index];
        char* current_output = &encoded_str[encoded_index];

        current_output[0] = encode_to_1st_char(current_input[0]);
        switch (remaining_inputs) {
            case 1:
                current_output[1] = encode_to_2nd_char(current_input[0], 0x00);
                current_output[2] = '=';
                current_output[3] = '=';
                break;
            case 2:
                current_output[1] = encode_to_2nd_char(current_input[0], current_input[1]);
                current_output[2] = encode_to_3rd_char(current_input[1], 0x00);
                current_output[3] = '=';
                break;
            default:
                current_output[1] = encode_to_2nd_char(current_input[0], current_input[1]);
                current_output[2] = encode_to_3rd_char(current_input[1], current_input[2]);
                current_output[3] = encode_to_4th_char(current_input[2]);
                break;
        }

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

static uint8_t decode_to_1st_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x3f) << 2) | ((decode_b64_char(char2) & 0x30) >> 4);
}

static uint8_t decode_to_2nd_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x0f) << 4) | ((decode_b64_char(char2) & 0x3c) >> 2);
}

static uint8_t decode_to_3rd_byte(const char char1, const char char2) {
    return ((decode_b64_char(char1) & 0x03) << 6) | (decode_b64_char(char2) & 0x3f);
}

size_t b64_decode(uint8_t* decoded_bytes, const char* input_str, const size_t input_size_in_bytes) {
    size_t input_index = 0;
    size_t output_index = 0;

    size_t remaining_inputs = input_size_in_bytes;
    bool input_is_terminated = false;

    while (input_index < input_size_in_bytes) {
        const char* current_input = &input_str[input_index];
        if ((*current_input == '\0') || input_is_terminated) {
            break;
        }

        const int max_num_to_decode = (remaining_inputs >= 4) ? 4 : remaining_inputs;
        int num_to_decode = 0;
        while (num_to_decode < max_num_to_decode) {
            // Decoding fails when an input string contains invalid character
            if (!is_valid_b64_char(current_input[num_to_decode])) {
                return 0;
            }
            // Byte stream is finished when an input string contains '='
            if (current_input[num_to_decode] == '=') {
                input_is_terminated = true;
                break;
            }
            ++num_to_decode;
        }

        // If num_to_decode == 1, the length of the original input bytes is less than 1byte
        // and the encoding fails
        if (num_to_decode == 1) {
            return 0;
        }

        // Convert 4 input characters to 3 base64-decoded bytes
        uint8_t* current_output = &decoded_bytes[output_index];
        switch (num_to_decode) {
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

        input_index += 4;

        remaining_inputs -= 4;
    }

    return output_index;
}
