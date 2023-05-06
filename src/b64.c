#include <stdbool.h>
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

static char encode_1st_char(const uint8_t byte) {
    return encoding_table[(byte & 0xfc) >> 2];
}

static char encode_2nd_char(const uint8_t byte1, const uint8_t byte2) {
    return encoding_table[((byte1 & 0x03) << 4) | ((byte2 & 0xf0) >> 4)];
}

static char encode_3rd_char(const uint8_t byte1, const uint8_t byte2) {
    return encoding_table[((byte1 & 0x0f) << 2) | ((byte2 & 0xc0) >> 6)];
}

static char encode_4th_char(const uint8_t byte) {
    return encoding_table[byte & 0x3f];
}

void b64_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes) {
    size_t input_index = 0;
    size_t encoded_index = 0;

    while (input_index <= input_size_in_bytes) {
        int shortage = input_size_in_bytes - input_index;

        // Convert 3 input characters to 4 base64-encoded characters
        char b64_chars[4];
        b64_chars[0] = encode_1st_char(input_bytes[input_index]);
        switch (shortage) {
            case 1:
                b64_chars[1] = encode_2nd_char(input_bytes[input_index], input_bytes[input_index + 1]);
                b64_chars[2] = encode_3rd_char(input_bytes[input_index + 1], 0x00);
                b64_chars[3] = '=';
                break;
            case 2:
                b64_chars[1] = encode_2nd_char(input_bytes[input_index], 0x00);
                b64_chars[2] = '=';
                b64_chars[3] = '=';
                break;
            default:
                b64_chars[1] = encode_2nd_char(input_bytes[input_index], input_bytes[input_index + 1]);
                b64_chars[2] = encode_3rd_char(input_bytes[input_index + 1], input_bytes[input_index + 2]);
                b64_chars[3] = encode_4th_char(input_bytes[input_index + 2]);
                break;
        }

        encoded_str[encoded_index]     = b64_chars[0];
        encoded_str[encoded_index + 1] = b64_chars[1];
        encoded_str[encoded_index + 2] = b64_chars[2];
        encoded_str[encoded_index + 3] = b64_chars[3];

        input_index += 3;
        encoded_index += 4;
    }

    // Terminate encoded string
    encoded_str[encoded_index] = '\0';
}
