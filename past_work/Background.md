# The Background

## Goals of Task Group

The task group will deliver:

. A specification for a machine-readable format for the configuration
structure. It’s intended to be easily accessible by M-mode software and
debuggers.
. A specification for a human-readable format for the configuration
structure. It’s intended to be used to talk about the system description
in this document as well as other documentation, to help designers write
a configuration structure, and to display the configuration structure to
people.
. A software tool that translates back and forth between the
machine-readable and human-readable format.
. A specification for a method to discover and access, from M-mode
software, the machine-readable configuration structures.

The configuration structure should be flexible enough that future task
groups won’t feel the need to create another structure used to describe
implementation parameters. Implementation parameters are details that a
RISC-V specification explicitly leaves up to an implementation. This
includes hart-specific details like the kinds of hardware triggers
supported, as well as details that are outside harts such as the
supported abstract debug commands.

The configuration structure can be specified/implemented/configured/extended at
different points in supply chain (IP supplier, SoC integrator,
manufacture/testing SKU fuse programming, board assembly, board programming).
The solution will accommodate hundreds of different types of RISC-V core,
integrated onto thousands of unique SoCs per year, assembled into tens of
thousands of unique boards per year.

## Considerations

Software in early bootup stages might want to parse some of the format.
This limits the complexity of the parsing process. For reference,
Table #tab:earlyresources[[tab:earlyresources]] lists the resources
available to software when the first updateable instruction
executes in some real-world platforms.

Table: available to software when the first updateable RISC-V
instruction executes in some real-world platforms.

Platform |RAM |Flash/ROM |Clock Speed
:---------|:----|:----------|:-----------
HiFive1 |16 kB |8 kB |14.4 MHz
HiFive1 Rev B |16 kB |4 MB |14.4 MHz
HiFive Unleashed |2 MB L2-LIM |32 MB |33.3 MHz
Intel Whitley |stackless code? |32MB |probably 3.7GHz
ET-SvcProc |1 MB |128 KB |10 MHz
ET-AppProcs |256 KB + cache |0 KB |100 MHz+
OpenTitan (small option) |64 KB |512 KB |
GigaDevice |32 KB |up to 128 KB |108 MHz
