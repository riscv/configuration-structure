/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Configuration-Structure-Schema"
 * 	found in "../schema.asn"
 * 	`asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler`
 */

#include "Hart.h"

asn_TYPE_member_t asn_MBR_Hart_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct Hart, hartid),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_FlexibleRange,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"hartid"
		},
	{ ATF_POINTER, 5, offsetof(struct Hart, debug),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Debug,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"debug"
		},
	{ ATF_POINTER, 4, offsetof(struct Hart, isa),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Isa,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"isa"
		},
	{ ATF_POINTER, 3, offsetof(struct Hart, privileged),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Privileged,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"privileged"
		},
	{ ATF_POINTER, 2, offsetof(struct Hart, clic),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Clic,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"clic"
		},
	{ ATF_POINTER, 1, offsetof(struct Hart, fastint),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Fastint,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"fastint"
		},
};
static const int asn_MAP_Hart_oms_1[] = { 1, 2, 3, 4, 5 };
static const ber_tlv_tag_t asn_DEF_Hart_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Hart_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* hartid */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* debug */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* isa */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* privileged */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* clic */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 } /* fastint */
};
asn_SEQUENCE_specifics_t asn_SPC_Hart_specs_1 = {
	sizeof(struct Hart),
	offsetof(struct Hart, _asn_ctx),
	asn_MAP_Hart_tag2el_1,
	6,	/* Count of tags in the map */
	asn_MAP_Hart_oms_1,	/* Optional members */
	5, 0,	/* Root/Additions */
	6,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_Hart = {
	"Hart",
	"Hart",
	&asn_OP_SEQUENCE,
	asn_DEF_Hart_tags_1,
	sizeof(asn_DEF_Hart_tags_1)
		/sizeof(asn_DEF_Hart_tags_1[0]), /* 1 */
	asn_DEF_Hart_tags_1,	/* Same as above */
	sizeof(asn_DEF_Hart_tags_1)
		/sizeof(asn_DEF_Hart_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_Hart_1,
	6,	/* Elements count */
	&asn_SPC_Hart_specs_1	/* Additional specs */
};

