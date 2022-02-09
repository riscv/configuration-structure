# RISC-V Configuration structure

## Layout of files here
- asn1tools/ contains files related to the python asn1tools library. This is
  where you want to start if you want to encode/decode examples on your PC.
- examples/ contains human-readable examples.
- schema/ contains the ASN.1 schema used that describes the configuration structure
  format.

## Build PDF

1. Update the submodules: `git submodule init && git submodule update`
2. Follow the [local build
instructions](https://github.com/riscv/docs-dev-guide/blob/main/local_build.md).
3. Run `make`

## Test Schema Changes

1. Run `pip3 install -r requirements.txt`
2. Run `make test`

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
under the [schema/](https://github.com/riscv/configuration-structure/tree/master/schema) directory should be all you need. For a simple example, look
at [zjpm](https://github.com/riscv/configuration-structure/blob/master/schema/zjpm-extension.asn). For a complex example look at [debug](https://github.com/riscv/configuration-structure/blob/master/schema/debug-extension.asn).
5. Following those examples, make a new file for your extension. Then edit
configuration-structure.asn, adding to the IMPORTS and adding appropriate
reference in the Configuration and Harts section. (You might only need one of
those.)
6. Run `make test` at the top level, and make sure no errors are reported.
7. Make a pull request against the github repo.

If you have any questions during this process, you might find the answers
in the spec. Otherwise, please contact us at
tech-config@lists.riscv.org.
