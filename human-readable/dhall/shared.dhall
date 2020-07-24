let BitMask
    : Type
    = Natural

let Tuple
    : Type
    = { value : Natural, mask : BitMask }

let Triple
    : Type
    = { low : Natural, high : Natural, mask : BitMask }

let Range
    : Type
    = { start : Natural, end : Natural }

let FlexibleRange
    : Type
    = < Single : Natural | Multiple : List Natural | Range : Range >

let Matches = < TupleMatches : Tuple | TripleMatches : Triple >

let TriggerType
    : Type
    = { triggers : FlexibleRange, matches : List Matches }

let IsaEntry : Type = < RISCV_32 | RISCV_64 >

let DebugEntry
    : Type
    = < DebugTrigger : TriggerType >

let Mode
    : Type
    = < M | S | U >

let Satp
    : Type
    = < Sv39 | Sv48 | Sv57 | Sv64 >

let DebugModuleEntry
    : Type
    = { abstractCommands : List Matches, connectedHarts : FlexibleRange }

let PrivEntry
    : Type
    = { modes : List Mode, epmp : Bool, satp : List Satp }

in { FlexibleRange = FlexibleRange
    , Range = Range
    , Mode = Mode
    , Satp = Satp
    , TupleMatches = Matches.TupleMatches
    , TripleMatches = Matches.TripleMatches
    , DebugTrigger = DebugEntry.DebugTrigger
    , DebugEntry = DebugEntry
    , DebugModuleEntry = DebugModuleEntry
    , IsaEntry = IsaEntry
    , PrivEntry = PrivEntry
    , FastIntEntry = ./fast-int.dhall
    , TraceEntry = ./trace.dhall
   }
