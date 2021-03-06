/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Configuration-Structure-Schema"
 * 	found in "../schema.asn"
 * 	`asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler`
 */

#ifndef	_Isa_H_
#define	_Isa_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BOOLEAN.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Isa */
typedef struct Isa {
	BOOLEAN_t	*riscv32	/* OPTIONAL */;
	BOOLEAN_t	*riscv64	/* OPTIONAL */;
	BOOLEAN_t	*riscv128	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Isa_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Isa;
extern asn_SEQUENCE_specifics_t asn_SPC_Isa_specs_1;
extern asn_TYPE_member_t asn_MBR_Isa_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _Isa_H_ */
#include <asn_internal.h>
