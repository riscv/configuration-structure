#include <stdio.h>

#include "cs_decode.h"

typedef struct {
    const cs_schema_t *schema;
    const uint8_t *encoded;
    int (*const callback)(const cs_path_t *path, int value);
    unsigned offset;
} cs_decoder_t;

unsigned get_bits(cs_decoder_t *decoder, unsigned n) {
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
int decode_number(cs_decoder_t *decoder)
{
    static const unsigned number_bits[] = {3, 4, 5, 7, 10, 14, 19, 25, 32, 40};
    int value = 0;
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
    printf("decode_number() -> %d\n", value);
    return value;
}

static int decode_boolean(cs_decoder_t *decoder)
{
    return 456;
}

static int decode(cs_decoder_t *decoder, unsigned type,
        bool repeatable, bool required)
{
    switch (type) {
        case BUILTIN_NUMBER:
            return decode_number(decoder);
        case BUILTIN_BOOLEAN:
            return decode_boolean(decoder);
    }

    unsigned length = decode_number(decoder);
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