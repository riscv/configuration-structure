/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Configuration-Structure-Schema"
 * 	found in "../schema.asn"
 * 	`asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler`
 */

#ifndef	_Configuration_H_
#define	_Configuration_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct TraceModule;
struct Hart;
struct DebugModule;
struct PhysicalMemory;

/* Configuration */
typedef struct Configuration {
	struct harts {
		A_SEQUENCE_OF(struct Hart) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *harts;
	struct debugModule {
		A_SEQUENCE_OF(struct DebugModule) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *debugModule;
	struct TraceModule	*traceModule	/* OPTIONAL */;
	struct physicalMemory {
		A_SEQUENCE_OF(struct PhysicalMemory) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *physicalMemory;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Configuration_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Configuration;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "TraceModule.h"
#include "Hart.h"
#include "DebugModule.h"
#include "PhysicalMemory.h"

#endif	/* _Configuration_H_ */
#include <asn_internal.h>
