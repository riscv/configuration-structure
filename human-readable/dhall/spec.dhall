let Prelude = https://prelude.dhall-lang.org/v17.0.0/package.dhall

let Shared = ./shared.dhall

let Categories
    =
    { Type =
      { Debug       : List Shared.DebugEntry
      , DebugModule : Optional Shared.DebugModuleEntry
      , Isa         : List Shared.IsaEntry
      , Priv        : Optional Shared.PrivEntry
      , FastInt     : Optional Shared.FastIntEntry
      , Trace       : Optional Shared.TraceEntry
      }
    , default = { Debug       = [] : List Shared.DebugEntry
                , DebugModule = None Shared.DebugModuleEntry
                , Isa         = [] : List Shared.IsaEntry
                , Priv        = None Shared.PrivEntry
                , FastInt     = None Shared.FastIntEntry
                , Trace       = None Shared.TraceEntry
                }
    }

let HartConfig
    : Type
    = { hartid : Natural, config : Categories.Type }

let Description
    : Type
    = { harts : List HartConfig, uncore : Categories.Type }

let HartList
    : Type
    = < Single : HartConfig | Multiple : List HartConfig >

let hartListToList =
        λ(hartList : HartList)
      → merge
          { Single = λ(item : HartConfig) → [ item ]
          , Multiple = λ(items : List HartConfig) → items
          }
          hartList

let hartListFromList =
        λ(config : Categories.Type)
      → λ(hartIds : List Natural)
      → HartList.Multiple
          ( Prelude.List.map
              Natural
              HartConfig
              (λ(hartid : Natural) → { hartid = hartid, config = config })
              hartIds
          )

let enumerateFromTo =
        λ(start : Natural)
      → λ(end : Natural)
      → Prelude.List.filter
          Natural
          (λ(x : Natural) → Prelude.Natural.lessThanEqual start x)
          (Prelude.Natural.enumerate end)

let hartListFromRange =
        λ(config : Categories.Type)
      → λ(range : Shared.Range)
      -> hartListFromList config (enumerateFromTo range.start range.end)

let withHartRange
    : Shared.FlexibleRange → Categories.Type → HartList
    =   λ(range : Shared.FlexibleRange)
      → λ(config : Categories.Type)
      → merge
          { Single =
              λ(hartid : Natural) → HartList.Single { hartid = hartid, config = config }
          , Multiple = hartListFromList config
          , Range = hartListFromRange config
          }
          range

let flattenHarts : List HartList -> List HartConfig =
        λ(harts : List HartList)
      → Prelude.List.concatMap HartList HartConfig hartListToList harts

let mergeHarts : List HartList -> List HartConfig
    = \(x: List HartList) -> flattenHarts x
       --Prelude.List.fold
       --  HartConfig
       --  (flattenHarts x)
       --  (List HartConfig)
       --  (\(x: HartConfig) -> \(y: List HartConfig) -> y)

let mergeHartLists : List (List HartConfig) -> List HartConfig
    = \(hartLists : List (List HartConfig)) ->
       mergeHarts ( Prelude.List.map
                   (List HartConfig)
                    HartList
                    (\(list : List HartConfig) -> HartList.Multiple list)
                    hartLists
                   )

-- TODO: Actually merge uncores
let mergeUncore =
        \(uncores : List Categories.Type)
     -> Prelude.List.head Categories.Type uncores

-- TODO: Actually merge the configs
--let mergeConfigs : List Description -> Description =
--        \(configs : List Description)
--     -> { harts =  [] :: List HartConfig --mergeHartLists (Prelude.List.map Description (List HartConfig) (\(config : Description) -> config.harts) configs)
--        , uncore = Categories::{}
--        }

in    { Description = Description
      , Categories = Categories
      , withHartRange = withHartRange
      , mergeHarts = mergeHarts
      --, mergeConfigs = mergeConfigs
      }
    ⫽ Shared
