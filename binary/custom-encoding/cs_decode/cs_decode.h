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
} cs_typedef_entry_t;

#define CS_FLAG_REPEATABLE      1
#define CS_FLAG_REQUIRED        2
typedef uint8_t cs_typedef_flags_t;

typedef struct {
    unsigned entry_count;
    unsigned entry_index;
} cs_typedef_t;

typedef struct {
    unsigned type_count;
    const cs_typedef_t *types;
    const cs_typedef_entry_t *all_entries;
    const cs_typedef_flags_t *all_flags;
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