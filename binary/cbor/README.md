# CBOR Example

The example script requires the Ruby [cddl](https://rubygems.org/gems/cddl)
and [cbor-diag](https://rubygems.org/gems/cbor-diag) tools, as well as
[libcbor](https://github.com/PJK/libcbor).

The binary size is 156 bytes.

Files:

- `spec.cddl` defines the format of the CBOR
- `instance.json` is an example instance, just like `../example.md`
- `instance.cbor` is the example converted to CBOR
- `parse-cbor.c` is an example program to parse the CBOR and dump some information
- `test.sh`
  - Generates a random JSON instance, in compliance with the spec
  - Validates the example instance
  - Converts the example instance to CBOR
  - Shows the converted CBOR as hex
  - Compiles the example parser
  - Runs the example parser

TODO:

- [x] Eliminate string keys from CBOR
- [ ] Represent e.g. bitfields more compactly

## CBOR
- Specs
  - [Overview](http://cbor.io/)
  - [CBOR](https://tools.ietf.org/html/rfc7049)
  - [CDDL](https://tools.ietf.org/html/rfc8610)
  - [Typed Arrays](https://cbor-wg.github.io/array-tags/draft-ietf-cbor-array-tags.html)
  - [IANA Tag Registry](https://www.iana.org/assignments/cbor-tags/cbor-tags.xhtml)
- Tools
  - [Validate well-formedness of CDDL](https://cddl.anweiss.tech/)
  - [CDDL Validator for CBOR and JSON](https://github.com/anweiss/cddl)
  - [Tooling for diagnostic notation](https://github.com/cabo/cbor-diag)
- Libraries
  - [CBOR Ruby](https://github.com/cabo/cbor-ruby)
  - [Rust codegen from CDDL](https://github.com/Emurgo/cddl-codegen)
- Users
  - [Webauthn](https://www.w3.org/TR/webauthn/)
