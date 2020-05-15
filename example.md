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

* Hart 0
    * Debug
        * Triggers 0--3
            * triple of LOW, HIGH, MASK
        * Trigger 4
            * tuple of VALUE0, MASK0
            * tuple of VALUE1, MASK1
* hart 1--4
    * Debug
        * Triggers 0--1:
            * triple of LOW0, HIGH0, MASK0
            * triple of LOW1, HIGH1, MASK1
* hart 0, 2, 4:
    * ISA
        * XLEN
            * 32 or 64
    * Privilege
        * modes
            * U, M, S
        * SATS supported modes
            * Sv48, Sv64
* hart 1, 3
    * ISA
        * XLEN
            * 64
    * Privilege
        * modes
            * M
        * ePMP supported
* Debug
    * Debug Module 0
        * abstract commands
            * triple of LOW, HIGH, MASK
            * tuple of VALUE0, MASK0
            * tuple of VALUE1, MASK1
        * connected harts:
            * 0--4
* Fast interrupt
    * CLIC 0
        * connected harts:
            * 1--4
    * hart 1--4:
        * Machine Mode Time Register Address: 0x...
        * Machine Mode Time Compare Register Address: 0x...
* Trace
    * Number of entries in the branch predictor: 8
    * Number of entries in the jump target cache: 2
    * Width of context bus: 32
* Physical Memory
    * tuple of LOW, HIGH addresses
        * cacheable
        * LR/SC support
    * tuple of LOW, HIGH addresses
        * ...
