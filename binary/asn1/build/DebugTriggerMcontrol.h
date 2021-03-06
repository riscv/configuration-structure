/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Configuration-Structure-Schema"
 * 	found in "../schema.asn"
 * 	`asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler`
 */

#ifndef	_DebugTriggerMcontrol_H_
#define	_DebugTriggerMcontrol_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <BOOLEAN.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DebugTriggerMcontrol */
typedef struct DebugTriggerMcontrol {
	long	 maskmax;
	BOOLEAN_t	 hit	/* DEFAULT FALSE */;
	BOOLEAN_t	 addressMatch	/* DEFAULT FALSE */;
	BOOLEAN_t	 dataMatch	/* DEFAULT FALSE */;
	BOOLEAN_t	 timingBefore	/* DEFAULT FALSE */;
	BOOLEAN_t	 timingAfter	/* DEFAULT FALSE */;
	BOOLEAN_t	 sizeAny	/* DEFAULT FALSE */;
	BOOLEAN_t	 sizeS8	/* DEFAULT FALSE */;
	BOOLEAN_t	 sizeS16	/* DEFAULT FALSE */;
	BOOLEAN_t	 sizeS32	/* DEFAULT FALSE */;
	BOOLEAN_t	 sizeS64	/* DEFAULT FALSE */;
	BOOLEAN_t	 sizeS80	/* DEFAULT FALSE */;
	BOOLEAN_t	 sizeS96	/* DEFAULT FALSE */;
	BOOLEAN_t	 sizeS112	/* DEFAULT FALSE */;
	BOOLEAN_t	 sizeS128	/* DEFAULT FALSE */;
	BOOLEAN_t	 actionMmode	/* DEFAULT FALSE */;
	BOOLEAN_t	 actionDmode	/* DEFAULT FALSE */;
	BOOLEAN_t	 chain	/* DEFAULT FALSE */;
	BOOLEAN_t	 matchEqual	/* DEFAULT FALSE */;
	BOOLEAN_t	 matchNapot	/* DEFAULT FALSE */;
	BOOLEAN_t	 matchGreaterEqual	/* DEFAULT FALSE */;
	BOOLEAN_t	 matchLess	/* DEFAULT FALSE */;
	BOOLEAN_t	 matchLowMask	/* DEFAULT FALSE */;
	BOOLEAN_t	 matchHighMask	/* DEFAULT FALSE */;
	BOOLEAN_t	 matchNotEqual	/* DEFAULT FALSE */;
	BOOLEAN_t	 matchNotNapot	/* DEFAULT FALSE */;
	BOOLEAN_t	 matchNotLowMask	/* DEFAULT FALSE */;
	BOOLEAN_t	 matchNotHighMask	/* DEFAULT FALSE */;
	BOOLEAN_t	 m	/* DEFAULT FALSE */;
	BOOLEAN_t	 s	/* DEFAULT FALSE */;
	BOOLEAN_t	 u	/* DEFAULT FALSE */;
	BOOLEAN_t	 execute	/* DEFAULT FALSE */;
	BOOLEAN_t	 store	/* DEFAULT FALSE */;
	BOOLEAN_t	 load	/* DEFAULT FALSE */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DebugTriggerMcontrol_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DebugTriggerMcontrol;
extern asn_SEQUENCE_specifics_t asn_SPC_DebugTriggerMcontrol_specs_1;
extern asn_TYPE_member_t asn_MBR_DebugTriggerMcontrol_1[34];

#ifdef __cplusplus
}
#endif

#endif	/* _DebugTriggerMcontrol_H_ */
#include <asn_internal.h>
