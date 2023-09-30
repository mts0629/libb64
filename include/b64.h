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
 * @param[out] length Length of the encoded string
 * @param[in] src Pointer to the input byte array
 * @param[in] src_size Byte size of the input
 * @return Pointer to the encoded string, NULL-terminated
 * @retval NULL Encoding failed
 */
char* b64_encode(int* length, const void* src, const size_t src_size);

/**
 * @brief Encode byte array by URL-safe Base64
 * 
 * @param[out] length Length of the encoded string
 * @param[in] src Pointer to the input byte array
 * @param[in] src_size Byte size of the input
 * @return Pointer to the encoded string, NULL-terminated, dynamically allocated
 * @retval NULL Encoding failed
 */
char* b64_url_encode(int* length, const void* src, const size_t src_size);

/**
 * @brief Encode byte array by Base64 for MIME
 * 
 * @param[out] length Length of the encoded string
 * @param[in] src Pointer to the input byte array
 * @param[in] src_size Byte size of the input
 * @return Pointer to the encoded string, NULL-terminated, dynamically allocated
 * @retval NULL Encoding failed
 */
char* b64_mime_encode(int* length, const void* src, const size_t src_size);

/**
 * @brief Decode Base64 string
 * 
 * @param[out] dest Pointer to the output buffer storing decoded bytes
 * @param[in] src Pointer to the input Base64-encoded string, NULL-terminated
 * @return Byte size of the decoded byte array
 * @retval B64_ERROR_INVALID_CHAR Input string contains invalid character
 * @retval B64_ERROR_REMAINING_BITS The last bits are not enough to 1 byte
 */
int b64_decode(void* dest, const char* src);

/**
 * @brief Decode URL-safe Base64 string
 * 
 * @param[out] dest Pointer to the output buffer storing decoded bytes
 * @param[in] src Pointer to the input URL-safe Base64-encoded string, NULL-terminated
 * @return Byte size of the decoded byte array
 * @retval B64_ERROR_INVALID_CHAR Input string contains invalid character
 * @retval B64_ERROR_REMAINING_BITS The last bits are not enough to 1 byte
 */
int b64_url_decode(void* dest, const char* src);

/**
 * @brief Decode Base64 string for MIME
 * 
 * @param[out] dest Pointer to the output buffer storing decoded bytes
 * @param[in] src Pointer to the input Base64-MIME-encoded string, NULL-terminated
 * @return Byte size of the decoded byte array
 * @retval B64_ERROR_INVALID_CHAR Input string contains invalid character
 * @retval B64_ERROR_REMAINING_BITS The last bits are not enough to 1 byte
 */
int b64_mime_decode(void* dest, const char* src);

#endif // B64_H
