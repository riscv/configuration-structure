# Task Group Charter

The Configuration Structure Task Group will:
* Specify syntax and semantics for a static data structure that can accommodate
  all implementation parameters of RISC-V standards: the configuration
  structure. There will be two configuration structure formats: a
  machine-readable format intended to be embedded in hardware, and a
  human-readable format intended for people to work with directly.
* Specify how M-mode software can discover and access any present
  machine-readable configuration structures.
* Provide a tool that can translate between the machine-readable and
  human-readable formats.

Implementation parameters are details that a RISC-V specification explicitly
leaves up to an implementation. This includes hart-specific details like the
kinds of hardware triggers supported, as well as details that are outside
harts such as the supported abstract debug commands.

The configuration structure should:
* be flexible enough that future task groups won’t feel the need to
  create another structure used to describe implementation parameters.
* be relatively easy to translate into Devicetree

The configuration structure is intended to be used:
* to describe RISC-V hardware profiles
* by firmware and BIOSes during the boot process
* by debuggers
* by a tool chain to build software tailored to a configuration profile
