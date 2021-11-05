# RISC-V Configuration structure

## Layout of files here
- asn1c/ contains files related to the C asn1c tool. This is where you want to
  start if you're interested in what a C decoder would look like.
- asn1tools/ contains files related to the python asn1tools library. This is
  where you want to start if you want to encode/decode examples on your PC.
- examples/ contains human-readable examples.
- schema.asn is the ASN.1 schema used that contains the configuration structure
  format.

## Extend the Schema

This section describes as concisely as possible how to add information for your
extension to the configuration structure. Many details will be omitted, but for
simple use cases that should be fine.

1. Make a list of any implementation decision your specification explicitly
allows that affects what software can do.
2. If the list is long, divide the decisions into primary, secondary, and
complete decisions. Primary information is only discoverable by reading the
configuration structure, or by running a significant amount of code. Secondary
information is discoverable but not straightforward (e.g. WARL register).
Anything else goes in the complete section.
3. Get ready to make a pull request to
https://github.com/riscv/configuration-structure, e.g. by cloning the repo.
4. Write an ASN.1 description of this information. ASN.1 allows for all kinds of
complexity, but for your purposes simply reading one of extension .asn files
under the schema/ directory should be all you need. For a simple example, look
at zjpm. For a complex example look at debug.
5. Following those examples, make a new file for your extension. Then edit
configuration-structure.asn, adding to the IMPORTS and adding appropriate
reference in the Configuration and Harts section. (You might only need one of
those.)
6. Run `make test` at the top level, and make sure no errors are reported.
7. Make a pull request against the github repo.

If you have any questions during this process, you might find the answers
in the spec. Otherwise, please contact us at
tech-config@lists.riscv.org.

## Test changes

`make test` or `./asn1tools/rvcs.py test examples/*.jer`

Several python libraries are required to run the tests.
See the `requirements.txt` file, or run `pip3 install -r requirements.txt`

to install them all automatically.

## Open Questions

### Socketed System

How to discover information about each hart in a socketed system is an open
question. Presumably each chip contains a structure it describes itself. It
would be nice to have some definition of a motherboard configuration structure,
which could contain pointers to the configuration structures in each chip.  That
feels outside the scope of what the RISC-V Foundation defines, however.

### Schema Naming and the Structure Organization

The best naming rule and organization of the schema.
