#include <stdio.h>
#include <stdlib.h>

#include "cs_decode.h"
#include "schema.h"

static int value_callback(const cs_path_t *path, int value)
{
    for (unsigned i = 0; i < path->depth; i++) {
        printf("  ");
    }
    printf("%d: %d\n", path->values[path->depth - 1], value);
    return 0;
}

static int enter_exit_callback(const cs_path_t *path, bool enter)
{
    for (unsigned i = 0; i < path->depth; i++) {
        printf("  ");
    }
    if (enter && path->depth) {
        printf("%d: ", path->values[path->depth - 1]);
    }
    printf("%s\n", enter ? "{" : "}");
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        return 1;
    }

    uint8_t *encoded;
    FILE *fp = fopen(argv[1], "rb");
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

    cs_decode(&schema_schema,
              value_callback,
              enter_exit_callback,
              encoded, TYPE_CONFIGURATION);
    free(encoded);
}