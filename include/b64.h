#ifndef B64_H
#define B64_H

#include <stddef.h>
#include <stdint.h>

/**
 * @def Error by containing invalid Base64 character
 */
#define B64_ERROR_INVALID_CHAR -1

/**
 * @def Error by remaining last bits are not enough to 1 byte
 */
#define B64_ERROR_REMAINING_BITS -2

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
 * @brief Encode byte array by Base64 for MIME
 * 
 * @param[out] encoded_str URL-safe Base64-encoded output string
 * @param[in] input_bytes Input byte array
 * @param[in] input_size_in_bytes Input size in bytes
 * @return Byte size of the encoded string, NULL-terminated
 */
int b64_mime_encode(char* encoded_str, const uint8_t* input_bytes, const size_t input_size_in_bytes);

/**
 * @brief Decode Base64 string
 * 
 * @param[out] decoded_bytes Decoded byte array
 * @param[in] input_string Base64-encoded input string, NULL-terminated
 * @return Byte size of the decoded byte array
 * @retval B64_ERROR_INVALID_CHAR Input string contains invalid character
 * @retval B64_ERROR_REMAINING_BITS The last bits are not enough to 1 byte
 */
int b64_decode(uint8_t* decoded_bytes, const char* input_string);

/**
 * @brief Decode URL-safe Base64 string
 * 
 * @param[out] decoded_bytes Decoded byte array
 * @param[in] input_string URL-safe Base64-encoded input string, NULL-terminated
 * @return Byte size of the decoded byte array
 * @retval B64_ERROR_INVALID_CHAR Input string contains invalid character
 * @retval B64_ERROR_REMAINING_BITS The last bits are not enough to 1 byte
 */
int b64_url_decode(uint8_t* decoded_bytes, const char* input_string);

/**
 * @brief Decode Base64 string for MIME
 * 
 * @param[out] decoded_bytes Decoded byte array
 * @param[in] input_string Base64-MIME-encoded input string, NULL-terminated
 * @return Byte size of the decoded byte array
 * @retval B64_ERROR_INVALID_CHAR Input string contains invalid character
 * @retval B64_ERROR_REMAINING_BITS The last bits are not enough to 1 byte
 */
int b64_mime_decode(uint8_t* decoded_bytes, const char* input_string);

#endif // B64_H
