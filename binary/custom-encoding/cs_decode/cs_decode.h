#ifndef CS_DECODE_H
#define CS_DECODE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    unsigned code;
    unsigned type;
    bool repeatable;
    bool required;
} cs_typedef_entry_t;

typedef struct {
    unsigned entry_count;
    cs_typedef_entry_t *entries;
} cs_typedef_t;

typedef struct {
    unsigned type_count;
    cs_typedef_t *types;
} cs_schema_t;

typedef struct {
} cs_path_t;

int cs_decode(
    cs_schema_t *schema,
    uint8_t *encoded,
    int (*callback)(const cs_path_t *path, int value));

#endif