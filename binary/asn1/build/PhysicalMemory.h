/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Configuration-Structure-Schema"
 * 	found in "../schema.asn"
 * 	`asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler`
 */

#ifndef	_PhysicalMemory_H_
#define	_PhysicalMemory_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BOOLEAN.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct Range;

/* PhysicalMemory */
typedef struct PhysicalMemory {
	struct address {
		A_SEQUENCE_OF(struct Range) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} address;
	BOOLEAN_t	*cacheable	/* OPTIONAL */;
	BOOLEAN_t	*lrScSupported	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PhysicalMemory_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PhysicalMemory;
extern asn_SEQUENCE_specifics_t asn_SPC_PhysicalMemory_specs_1;
extern asn_TYPE_member_t asn_MBR_PhysicalMemory_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "Range.h"

#endif	/* _PhysicalMemory_H_ */
#include <asn_internal.h>
