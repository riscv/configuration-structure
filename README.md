# RISC-V Configuration structure

## Layout of files here
- asn1c/ contains files related to the C asn1c tool. This is where you want to
  start if you're interested in what a C decoder would look like.
- asn1tools/ contains files related to the python asn1tools library. This is
  where you want to start if you want to encode/decode examples on your PC.
- examples/ contains human-readable examples.
- schema.asn is the ASN.1 schema used that contains the configuration structure
  format.

## Test changes

`make test` or `./asn1tools/rvcs.py test examples/*.jer`

Several python libraries are required to run the tests.
See the `requirements.txt` file, or run `pip3 install -r requirements.txt`

to install them all automatically.
