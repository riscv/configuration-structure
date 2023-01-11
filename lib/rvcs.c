#include <asn_application.h>

#include "rvcs.h"

rvcs_decode_result_t rvcs_decode_single(Top_t **top, const void *cs)
{
	asn_codec_ctx_t asn_code_ctx = {
		.max_stack_size = 65535
	};

	*top = 0;
	/* The structure itself includes its size. So include a number that is
	 * too large. */
	const size_t size = 65536;
	asn_dec_rval_t result = uper_decode_complete(&asn_code_ctx,
				    &asn_DEF_Top,
				    top, cs, 65536);
	switch (result.code) {
		case RC_OK:
			return RVCS_DECODE_SUCCESS;
		case RC_FAIL:
		case RC_WMORE:
			return RVCS_DECODE_FAILED;
	}
}