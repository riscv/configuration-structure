#include <asn_application.h>
#include <per_decoder.h>

#include "Top.h"

#include "rvcs.h"

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
	long size = ftell(f);

	char *bytes = malloc(size);
	if (!bytes) {
		fprintf(stderr, "malloc(%ld) failed\n", size);
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

	Top_t *top;
	rvcs_decode_result_t result = rvcs_decode_single(&top, bytes);
	switch (result) {
		case RVCS_DECODE_SUCCESS:
			break;
		case RVCS_DECODE_FAILED:
			fprintf(stderr, "rvcs_decode_single() failed\n");
			return 1;
	}

	asn_fprint(stdout, &asn_DEF_Top, top);

	return 0;
}
