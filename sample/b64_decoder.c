#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>

#include "b64.h"

// Get the byte size of the file
long get_file_size(const char* file) {
    FILE* fp = fopen(file, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open %s\n", file);
        return -1;
    }

    if (fseek(fp, 0L, SEEK_END) == 0) {
        long fsize = ftell(fp);
        if (fsize != -1) {
            fclose(fp);
            return fsize;
        }
    }

    fclose(fp);

    return -1;
}

char* input_chars = NULL;
uint8_t* decoded_bytes = NULL;

// Free dynamically allocated memories
void free_memories(void) {
    free(input_chars);
    free(decoded_bytes);
}

// Write base64 decoded bytes to a file
bool write_decoded_bytes_to_file(const uint8_t* decoded_bytes, const size_t size, const char* fname) {
    FILE* fp = fopen(fname,"wb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open %s\n", fname);
        return false;
    }

    fwrite(decoded_bytes, sizeof(uint8_t), size, fp);

    fclose(fp);

    return true;
}

int main(int argc, char* argv[]) {
    atexit(free_memories);

    if (argc < 2) {
        fprintf(stderr, "Error: input file is not specified\n");
        fprintf(stdout, "usage: %s input_file [output_name]('decoded.bin')\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* fname = argv[1];
    const char* out_fname = "decoded.bin";
    if (argc == 3) {
        out_fname = argv[2];
    }

    long fsize = get_file_size(fname);
    if (fsize == -1) {
        fprintf(stderr, "Error: failed to get file size of %s\n", fname);
        exit(EXIT_FAILURE);
    }

    FILE* fp = fopen(fname, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open %s\n", fname);
        exit(EXIT_FAILURE);
    }

    input_chars = malloc(sizeof(char) * (size_t)fsize);
    if (input_chars == NULL) {
        fprintf(stderr, "Error: failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    size_t read_size = fread(input_chars, sizeof(char), (size_t)fsize, fp);
    if (read_size != (size_t)fsize) {
        fprintf(stderr, "Error: failed to read %s\n", fname);
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    decoded_bytes = malloc(sizeof(uint8_t) * (size_t)(fsize * 2));
    if (decoded_bytes == NULL) {
        fprintf(stderr, "Error: failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    int decoded_size = b64_decode(decoded_bytes, input_chars);

    printf("Base64 decoding of %s is finished (%lu to %d bytes).\n", fname, read_size, decoded_size);

    if (!write_decoded_bytes_to_file(decoded_bytes, (size_t)decoded_size, out_fname)) {
        exit(EXIT_FAILURE);
    }

    printf("The byte expression is written to '%s'.\n", out_fname);

    return EXIT_SUCCESS;
}
