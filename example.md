In general, all information is in a section named for the task group that
defines that information. Additionally there may be groups of harts, since
those cut across task groups.

This lists one giant structure, but the structure does not have to be all
together. A debug section could be in the Debug Module. A fast interrupt
section could be in the CLIC module. Each hart may contain its own
description, etc.

* hart: 0
    * category: Debug
        * trigger: 0--3
            * triple: 0x1234, 0x5678, 0xff00
        * trigger: 4
            * tuple: 0x1234, 0xff00
            * tuple: 0x1234, 0xff00
* hart: 1--4
    * category: Debug
        * trigger: 0--1
            * triple: 0x1234, 0x5678, 0xff00
            * triple: 0x1234, 0x5678, 0xff00
    * clic:
        * Machine Mode Time Register Address: 0x10004220
        * Machine Mode Time Compare Register Address: 0x10004228
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
* Debug Module: 0
    * abstract commands
        * triple: 0x1234, 0x5678, 0xff00
        * tuple: 0x1234, 0xff00
        * tuple: 0x1234, 0xff00
    * connected harts: 0--4
* fast interrupt module: 0
    * connected harts: 1--4
* trace module:
    * Number of entries in the branch predictor: 8
    * Number of entries in the jump target cache: 2
    * Width of context bus: 32
* Physical Memory:
    * tuple: 0x80000000 -- 0x81ffffff
        * cacheable: true
        * LR/SC support: true
