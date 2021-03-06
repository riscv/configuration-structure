/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Configuration-Structure-Schema"
 * 	found in "../schema.asn"
 * 	`asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler`
 */

#ifndef	_AbstractCommand_H_
#define	_AbstractCommand_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <BOOLEAN.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct AccessRegisterCommand;
struct AccessMemoryCommand;

/* AbstractCommand */
typedef struct AbstractCommand {
	struct accessRegister {
		A_SEQUENCE_OF(struct AccessRegisterCommand) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *accessRegister;
	struct quickAccess {
		A_SEQUENCE_OF(BOOLEAN_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *quickAccess;
	struct accessMemory {
		A_SEQUENCE_OF(struct AccessMemoryCommand) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *accessMemory;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} AbstractCommand_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AbstractCommand;
extern asn_SEQUENCE_specifics_t asn_SPC_AbstractCommand_specs_1;
extern asn_TYPE_member_t asn_MBR_AbstractCommand_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "AccessRegisterCommand.h"
#include "AccessMemoryCommand.h"

#endif	/* _AbstractCommand_H_ */
#include <asn_internal.h>
