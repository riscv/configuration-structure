#include <stdio.h>
#include <stdlib.h>

#include "cs_decode.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        return 1;
    }

    uint8_t *encoded;
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        return 2;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    encoded = malloc(size);
    fseek(fp, 0L, SEEK_SET);
    size_t read = fread(encoded, 1, size, fp);
    if (read != size) {
        return 3;
    }
    fclose(fp);

    cs_decode(NULL, encoded, NULL);
}