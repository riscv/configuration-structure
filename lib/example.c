#include <asn_application.h>
#include <per_decoder.h>

#include "Configuration.h"

int main() {
	asn_codec_ctx_t asn_code_ctx = {
		.max_stack_size = 65535
	};
	asn_dec_rval_t result = uper_decode_complete(&asn_code_ctx,
				    &asn_DEF_Configuration,
				    // TODO:
				    NULL, NULL, 1);
	return result.code != RC_OK;
}
