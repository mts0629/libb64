#ifndef B64_H
#define B64_H

#include <stdint.h>

/**
 * @brief Encode byte array by base64
 * 
 * @param[out] encoded_str Base64-encoded output string
 * @param[in] input_bytes Input byte array
 * @param[in] input_size_in_bytes Input size in bytes
 */
void b64_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes);

#endif // B64_H
