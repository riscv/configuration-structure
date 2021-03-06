/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Configuration-Structure-Schema"
 * 	found in "../schema.asn"
 * 	`asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler`
 */

#ifndef	_AccessMemoryCommand_H_
#define	_AccessMemoryCommand_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BOOLEAN.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* AccessMemoryCommand */
typedef struct AccessMemoryCommand {
	BOOLEAN_t	 aamvirtual0	/* DEFAULT FALSE */;
	BOOLEAN_t	 aamvirtual1	/* DEFAULT FALSE */;
	BOOLEAN_t	 aamsize8	/* DEFAULT FALSE */;
	BOOLEAN_t	 aamsize16	/* DEFAULT FALSE */;
	BOOLEAN_t	 aamsize32	/* DEFAULT FALSE */;
	BOOLEAN_t	 aamsize64	/* DEFAULT FALSE */;
	BOOLEAN_t	 aamsize128	/* DEFAULT FALSE */;
	BOOLEAN_t	 aampostincrementSupported	/* DEFAULT FALSE */;
	BOOLEAN_t	 writeSupported	/* DEFAULT FALSE */;
	BOOLEAN_t	 readSupported	/* DEFAULT FALSE */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} AccessMemoryCommand_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AccessMemoryCommand;
extern asn_SEQUENCE_specifics_t asn_SPC_AccessMemoryCommand_specs_1;
extern asn_TYPE_member_t asn_MBR_AccessMemoryCommand_1[10];

#ifdef __cplusplus
}
#endif

#endif	/* _AccessMemoryCommand_H_ */
#include <asn_internal.h>
