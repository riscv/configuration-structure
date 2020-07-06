let Shared = ./shared.dhall

let CategoryEntry
    : Type
    = < DebugCategory : List Shared.DebugEntry
      | DebugModuleCategory : Shared.DebugModuleEntry
      | IsaCategory : List Natural
      | PrivCategory : Shared.PrivEntry
      | FastIntCategory : Shared.FastIntEntry
      | TraceCategory : Shared.TraceEntry
      | NoCategory : {}
      >

let HartEntry
    : Type
    = { range : Shared.FlexibleRange, properties : List CategoryEntry }

let GenericEntry
    : Type
    = < Hart : HartEntry | Category : CategoryEntry >

let Description
    : Type
    = List GenericEntry

in  { Description = Description
    , GenericEntry = GenericEntry
    , Hart = GenericEntry.Hart
    , Category = GenericEntry.Category
    , DebugCategory = CategoryEntry.DebugCategory
    , DebugModuleCategory = CategoryEntry.DebugModuleCategory
    , NoCategory = CategoryEntry.NoCategory
    , IsaCategory = CategoryEntry.IsaCategory
    , PrivCategory = CategoryEntry.PrivCategory
    , FastIntCategory = CategoryEntry.FastIntCategory
    , TraceCategory = CategoryEntry.TraceCategory
    } // Shared
