#include <stdbool.h>

/*
 * What would example.yaml look like as a C struct? Here's a fairly
 * straightforward conversion.
 * Note that there is no length encoded in the arrays anywhere. A real solution
 * would have to address that problem.
 * 
 * Result:
 * riscv64-unknown-elf-gcc -c cstruct.c -Os -march=rv32g -mabi=ilp32
 *    text	   data	    bss	    dec	    hex	filename
 *       0	    492	      4	    496	    1f0	cstruct.o
 * riscv64-unknown-elf-gcc -c cstruct.c -Os
 *    text	   data	    bss	    dec	    hex	filename
 *       0	    736	      4	    740	    2e4	cstruct.o
 */

typedef struct {
    unsigned *single;
    unsigned *start;
    unsigned *end;
} flexible_range_t;

typedef struct {
    unsigned maskmax;
    bool hit;
    bool address_match;
    bool data_match;
    bool timing_before;
    bool timing_after;
    bool size_any;
    bool size_s8;
    bool size_s16;
    bool size_s32;
    bool size_s64;
    bool size_s80;
    bool size_s96;
    bool size_s112;
    bool size_s128;
    bool action_mmode;
    bool action_dmode;
    bool chain;
    bool match_equal;
    bool match_napot;
    bool match_greater_equal;
    bool match_less;
    bool match_low_mask;
    bool match_high_mask;
    bool match_not_equal;
    bool match_not_napot;
    bool match_not_low_mask;
    bool match_not_high_mask;
    bool m;
    bool s;
    bool u;
    bool execute;
    bool store;
    bool load;
} debug_trigger_mcontrol_t;

typedef struct {
    unsigned bitfield;
} debug_trigger_mcontrol_compact_t;

typedef struct {
    flexible_range_t index;
    debug_trigger_mcontrol_t *mcontrol;
    debug_trigger_mcontrol_compact_t *mcontrolCompact;
} debug_trigger_t;

typedef struct {
    debug_trigger_t *trigger;
} debug_t;

typedef struct {
    bool RISCV32;
    bool RISCV64;
    bool RISCV128;
} isa_t;

typedef struct {
    bool U, M, S;
} priv_modes_t;

typedef struct {
    bool SV32, SV39, SV48, SV57, SV64;
} priv_satps;

typedef struct {
    priv_modes_t modes;
    priv_satps satps;
    bool epmp;
} privileged_t;

typedef struct {
    unsigned m_time_register_address;
    unsigned m_time_compare_register_address;
} clic_t;

typedef struct {
    unsigned mModeTimeRegAddr;
    unsigned mModeTimeCompRegAddr;
} fastInt_t;

typedef struct {
    flexible_range_t hartid;
    debug_t debug;
    isa_t isa;
    privileged_t privileged;
    clic_t clic;
    fastInt_t fastInt;
} hart_t;

typedef struct {
    unsigned value, mask;
} tuple_t;

typedef struct {
    unsigned low, high, mask;
} triple_t;

typedef struct {
    tuple_t *tuple;
    triple_t *triple;
} possible_values_t;

typedef struct {
    flexible_range_t index;
    possible_values_t *abstract;
    flexible_range_t connected_harts;
} debug_module_t;

typedef struct {
    unsigned branchPredictorEntries;
    unsigned jumpTargetCacheEntries;
    unsigned contextBusWidth;
} trace_module_t;

typedef struct {
    unsigned start, length;
} range_t;

typedef struct {
    range_t *address;
    bool cacheable;
    bool lr_sc_supported;
} physical_memory_t;

typedef struct {
    hart_t *harts;
    debug_module_t *debug_module;
    trace_module_t *trace_module;
    physical_memory_t *physical_memory;
} configuration_t;

unsigned u0[] = {0};
unsigned u1[] = {1};
unsigned u3[] = {3};
unsigned u4[] = {4};
unsigned u_0_2_4[] = {0, 2, 4};
unsigned u_1_3[] = {1, 3};

debug_trigger_mcontrol_t mcontrol_no_s[] = {
    {
        .maskmax = 4,
        .size_any = true,
        .data_match = true,
        .timing_before = true,
        .match_equal = true,
        .match_napot = true,
        .match_greater_equal = true,
        .match_less = true,
        .action_mmode = true,
        .action_dmode = true,
        .m = true,
        .u = true,
        .execute = true,
        .store = true,
        .load = true
    }
};

debug_trigger_mcontrol_t mcontrol_s[] = {
    {
        .maskmax = 4,
        .size_any = true,
        .data_match = true,
        .timing_before = true,
        .match_equal = true,
        .match_napot = true,
        .match_greater_equal = true,
        .match_less = true,
        .action_mmode = true,
        .action_dmode = true,
        .m = true,
        .s = true,
        .u = true,
        .execute = true,
        .store = true,
        .load = true
    }
};

debug_trigger_t trig0[] = {
    {
        .index = { .start = u0, .end = u3},
        .mcontrol = mcontrol_no_s
    },
    {
        .index = { .single = u4 },
        .mcontrol = mcontrol_s
    }
};

debug_trigger_t trig1[] = {
    {
        .index = { .single = u4 },
        .mcontrol = mcontrol_s
    }
};

hart_t harts[] = {
    {
        .hartid = { .single = u0 },
        .debug = {
            .trigger = trig0
        }
    },
    {
        .hartid = { .start = u1, .end = u4 },
        .debug = {
            .trigger = trig1
        }
    },
    {
        .hartid = {
            .single = u_0_2_4
        },
        .isa = {
            .RISCV32 = true,
            .RISCV64 = true
        },
        .privileged = {
            .modes = {
                .M = true,
                .S = true,
                .U = true
            },
            .epmp = true,
            .satps = {
                .SV39 = true,
                .SV48 = true,
                .SV57 = true,
                .SV64 = true
            }
        }
    },
    {
        .hartid = {
            .single = u_1_3
        },
        .isa = {
            .RISCV64 = true
        },
        .privileged = {
            .modes = { .M = true },
            .epmp = true
        }
    },
    {
        .hartid = { .start = u1, .end = u4 },
        .fastInt = {
            .mModeTimeRegAddr = 4660,
            .mModeTimeCompRegAddr = 4660
        }
    }
};

triple_t trip = {
    .low = 4460,
    .high = 22136,
    .mask = 65280
};

tuple_t tup = {
    .value = 4460,
    .mask = 65280
};

possible_values_t abstracts[] = {
    {
        .triple = &trip
    },
    {
        .tuple = &tup
    },
    {
        .tuple = &tup
    }
};

debug_module_t debug_modules[] = {
    {
        .abstract = abstracts,
        .connected_harts = { .start = u0, .end = u4 }
    }
};

trace_module_t trace_modules[] = {
    {
        .branchPredictorEntries = 0,
        .jumpTargetCacheEntries = 0,
        .contextBusWidth = 32
    }
};

configuration_t configuration = {
    .harts = harts,
    .debug_module = debug_modules,
    .trace_module = trace_modules
};