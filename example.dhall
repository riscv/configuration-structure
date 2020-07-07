let Config = ./spec.dhall

-- Aliases to make it easy to read and write
let withHartRange = Config.withHartRange
let Single = Config.FlexibleRange.Single
let Range = Config.FlexibleRange.Range
let Multiple = Config.FlexibleRange.Multiple
let TripleMatches = Config.TripleMatches
let TupleMatches = Config.TupleMatches
let DebugCategory = Config.DebugCategory
let DebugTrigger = Config.DebugTrigger
let IsaCategory = Config.IsaCategory
let IsaEntry = Config.IsaEntry
let PrivCategory = Config.PrivCategory
let Mode = Config.Mode
let Satp = Config.Satp
let FastIntCategory = Config.FastIntCategory
let TraceCategory = Config.TraceCategory
let DebugModuleCategory = Config.DebugModuleCategory

let LOW
    : Natural
    = 4660

let HIGH
    : Natural
    = 22136

let MASK
    : Natural
    = 65280

let VALUE = LOW

let descr : Config.Description = {
  harts = Config.mergeHarts [
    -- Define debug triggers for hart 0
      withHartRange (Single 0) [
        DebugCategory [
          DebugTrigger {
            triggers = Range { start = 0, end = 3 },
            matches = [ TripleMatches { low = LOW, high = HIGH, mask = MASK } ]
          },
          DebugTrigger {
            triggers = Single 4,
            matches = [
              TupleMatches { value = VALUE, mask = MASK },
              TupleMatches { value = VALUE, mask = MASK }
            ]
          }
        ]
      ],

    -- Define debug triggers for harts 1 through 4
    withHartRange (Range { start = 1, end = 4 }) [
      DebugCategory [
        DebugTrigger {
          triggers = Range { start = 0, end = 1 },
          matches = [
            TripleMatches { low = LOW, high = HIGH, mask = MASK },
            TripleMatches { low = LOW, high = HIGH, mask = MASK }
          ]
        }
      ]
    ],

    -- Define Isa and PrivCategory for even numbered harts
    withHartRange (Multiple [ 0, 2, 4 ]) [
      IsaCategory [ IsaEntry.RISCV_32,  IsaEntry.RISCV_64 ],
      PrivCategory {
        modes = [ Mode.M , Mode.S , Mode.U ],
        epmp = True,
        satp = [ Satp.Sv39 , Satp.Sv48 , Satp.Sv57 , Satp.Sv64 ]
      }
    ],

    -- Define Isa and PrivCategory for odd numbered harts
    withHartRange (Multiple [ 1, 3 ]) [
      IsaCategory [ IsaEntry.RISCV_64 ],
      PrivCategory {
        modes = [ Mode.M ],
        epmp = True,
        satp = [] : List Satp
      }
    ],

    -- Define fast interrupts for hart 1 and 4
    -- TODO: Define FastInt for Hart or Hart for FastInt?
    withHartRange (Multiple [ 1, 4 ]) [
      FastIntCategory { mModeTimeRegAddr = 4660, mModeTimeCompRegAddr = 4660 }
    ]
  ],

  -- Define uncore config
  uncore = [
    -- Define core-wide tracing
    TraceCategory {
      branchPredictorEntries = 0,
      jumpTargetCacheEntries = 0,
      contextBusWidth = 32
    },

    -- Define core-wide debug module
    DebugModuleCategory {
      abstractCommands = [
        TripleMatches { low = LOW, high = HIGH, mask = MASK },
        TupleMatches { value = VALUE, mask = MASK },
        TupleMatches { value = VALUE, mask = MASK }
      ],
      connectedHarts = Range { start = 0, end = 4 }
    }
  ]
}

in  descr
