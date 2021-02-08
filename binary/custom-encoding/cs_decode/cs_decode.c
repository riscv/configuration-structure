#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#include "cs_decode.h"

typedef struct {
    const cs_schema_t *schema;
    const uint8_t *encoded;
    int (*const callback)(const cs_path_t *path, int value);
    unsigned offset;
    cs_path_t path;
} cs_decoder_t;

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
    static const unsigned number_bits[] = {3, 4, 5, 7, 10, 14, 19, 25, 32, 40};
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

static bool decode_boolean(cs_decoder_t *decoder)
{
    return decode_fixed(decoder, 1);
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
    if (type == BUILTIN_NUMBER)
        return true;
    if (type == BUILTIN_BOOLEAN)
        return false;
    const cs_typedef_t *typdef = &decoder->schema->types[type - BUILTIN_END];
    const cs_typedef_entry_t *all_entries = decoder->schema->all_entries;
    for (unsigned i = 0; i < typdef->entry_count; i++) {
        if (!all_entries[typdef->entry_index + i].required)
            return true;
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

    const cs_typedef_t *typdef = &decoder->schema->types[type - BUILTIN_END];
    const cs_typedef_entry_t *all_entries = decoder->schema->all_entries;
    for (unsigned i = 0; i < typdef->entry_count; i++) {
        const cs_typedef_entry_t *entry = &all_entries[typdef->entry_index + i];
        if (!entry->required)
            continue;
        path_push(decoder, entry->code);
        decode(decoder, entry->type, entry->repeatable, true);
        path_pop(decoder);
    }

    while (decoder->offset < end) {
        unsigned i;
        unsigned code = decode_number(decoder);
        debug(decoder, "got code %d\n", code);
        for (i = 0; i < typdef->entry_count; i++) {
            const cs_typedef_entry_t *entry = &all_entries[typdef->entry_index + i];
            if (entry->code == code) {
                path_push(decoder, code);
                decode(decoder, entry->type,
                            entry->repeatable,
                            entry->required);
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
        switch (type)
        {
            case BUILTIN_NUMBER:
            {
                unsigned num = decode_number(decoder);
                decoder->callback(&decoder->path, num);
                break;
            }
            case BUILTIN_BOOLEAN:
            {
                unsigned num = decode_boolean(decoder);
                decoder->callback(&decoder->path, num);
                break;
            }
            default:
                decode_schema_type(decoder, type);
                break;
        }
    }
    return 0;
}

int cs_decode(
    const cs_schema_t *schema,
    int (*callback)(const cs_path_t *path, int value),
    uint8_t *encoded,
    unsigned type)
{
    cs_decoder_t decoder = {
        .schema = schema,
        .encoded = encoded,
        .callback = callback,
        .offset = 0
    };

    return decode(&decoder, type, false, true);
}
