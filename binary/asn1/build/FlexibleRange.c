/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "Configuration-Structure-Schema"
 * 	found in "../schema.asn"
 * 	`asn1c -Wdebug-lexer -Wdebug-parser -Wdebug-fixer -Wdebug-compiler`
 */

#include "FlexibleRange.h"

static asn_TYPE_member_t asn_MBR_single_2[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_single_tags_2[] = {
	(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_single_specs_2 = {
	sizeof(struct single),
	offsetof(struct single, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_single_2 = {
	"single",
	"single",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_single_tags_2,
	sizeof(asn_DEF_single_tags_2)
		/sizeof(asn_DEF_single_tags_2[0]) - 1, /* 1 */
	asn_DEF_single_tags_2,	/* Same as above */
	sizeof(asn_DEF_single_tags_2)
		/sizeof(asn_DEF_single_tags_2[0]), /* 2 */
	{ 0, 0, SEQUENCE_OF_constraint },
	asn_MBR_single_2,
	1,	/* Single element */
	&asn_SPC_single_specs_2	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_start_4[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_start_tags_4[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_start_specs_4 = {
	sizeof(struct start),
	offsetof(struct start, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_start_4 = {
	"start",
	"start",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_start_tags_4,
	sizeof(asn_DEF_start_tags_4)
		/sizeof(asn_DEF_start_tags_4[0]) - 1, /* 1 */
	asn_DEF_start_tags_4,	/* Same as above */
	sizeof(asn_DEF_start_tags_4)
		/sizeof(asn_DEF_start_tags_4[0]), /* 2 */
	{ 0, 0, SEQUENCE_OF_constraint },
	asn_MBR_start_4,
	1,	/* Single element */
	&asn_SPC_start_specs_4	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_end_6[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		""
		},
};
static const ber_tlv_tag_t asn_DEF_end_tags_6[] = {
	(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_end_specs_6 = {
	sizeof(struct end),
	offsetof(struct end, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_end_6 = {
	"end",
	"end",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_end_tags_6,
	sizeof(asn_DEF_end_tags_6)
		/sizeof(asn_DEF_end_tags_6[0]) - 1, /* 1 */
	asn_DEF_end_tags_6,	/* Same as above */
	sizeof(asn_DEF_end_tags_6)
		/sizeof(asn_DEF_end_tags_6[0]), /* 2 */
	{ 0, 0, SEQUENCE_OF_constraint },
	asn_MBR_end_6,
	1,	/* Single element */
	&asn_SPC_end_specs_6	/* Additional specs */
};

asn_TYPE_member_t asn_MBR_FlexibleRange_1[] = {
	{ ATF_POINTER, 3, offsetof(struct FlexibleRange, single),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		0,
		&asn_DEF_single_2,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"single"
		},
	{ ATF_POINTER, 2, offsetof(struct FlexibleRange, start),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_start_4,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"start"
		},
	{ ATF_POINTER, 1, offsetof(struct FlexibleRange, end),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		0,
		&asn_DEF_end_6,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"end"
		},
};
static const int asn_MAP_FlexibleRange_oms_1[] = { 0, 1, 2 };
static const ber_tlv_tag_t asn_DEF_FlexibleRange_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_FlexibleRange_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* single */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* start */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* end */
};
asn_SEQUENCE_specifics_t asn_SPC_FlexibleRange_specs_1 = {
	sizeof(struct FlexibleRange),
	offsetof(struct FlexibleRange, _asn_ctx),
	asn_MAP_FlexibleRange_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_FlexibleRange_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	3,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_FlexibleRange = {
	"FlexibleRange",
	"FlexibleRange",
	&asn_OP_SEQUENCE,
	asn_DEF_FlexibleRange_tags_1,
	sizeof(asn_DEF_FlexibleRange_tags_1)
		/sizeof(asn_DEF_FlexibleRange_tags_1[0]), /* 1 */
	asn_DEF_FlexibleRange_tags_1,	/* Same as above */
	sizeof(asn_DEF_FlexibleRange_tags_1)
		/sizeof(asn_DEF_FlexibleRange_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_FlexibleRange_1,
	3,	/* Elements count */
	&asn_SPC_FlexibleRange_specs_1	/* Additional specs */
};

