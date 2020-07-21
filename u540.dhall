-- Import individual components of U540 and merge them

-- Modify imported config
let u54 = Config.removeDebug ./u54.dhall

-- Import from URL with hash
let e51 = https://riscv.org/config-struct/e51.dhall sha256:abcdef12345

let u540-uncore = ./u540-uncore.dhall

let u540 = Config.mergeConfigs [
  (Prelude.List.replicate Config.Description 4 u54),
  e51,
  Config.Description {
    harts = [] :: List HartConfig,
    uncore = u540-uncore
  }
]
