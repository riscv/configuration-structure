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
    /* Gets called for every simple value (integer, bool), with the path and
     * value. */
    int (*value_callback)(const cs_path_t *path, int value),
    /* Gets called every time we begin/complete decoding an entry with a custom
     * type. Path contains the complete path to the entry.
     * A decoder can use this to tell what data it's about to receive, and also
     * to differentiate which index in an array the current data belongs to.
     */
    int (*enter_exit_callback)(const cs_path_t *path, bool enter),
    uint8_t *encoded,
    unsigned type);

#endif