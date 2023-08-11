#ifndef B64_H
#define B64_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Encode byte array by Base64
 * 
 * @param[out] encoded_str Base64-encoded output string
 * @param[in] input_bytes Input byte array
 * @param[in] input_size_in_bytes Input size in bytes
 * @return Byte size of the encoded string, NULL-terminated
 */
int b64_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes);

/**
 * @brief Encode byte array by URL-safe Base64
 * 
 * @param[out] encoded_str URL-safe Base64-encoded output string
 * @param[in] input_bytes Input byte array
 * @param[in] input_size_in_bytes Input size in bytes
 * @return Byte size of the encoded string, NULL-terminated
 */
int b64_url_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes);

/**
 * @brief Decode Base64 string
 * 
 * @param[out] decoded_bytes Decoded byte array
 * @param[in] input_str Base64-encoded input string, NULL-terminated
 * @return Byte size of the decoded byte array
 * @retval 0 Decoding failed
 */
int b64_decode(uint8_t* decoded_bytes, const char* input_str);

/**
 * @brief Decode URL-safe Base64 string
 * 
 * @param[out] decoded_bytes Decoded byte array
 * @param[in] input_str URL-safe Base64-encoded input string, NULL-terminated
 * @return Byte size of the decoded byte array
 * @retval 0 Decoding failed
 */
int b64_url_decode(uint8_t* decoded_bytes, const char* input_str);

#endif // B64_H
