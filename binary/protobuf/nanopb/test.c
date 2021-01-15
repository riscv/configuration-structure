#include <stdio.h>
#include <stdlib.h>

#include "stdint.h"

#include "pb_decode.h"
#include "main.pb.h"

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

    pb_istream_t stream = pb_istream_from_buffer(encoded, size);
    Configuration config;
    pb_decode(&stream, &Configuration_msg, &config);

    free(encoded);
}
