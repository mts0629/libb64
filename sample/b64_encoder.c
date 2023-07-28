#include <stdio.h>
#include <stdlib.h>

#include "b64.h"

long get_file_size(const char *file) {
    FILE *fp = fopen(file, "rb");
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: input file is not specified\n");
        return 1;
    }

    long fsize = get_file_size(argv[1]);
    if (fsize == -1) {
        fprintf(stderr, "Error: failed to get file size of %s\n", argv[1]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open %s\n", argv[1]);
        return 1;
    }

    uint8_t* input_bytes = malloc(sizeof(uint8_t) * (size_t)fsize);

    fread(input_bytes, sizeof(uint8_t), fsize, fp);

    size_t b64_size = (size_t)(fsize * 1.34);
    char *encoded_str = malloc(sizeof(char) * b64_size);

    b64_encode(encoded_str, input_bytes, (size_t)fsize);

    printf("%s is encoded with Base64\n", argv[1]);

    for (int i = 0; i < b64_size; ++i) {
        printf("%c", encoded_str[i]);
    }
    printf("\n");

    fclose(fp);

    free(encoded_str);

    free(input_bytes);

    return 0;
}

