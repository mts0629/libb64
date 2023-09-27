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
 * @def Error by shortage of the output buffer
 */
#define B64_ERROR_BUFFER_SHORTAGE -3

/**
 * @brief Encode byte array by Base64
 * 
 * @param[out] dest Pointer to the output buffer storing Base64-encoded string
 * @param[in] max_dest_size Max byte size of the buffer
 * @param[in] src Pointer to the input byte array
 * @param[in] src_size Byte size of the input
 * @return Byte size of the encoded string, NULL-terminated
 * @retval B64_ERROR_BUFFER_SHORTAGE Shortage of the output buffer
 */
int b64_encode(char* dest, const size_t max_dest_size, const void* src, const size_t src_size);

/**
 * @brief Encode byte array by URL-safe Base64
 * 
 * @param[out] dest Pointer to the output buffer storing URL-safe Base64-encoded string
 * @param[in] max_dest_size Max byte size of the buffer
 * @param[in] src Pointer to the input byte array
 * @param[in] src_size Byte size of the input
 * @return Byte size of the encoded string, NULL-terminated
 * @retval B64_ERROR_BUFFER_SHORTAGE Shortage of the output buffer
 */
int b64_url_encode(char* dest, const size_t max_dest_size, const void* src, const size_t src_size);

/**
 * @brief Encode byte array by Base64 for MIME
 * 
 * @param[out] dest Pointer to the output buffer storing Base64-MIME-encoded string
 * @param[in] max_dest_size Max byte size of the buffer
 * @param[in] src Pointer to the input byte array
 * @param[in] src_size Byte size of the input
 * @return Byte size of the encoded string, NULL-terminated
 * @retval B64_ERROR_BUFFER_SHORTAGE Shortage of the output buffer
 */
int b64_mime_encode(char* dest, const size_t max_dest_size, const void* src, const size_t src_size);

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
