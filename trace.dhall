let TraceEntry
    : Type
    = { branchPredictorEntries : Natural
      , jumpTargetCacheEntries : Natural
      , contextBusWidth : Natural
      }
in TraceEntry
