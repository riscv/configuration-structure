let Config = ./manual-dedup-spec.dhall

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

let descr
    : Config.Description
      -- Define debug triggers for hart 0
    = [ Config.Hart
          { range = Config.FlexibleRange.Single 0
          , properties =
            [ Config.DebugCategory
                [ Config.DebugTrigger
                    { triggers =
                        Config.FlexibleRange.Range { start = 0, end = 3 }
                    , matches =
                      [ Config.TripleMatches
                          { low = LOW, high = HIGH, mask = MASK }
                      ]
                    }
                , Config.DebugTrigger
                    { triggers = Config.FlexibleRange.Single 4
                    , matches =
                      [ Config.TupleMatches { value = VALUE, mask = MASK }
                      , Config.TupleMatches { value = VALUE, mask = MASK }
                      ]
                    }
                ]
            ]
          }
      -- Define debug triggers for harts 1 through 4
      , Config.Hart
          { range = Config.FlexibleRange.Range { start = 1, end = 4 }
          , properties =
            [ Config.DebugCategory
                [ Config.DebugTrigger
                    { triggers =
                        Config.FlexibleRange.Range { start = 0, end = 1 }
                    , matches =
                      [ Config.TripleMatches
                          { low = LOW, high = HIGH, mask = MASK }
                      , Config.TripleMatches
                          { low = LOW, high = HIGH, mask = MASK }
                      ]
                    }
                ]
            ]
          }
      -- Define Isa and PrivCategory for even numbered harts
      , Config.Hart
          { range = Config.FlexibleRange.Multiple [ 0, 2, 4 ]
          , properties =
            [ Config.IsaCategory [ Config.IsaEntry.RISCV_32, Config.IsaEntry.RISCV_64 ]
            , Config.PrivCategory
                { modes =
                  [ Config.Mode.M, Config.Mode.S, Config.Mode.U ]
                , epmp = True
                , satp =
                  [ Config.Satp.Sv39
                  , Config.Satp.Sv48
                  , Config.Satp.Sv57
                  , Config.Satp.Sv64
                  ]
                }
            ]
          }
      -- Define Isa and PrivCategory for odd numbered harts
      , Config.Hart
          { range = Config.FlexibleRange.Multiple [ 1, 3 ]
          , properties =
            [ Config.IsaCategory [ Config.IsaEntry.RISCV_64 ]
            , Config.PrivCategory
                { modes = [ Config.Mode.M ]
                , epmp = True
                , satp = [] : List Config.Satp
                }
            ]
          }
       -- Define fast interrupts for hart 1 and 4
      , Config.Hart
          { range = Config.FlexibleRange.Multiple [ 1, 4 ]
          , properties =
            [ Config.FastIntCategory
                { mModeTimeRegAddr = 4660, mModeTimeCompRegAddr = 4660 }
            ]
          }
      , Config.Category
          ( Config.TraceCategory
              { branchPredictorEntries = 0
              , jumpTargetCacheEntries = 0
              , contextBusWidth = 32
              }
          )
      , Config.Category
          ( Config.DebugModuleCategory
              { abstractCommands
                = [ Config.TripleMatches
                              { low = LOW, high = HIGH, mask = MASK }
                  , Config.TupleMatches { value = VALUE, mask = MASK }
                  , Config.TupleMatches { value = VALUE, mask = MASK }
                  ]
              , connectedHarts = Config.FlexibleRange.Range { start = 0, end = 4 } }
                )
      ]

in  descr
