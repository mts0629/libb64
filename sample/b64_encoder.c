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

uint8_t* input_bytes = NULL;
char* encoded_str = NULL;

// Free dynamically allocated memories
void free_memories(void) {
    free(input_bytes);
    free(encoded_str);
}

// Write base64 encoded string to a file
bool write_b64_str_to_file(const char* encoded_str, const char* fname) {
    FILE* fp = fopen(fname,"wb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open %s\n", fname);
        return false;
    }

    size_t i = 0;
    while (encoded_str[i] != '\0') {
        fwrite(&encoded_str[i], sizeof(char), 1, fp);
        ++i;
    }

    fclose(fp);

    return true;
}

int main(int argc, char* argv[]) {
    atexit(free_memories);

    if (argc < 2) {
        fprintf(stderr, "Error: input file is not specified\n");
        fprintf(stdout, "usage: %s input_file [output_name]('encoded.txt')\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* fname = argv[1];
    const char* out_fname = "encoded.txt";
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

    input_bytes = malloc(sizeof(uint8_t) * (size_t)fsize);
    if (input_bytes == NULL) {
        fprintf(stderr, "Error: failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    size_t read_size = fread(input_bytes, sizeof(uint8_t), (size_t)fsize, fp);
    if (read_size != (size_t)fsize) {
        fprintf(stderr, "Error: failed to read %s\n", fname);
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    int osize;
    char* encoded_str = b64_encode(&osize, input_bytes, (size_t)fsize);

    printf("Base64 encoding of %s is finished (%lu to %d bytes).\n", fname, read_size, osize);

    if (!write_b64_str_to_file(encoded_str, out_fname)) {
        exit(EXIT_FAILURE);
    }

    printf("The string is written to '%s'.\n", out_fname);

    free(encoded_str);

    return EXIT_SUCCESS;
}
