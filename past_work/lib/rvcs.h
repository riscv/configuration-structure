#ifndef RVCS_H
#define RVCS_H

#include "Top.h"

typedef enum {
	RVCS_DECODE_SUCCESS,
	RVCS_DECODE_FAILED
} rvcs_decode_result_t;

/**
 * @brief Decode a single configuration structure, ignoring pointers to other
 * structures.
 * 
 * @param decoded
 * @param cs Pointer to the first byte of the configuration structure.
 * @return rvcs_decode_result_t 
 */
rvcs_decode_result_t rvcs_decode_single(Top_t **top, const void *cs);

#endif