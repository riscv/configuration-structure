project('RISC-V Configuration Structure Parser', 'c',
	default_options: [
		'optimization=s'
	])

asn_types = [
'Configuration',
'Custom',
'PhysicalAddress',
'ChildStructure',
'DebugModule',
'DebugModuleComplete',
'Hart',
'CustomOctetString',
'Translation',
'DebugModuleSecondary',
'DebugSecondary',
'DebugTrigger',
'AccessMemoryCommand',
'QuickAccessCommand',
'AccessRegisterCommand',
'FlexibleRange',
'Range',
'Debug',
'Zjpm',
'Zk',
'HartInfo',
'DebugComplete',
'ContextInfo',
'TriggerAction',
'TriggerModes',
'TriggerEtrigger',
'TriggerItrigger',
'TriggerIcount',
'TriggerMcontrol6',
'TriggerMcontrol',
'Top',
'SignedConfiguration',
'PrintableString',
'Version',
'H',
'Shcounterenw',
'Shgatpa',
'Shtvala',
'Shvsatpa',
'Shvstvala',
'Shvstvecd',
'Sm1p11',
'Sm1p12',
'Smepmp',
'Smstateen',
'Ss1p11',
'Sscofpmf',
'Ssptead',
'Ssstateen',
'Sstc',
'Ssube',
'Sv32',
'Sv39',
'Sv48',
'Sv57',
'Svinval',
'Svnapot',
'Svpbmt',
'M',
'A',
'F',
'D',
'Q',
'L',
'C',
'B',
'J',
'T',
'P',
'V',
'N',
'Zic64b',
'Zicbom',
'Zicbop',
'Zicboz',
'Zicntr',
'Zicsr',
'Zifencei',
'Zihintpause',
'Zihpm',
'Zmmul',
'Zaamo',
'Zalrsc',
'Zam',
'Zfh',
'Zhinx',
'Zhinxmin',
'Zfhmin',
'Zfinx',
'Zdinx',
'Zba',
'Zbb',
'Zbc',
'Zbkb',
'Zbkc',
'Zbkx',
'Zbs',
'Zve32d',
'Zve32f',
'Zve32x',
'Zve64d',
'Zve64f',
'Zve64x',
'Zkn',
'Zknd',
'Zkne',
'Zknh',
'Zkr',
'Zks',
'Zksed',
'Zksh',
'Zkt',
'Rv32I',
'Rv64I'
]

# glob auto-generate files based on data types
schema_sources = run_command('sh', '-c', 'ls ' + join_paths(meson.source_root(),
		'../schema/*.asn'), check: true).stdout().strip().split('\n')

asn_sources = [
	'BOOLEAN.c',
	'INTEGER.c',
	'NULL.c',
	'OCTET_STRING.c',
	'NativeEnumerated.c',
	'NativeInteger.c',
	'asn_SET_OF.c',
	'asn_codecs_prim.c',
	'ber_decoder.c',	# remove?
	'ber_tlv_length.c',	# remove?
	'ber_tlv_tag.c',	# remove?
	'constraints.c',
	'constr_CHOICE.c',
	'constr_SEQUENCE.c',
	'constr_SEQUENCE_OF.c',
	'constr_SET_OF.c',
	'constr_TYPE.c',
	'der_encoder.c',	# remove?
	'per_decoder.c',
	'per_encoder.c',
	'per_opentype.c',
	'per_support.c',
	'xer_decoder.c',	# remove?
	'xer_support.c',	# remove?

	# Mainline asn1c also needs the following files:
	#'asn_bit_data.c',
	#'asn_internal.c',
	#'asn_random_fill.c',	# remove?
	#'constr_CHOICE_oer.c',	# remove?
	#'constr_SEQUENCE_oer.c',# remove?
	#'constr_SET_OF_oer.c',	# remove?
	#'INTEGER_oer.c',	# remove?
	#'NativeEnumerated_oer.c',	# remove?
	#'NativeInteger_oer.c',	# remove?
	#'OCTET_STRING_oer.c',	# remove?
	#'oer_decoder.c',	# remove?
	#'oer_encoder.c',	# remove?
	#'oer_support.c',	# remove?
	#'OPEN_TYPE.c',
	#'OPEN_TYPE_oer.c',	# remove?
]

asn_headers = [
	'asn_application.h',	# This one has to be first in this array.
]

foreach type : asn_types
	asn_headers += [type + '.h']
	asn_sources += [type + '.c']
endforeach

asn1c = find_program('asn1c')
asn1c = find_program('/usr/bin/asn1c')
asn_generated = custom_target(
    'asn1c',
    output : asn_headers + asn_sources,
    input : schema_sources,
    command : [asn1c, '-Werror', '-gen-PER', '-fcompound-names', '@INPUT@'],
)

#add_project_arguments('-DEMIT_ASN_DEBUG=1', language : 'c')

rvcslib = static_library('rvcs',
	'rvcs.c',
	asn_generated,
	)

executable('example', 'example.c',
	link_with: rvcslib,
	sources: asn_generated[0]
	)

distribution = custom_target('distribution',
	output : 'rvcs.tar.gz',
	input : asn_generated.to_list() + rvcslib + ['rvcs.c', 'rvcs.h'],
	command : [ 'tar', 'zcf', '@OUTPUT@', '@INPUT@' ],
	build_by_default : true)
