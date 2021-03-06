/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Configuration-Structure-Schema"
 * 	found in "../schema.asn"
 * 	`asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler`
 */

#ifndef	_FlexibleRange_H_
#define	_FlexibleRange_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* FlexibleRange */
typedef struct FlexibleRange {
	struct single {
		A_SEQUENCE_OF(long) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *single;
	struct start {
		A_SEQUENCE_OF(long) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *start;
	struct end {
		A_SEQUENCE_OF(long) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *end;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FlexibleRange_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FlexibleRange;
extern asn_SEQUENCE_specifics_t asn_SPC_FlexibleRange_specs_1;
extern asn_TYPE_member_t asn_MBR_FlexibleRange_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _FlexibleRange_H_ */
#include <asn_internal.h>
