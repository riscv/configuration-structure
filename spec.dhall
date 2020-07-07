let Prelude = https://prelude.dhall-lang.org/v17.0.0/package.dhall

let Shared = ./shared.dhall

let CategoryEntry
    : Type
    = < DebugCategory : List Shared.DebugEntry
      | DebugModuleCategory : Shared.DebugModuleEntry
      | IsaCategory : List Shared.IsaEntry
      | PrivCategory : Shared.PrivEntry
      | FastIntCategory : Shared.FastIntEntry
      | TraceCategory : Shared.TraceEntry
      | NoCategory : {}
      >

let HartConfig
    : Type
    = { id : Natural, config : List CategoryEntry }

let Description
    : Type
    = { harts : List HartConfig, uncore : List CategoryEntry }

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
        λ(config : List CategoryEntry)
      → λ(hartIds : List Natural)
      → HartList.Multiple
          ( Prelude.List.map
              Natural
              HartConfig
              (λ(id : Natural) → { id = id, config = config })
              hartIds
          )

let enumerateFromTo =
        λ(start : Natural)
      → λ(end : Natural)
      → Prelude.List.filter
          Natural
          (λ(x : Natural) → Prelude.Natural.lessThanEqual start x)
          (Prelude.Natural.enumerate end)

let foo
    : Shared.Range → List Natural
    = λ(range : Shared.Range) → enumerateFromTo range.start range.end

let hartListFromRange =
        λ(config : List CategoryEntry)
      → λ(range : Shared.Range)
      -> hartListFromList config (enumerateFromTo range.start range.end)

let withHartRange
    : Shared.FlexibleRange → List CategoryEntry → HartList
    =   λ(range : Shared.FlexibleRange)
      → λ(config : List CategoryEntry)
      → merge
          { Single =
              λ(id : Natural) → HartList.Single { id = id, config = config }
          , Multiple = hartListFromList config
          , Range = hartListFromRange config
          }
          range

-- TODO: Actually merge the hart configuration, not just the lists
let mergeHarts : List HartList -> List HartConfig =
        λ(harts : List HartList)
      → Prelude.List.concatMap HartList HartConfig hartListToList harts

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
        \(uncores : List CategoryEntry)
     -> Prelude.List.head CategoryEntry uncores

-- TODO: Actually merge the configs
let mergeConfigs : List Description -> Description =
        \(configs : List Description)
     -> { harts =  mergeHartLists (Prelude.List.map Description (List HartConfig) (\(config : Description) -> config.harts) configs)
        , uncore = [] : List CategoryEntry
        }

in    { Description = Description
      , foo = foo
      , CategoryEntry = CategoryEntry
      , DebugCategory = CategoryEntry.DebugCategory
      , DebugModuleCategory = CategoryEntry.DebugModuleCategory
      , NoCategory = CategoryEntry.NoCategory
      , IsaCategory = CategoryEntry.IsaCategory
      , PrivCategory = CategoryEntry.PrivCategory
      , FastIntCategory = CategoryEntry.FastIntCategory
      , TraceCategory = CategoryEntry.TraceCategory
      , withHartRange = withHartRange
      , mergeHarts = mergeHarts
      --, mergeConfigs = mergeConfigs
      }
    ⫽ Shared
