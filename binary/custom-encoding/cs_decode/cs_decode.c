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

int cs_decode(
    cs_schema_t *schema,
    uint8_t *encoded,
    int (*callback)(const cs_path_t *path, int value))
{
    cs_decoder_t decoder = {
        .schema = schema,
        .encoded = encoded,
        .callback = callback,
        .offset = 0
    };

    int number = decode_number(&decoder);
    printf("%d\n", number);

    return 0;
}