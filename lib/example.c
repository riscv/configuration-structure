#include <asn_application.h>
#include <per_decoder.h>

#include "Top.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Error: Need exactly one argument.\n");
		return 1;
	}

	const char *path = argv[1];
	FILE *f = fopen(path, "r");
	if (!f) {
		fprintf(stderr, "Failed to open %s\n", path);
		return 1;
	}

	if (fseek(f, 0, SEEK_END) != 0) {
		fprintf(stderr, "fseek() failed\n");
		return 1;
	}
	int size = ftell(f);

	char *bytes = malloc(size);
	if (!bytes) {
		fprintf(stderr, "malloc(%d) failed\n", size);
		return 1;
	}
	if (fseek(f, 0, SEEK_SET) != 0) {
		fprintf(stderr, "fseek() failed\n");
		return 1;
	}
	if (fread(bytes, 1, size, f) != size) {
		fprintf(stderr, "fread() didn't read entire file\n");
		return 1;
	}

	asn_codec_ctx_t asn_code_ctx = {
		.max_stack_size = 65535
	};

	void *structure = 0;
	asn_dec_rval_t result = uper_decode_complete(&asn_code_ctx,
				    &asn_DEF_Top,
				    &structure, bytes, size);
	switch (result.code) {
		case RC_OK:
			break;
		case RC_FAIL:
			fprintf(stderr, "uper_decode failed\n");
			return 1;
		case RC_WMORE:
			fprintf(stderr, "uper_decode asked for more bytes\n");
			return 1;
	}

	asn_fprint(stdout, &asn_DEF_Top, structure);

	return result.code != RC_OK;
}
