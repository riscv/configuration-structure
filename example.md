In general, all information is in a section named for the task group that
defines that information. Additionally there may be groups of harts, since
those cut across task groups. Hart groups may be at top level, or inside a
task group section.

I didn't include extensions like D, F, Zfinx. Those should be in their own
sections, defined by their own task groups.

This lists one giant structure, but the structure does not have to be all
together. A debug section could be in the Debug Module. A fast interrupt
section could be in the CLIC module. Each hart may contain its own
description, etc.

* hart: 0
    * category: Debug
        * trigger: 0--3
            * triple: LOW, HIGH, MASK
        * trigger: 4
            * tuple: VALUE0, MASK0
            * tuple: VALUE1, MASK1
* hart: 1--4
    * category: Debug
        * trigger: 0--1
            * triple: LOW0, HIGH0, MASK0
            * triple: LOW1, HIGH1, MASK1
* hart: 0, 2, 4
    * category: ISA
        * xlen: 32, 64
    * category: Privileged
        * privilege modes: U, M, S
        * SATP supported modes: sv48, sv64
* hart: 1, 3
    * category: ISA
        * xlen: 64
    * category: Privileged
        * privilege modes: M
        * ePMP supported: true
* category: Debug
    * debug module: 0
        * abstract commands
            * triple: LOW, HIGH, MASK
            * tuple: VALUE0, MASK0
            * tuple: VALUE1, MASK1
        * connected harts: 0--4
* category: Fast interrupt
    * clic: 0
        * connected harts: 1--4
    * hart: 1--4
        * Machine Mode Time Register Address: 0x...
        * Machine Mode Time Compare Register Address: 0x...
* category: Trace
    * Number of entries in the branch predictor: 8
    * Number of entries in the jump target cache: 2
    * Width of context bus: 32
* category: Physical Memory
    * tuple: LOW, HIGH addresses
        * cacheable
        * LR/SC support
        * alignment and size restrictions
        * mode restrictions
    * tuple: LOW, HIGH addresses
        * ...
