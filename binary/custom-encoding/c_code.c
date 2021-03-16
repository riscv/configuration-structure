#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct {
    const cs_schema_t *schema;
    const uint8_t *encoded;
    int (*const value_callback)(const cs_path_t *path, int value);
    int (*const enter_exit_callback)(const cs_path_t *path, bool enter);
    unsigned offset;
    cs_path_t path;
} cs_decoder_t;

static inline bool is_builtin(unsigned type);
static int decode_builtin(cs_decoder_t *decoder, unsigned type);
static int needs_length_builtin(unsigned type);

static void debug(const cs_decoder_t *decoder, const char *format, ...)
{
#ifdef DEBUG
    va_list args;
    va_start(args, format);

    for (unsigned i = 0; i < decoder->path.depth; i++)
        printf("  ");
    vprintf(format, args);
#endif
}

static int decode(cs_decoder_t *decoder, unsigned type,
        bool repeatable, bool required);

static unsigned get_bits(cs_decoder_t *decoder, unsigned n) {
    unsigned result = 0;
    while (n > 0) {
        result <<= 1;
        uint8_t byte = decoder->encoded[decoder->offset / 8];
        if (byte & (1 << (7 - (decoder->offset % 8)))) {
            result |= 1;
        }
        decoder->offset++;
        n--;
    }
    return result;
}

#define DIM(x)  (sizeof(x)/sizeof(*(x)))
static unsigned decode_number(cs_decoder_t *decoder)
{
    static const uint8_t number_bits[] = {3, 4, 5, 7, 10, 14, 19, 25, 32, 40};
    unsigned value = 0;
    unsigned offset = 0;
    for (unsigned i = 0; i < DIM(number_bits); i++) {
        unsigned length = number_bits[i];

        unsigned data = get_bits(decoder, length);
        value = value | (data << offset);
        offset += length;
        unsigned more = get_bits(decoder, 1);
        if (!more)
            break;
    }
    debug(decoder, "decode_number() -> %d\n", value);
    return value;
}

static unsigned decode_fixed(cs_decoder_t *decoder, unsigned length)
{
    unsigned value = 0;
    for (unsigned i = 0; i < length; i++) {
        value <<= 1;
        value |= get_bits(decoder, 1);
    }
    debug(decoder, "decode_fixed(%d) -> %d\n", length, value);
    return value;
}

static void path_push(cs_decoder_t *decoder, unsigned value)
{
    assert(decoder->path.depth < DIM(decoder->path.values));
    decoder->path.values[decoder->path.depth++] = value;
}

static void path_pop(cs_decoder_t *decoder)
{
    assert(decoder->path.depth > 0);
    decoder->path.depth--;
}

static bool needs_length(cs_decoder_t *decoder, unsigned type)
{
    if (is_builtin(type)) {
        return needs_length_builtin(type);

    } else {
        const cs_typedef_t *typdef = &decoder->schema->types[type];
        const cs_typedef_flags_t *all_flags = decoder->schema->all_flags;
        for (unsigned i = 0; i < typdef->entry_count; i++) {
            if (!(all_flags[typdef->entry_index + i] & CS_FLAG_REQUIRED))
                return true;
        }
    }
    return false;
}

static int decode_schema_type(cs_decoder_t *decoder, unsigned type)
{
    debug(decoder, "decode_schema_type(%d)\n", type);

    unsigned end;
    if (needs_length(decoder, type)) {
        end = decode_number(decoder);
        end += decoder->offset;
        debug(decoder, "got length\n");
    } else {
        end = decoder->offset;
    }

    if (decoder->enter_exit_callback)
        decoder->enter_exit_callback(&decoder->path, true);

    const cs_typedef_t *typdef = &decoder->schema->types[type];
    const cs_typedef_entry_t *all_entries = decoder->schema->all_entries;
    const cs_typedef_flags_t *all_flags = decoder->schema->all_flags;
    for (unsigned i = 0; i < typdef->entry_count; i++) {
        const cs_typedef_entry_t *entry = &all_entries[typdef->entry_index + i];
        const cs_typedef_flags_t flags = all_flags[typdef->entry_index + i];
        if (!(flags & CS_FLAG_REQUIRED))
            continue;
        path_push(decoder, entry->code);
        decode(decoder, entry->type, flags & CS_FLAG_REPEATABLE, true);
        path_pop(decoder);
    }

    while (decoder->offset < end) {
        unsigned i;
        unsigned code = decode_number(decoder);
        debug(decoder, "got code %d\n", code);
        for (i = 0; i < typdef->entry_count; i++) {
            const cs_typedef_entry_t *entry = &all_entries[typdef->entry_index + i];
            const cs_typedef_flags_t flags = all_flags[typdef->entry_index + i];
            if (entry->code == code) {
                path_push(decoder, code);
                decode(decoder, entry->type,
                            flags & CS_FLAG_REPEATABLE,
                            flags & CS_FLAG_REQUIRED);
                path_pop(decoder);
                break;
            }
        }
        if (i == typdef->entry_count) {
          //
          // Description code is not supported, skip this description.
          //
          unsigned int skip_length = decode_number(decoder);
          debug(decoder, "Not supported type/code: %d %d, skip %d bits\n", type, code, skip_length);
          decoder->offset += skip_length;
        }
    }

    if (decoder->enter_exit_callback)
        decoder->enter_exit_callback(&decoder->path, false);

    return 0;
}

static int decode(cs_decoder_t *decoder, unsigned type,
        bool repeatable, bool required)
{
    unsigned end;
    if (repeatable) {
        end = decode_number(decoder);
        end += decoder->offset;
        debug(decoder, "got length\n");
    } else {
        end = decoder->offset + 1;
    }
    while (decoder->offset < end) {
        if (is_builtin(type)) {
            decode_builtin(decoder, type);
        } else {
            decode_schema_type(decoder, type);
        }
    }
    return 0;
}

int cs_decode(
    const cs_schema_t *schema,
    int (*value_callback)(const cs_path_t *path, int value),
    int (*enter_exit_callback)(const cs_path_t *path, bool enter),
    uint8_t *encoded,
    unsigned type)
{
    cs_decoder_t decoder = {
        .schema = schema,
        .encoded = encoded,
        .value_callback = value_callback,
        .enter_exit_callback = enter_exit_callback,
        .offset = 0
    };

    return decode(&decoder, type, false, true);
}
