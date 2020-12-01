#ifndef CS_DECODE_H
#define CS_DECODE_H

#include <stdint.h>
#include <stdbool.h>

/* Built-in type. */
enum {
    BUILTIN_NUMBER,
    BUILTIN_BOOLEAN,
    BUILTIN_END
};

typedef struct {
    unsigned code;
    unsigned type;
    bool repeatable;
    bool required;
} cs_typedef_entry_t;

typedef struct {
    unsigned entry_count;
    const cs_typedef_entry_t *entries;
} cs_typedef_t;

typedef struct {
    unsigned type_count;
    const cs_typedef_t *types;
} cs_schema_t;

typedef struct {
    unsigned values[10];
    unsigned depth;
} cs_path_t;

int cs_decode(
    const cs_schema_t *schema,
    int (*callback)(const cs_path_t *path, int value),
    uint8_t *encoded,
    unsigned type);

#endif