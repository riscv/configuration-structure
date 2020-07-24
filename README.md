# RISC-V Configuration structure

## Work-In-Progress Examples
Currently the task group is in the process of deciding how to represent the
configuration structure both in a human-readable format, as well as a binary
format. The former will be written by the chip designer and the latter will be
embedded in the chip to be read by software running on it.

Proposed examples are (as present in the filesystem):

- example.md (The reference example)
- binary
  - custom-encoding
  - protobuf
- human-readable
  - scala
  - yaml (not full compliant with the example.md)
