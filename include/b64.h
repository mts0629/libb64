#ifndef B64_H
#define B64_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Encode byte array by base64
 * 
 * @param[out] encoded_str Base64-encoded output string
 * @param[in] input_bytes Input byte array
 * @param[in] input_size_in_bytes Input size in bytes
 * @return Byte size of the encoded string, NULL-terminated
 */
size_t b64_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes);

/**
 * @brief Decode base64 string
 * 
 * @param[out] decoded_bytes Base64-decoded byte array
 * @param[in] input_str Input string, NULL-terminated
 * @param[in] input_size_in_bytes Input size in bytes
 * @return Byte size of the encoded string
 * @retval 0 Decoding failed
 */
size_t b64_decode(uint8_t* decoded_bytes, const char* input_str, const size_t input_size_in_bytes);

#endif // B64_H
