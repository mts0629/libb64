#ifndef B64_H
#define B64_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Encode byte array Base64 encoding
 *
 * @param[out] length Length of the encoded string
 * @param[in] src Pointer to the input byte array
 * @param[in] src_size Byte size of the input
 * @param[in] last_2_encoding_chars 62nd/63rd encoding characters
 * @param[in] use_padding Use padding ('=')
 * @param[in] line_length Length to insert linebreak (CRLF) (no linebreaks with 0)
 * @return Pointer to the null-terminated encoded string, dynamically allocated on the heap
 * @retval NULL Encoding failed
 */
char* b64_encode(size_t* length, const void* src, const size_t src_size, char last_2_encoding_chars[2], const bool use_padding, const size_t line_length);

/**
 * @brief Encode byte array by standard Base64 encoding
 *
 * @param[out] length Length of the encoded string
 * @param[in] src Pointer to the input byte array
 * @param[in] src_size Byte size of the input
 * @return Pointer to the null-terminated encoded string, dynamically allocated on the heap
 * @retval NULL Encoding failed
 */
char* b64_std_encode(size_t* length, const void* src, const size_t src_size);

/**
 * @brief Encode byte array by URL-safe Base64 encoding
 *
 * @param[out] length Length of the encoded string
 * @param[in] src Pointer to the input byte array
 * @param[in] src_size Byte size of the input
 * @return Pointer to the null-terminated encoded string, dynamically allocated on the heap
 * @retval NULL Encoding failed
 */
char* b64_url_encode(size_t* length, const void* src, const size_t src_size);

/**
 * @brief Encode byte array by Base64 encoding for MIME
 *
 * @param[out] length Length of the encoded string
 * @param[in] src Pointer to the input byte array
 * @param[in] src_size Byte size of the input
 * @return Pointer to the null-terminated encoded string, dynamically allocated on the heap
 * @retval NULL Encoding failed
 */
char* b64_mime_encode(size_t* length, const void* src, const size_t src_size);

/**
 * @brief Decode standard Base64-encoded string
 *
 * @param[out] size Byte size of the output decoded byte array
 * @param[in] src Pointer to the input null-terminated Base64-encoded string
 * @param[in] last_2_encoding_chars 62nd/63rd encoding characters
 * @param[in] validate Validate characters in the input string
 * @return Pointer to the decoded byte array, dynamically allocated on the heap
 * @retval NULL Decoding failed
 */
void* b64_decode(size_t* size, const char* src, char last_2_encoding_chars[2], const bool validate);

/**
 * @brief Decode standard Base64-encoded string
 *
 * @param[out] size Byte size of the output decoded byte array
 * @param[in] src Pointer to the input null-terminated Base64-encoded string
 * @return Pointer to the decoded byte array, dynamically allocated on the heap
 * @retval NULL Decoding failed
 */
void* b64_std_decode(size_t* size, const char* src);

/**
 * @brief Decode URL-safe Base64-encoded string
 *
 * @param[out] size Byte size of the output decoded byte array
 * @param[in] src Pointer to the input null-terminated URL-safe Base64-encoded string
 * @return Pointer to the decoded byte array, dynamically allocated on the heap
 * @retval NULL Decoding failed
 */
void* b64_url_decode(size_t* size, const char* src);

/**
 * @brief Decode Base64-encoded string for MIME
 *
 * @param[out] size Byte size of the output decoded byte array
 * @param[in] src Pointer to the input null-terminated Base64-MIME-encoded string
 * @return Pointer to the decoded byte array, dynamically allocated on the heap
 * @retval NULL Decoding failed
 */
void* b64_mime_decode(size_t* size, const char* src);

#endif // B64_H
