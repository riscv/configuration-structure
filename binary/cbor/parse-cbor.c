#include <stdio.h>
#include <string.h>

#include "cbor.h"

/**
 * Print a human-readable form of a range
 * Can be single, range(start+end) or list
 */
void print_range(cbor_item_t *range)
{
    if (cbor_isa_uint(range)) {
        // Single hart
        printf("%d", cbor_get_uint8(range));
    } else if (cbor_isa_map(range)) {
        // Range of harts
        struct cbor_pair *range_map = cbor_map_handle(range);
        size_t range_map_len = cbor_map_size(range);

        for (size_t i = 0; i < range_map_len; i++) {
            cbor_item_t *key = range_map[i].key;
            cbor_item_t *value = range_map[i].value;

            if (0 == strncmp("s", cbor_string_handle(key), (size_t) 1)) {
                printf("%d to ", cbor_get_uint8(value));
            } else if (0 == strncmp("e", cbor_string_handle(key), (size_t) 1)) {
                printf("%d", cbor_get_uint8(value));
            }

        }
    } else if (cbor_isa_array(range)) {
        // List of harts
        size_t harts_len = cbor_array_size(range);
        cbor_item_t **harts = cbor_array_handle(range);

        for (size_t i = 0; i < harts_len; i++) {
            printf("%d", cbor_get_uint8(harts[i]));
            if (i +1 < harts_len) {
                printf(", ");
            }
        }
    }
}

/**
 * Count how many categories are set
 *
 * Some use null as a default value, some lists, which are empty by default.
 */
int count_set_categories(cbor_item_t *categories)
{
    int set_categories_len = 0;

    cbor_item_t *debug = cbor_array_get(categories, 0);
    if (cbor_array_size(debug) > 0) {
        set_categories_len++;
    }

    cbor_item_t *debug_module = cbor_array_get(categories, 1);
    if (!cbor_is_null(debug_module)) {
        set_categories_len++;
    }

    cbor_item_t *isa = cbor_array_get(categories, 2);
    if (cbor_array_size(isa) > 0) {
        set_categories_len++;
    }

    cbor_item_t *priv = cbor_array_get(categories, 3);
    if (!cbor_is_null(priv)) {
        set_categories_len++;
    }

    cbor_item_t *fast_int = cbor_array_get(categories, 4);
    if (!cbor_is_null(fast_int)) {
        set_categories_len++;
    }

    cbor_item_t *trace = cbor_array_get(categories, 5);
    if (!cbor_is_null(trace)) {
        set_categories_len++;
    }

    return set_categories_len;
}

int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    size_t length = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *buffer = malloc(length);
    size_t size = fread(buffer, length, 1, f);
    assert (size == 1);

    /* Assuming `buffer` contains `info.st_size` bytes of input data */
    struct cbor_load_result result;
    cbor_item_t *configuration = cbor_load(buffer, length, &result);

    /* Pretty-print the result */
    //cbor_describe(configuration, stdout);
    //fflush(stdout);

    cbor_item_t *harts = cbor_array_get(configuration, 0);
    cbor_item_t *uncore = cbor_array_get(configuration, 1);

    size_t hart_configs_len = cbor_array_size(harts);
    printf("%ld hart configs present\n", hart_configs_len);

    cbor_item_t **hart_configs = cbor_array_handle(harts);
    for (size_t i = 0; i < hart_configs_len; i++) {
        cbor_item_t *hart_config = hart_configs[i];
        cbor_item_t *range = cbor_array_get(hart_config, 0);
        cbor_item_t *categories = cbor_array_get(hart_config, 1);

        printf("Harts: ");
        print_range(range);

        int set_categories_len = count_set_categories(categories);
        printf("\thave %d different configuration types\n", set_categories_len);
    }

    /* Deallocate the result */
    cbor_decref(&configuration);

    fclose(f);
}
