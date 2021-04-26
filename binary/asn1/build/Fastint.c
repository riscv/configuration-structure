/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Configuration-Structure-Schema"
 * 	found in "../schema.asn"
 * 	`asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler`
 */

#include "Fastint.h"

asn_TYPE_member_t asn_MBR_Fastint_1[] = {
	{ ATF_POINTER, 2, offsetof(struct Fastint, mmodetimeregaddr),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"mmodetimeregaddr"
		},
	{ ATF_POINTER, 1, offsetof(struct Fastint, mmodetimecompregaddr),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"mmodetimecompregaddr"
		},
};
static const int asn_MAP_Fastint_oms_1[] = { 0, 1 };
static const ber_tlv_tag_t asn_DEF_Fastint_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Fastint_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* mmodetimeregaddr */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* mmodetimecompregaddr */
};
asn_SEQUENCE_specifics_t asn_SPC_Fastint_specs_1 = {
	sizeof(struct Fastint),
	offsetof(struct Fastint, _asn_ctx),
	asn_MAP_Fastint_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_Fastint_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	2,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Fastint = {
	"Fastint",
	"Fastint",
	&asn_OP_SEQUENCE,
	asn_DEF_Fastint_tags_1,
	sizeof(asn_DEF_Fastint_tags_1)
		/sizeof(asn_DEF_Fastint_tags_1[0]), /* 1 */
	asn_DEF_Fastint_tags_1,	/* Same as above */
	sizeof(asn_DEF_Fastint_tags_1)
		/sizeof(asn_DEF_Fastint_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_Fastint_1,
	2,	/* Elements count */
	&asn_SPC_Fastint_specs_1	/* Additional specs */
};

