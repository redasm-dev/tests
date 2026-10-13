#include "rdtest.h"
#include "rdtest_helpers.h"
#include <redasm/redasm.h>

static int test_0x00_linux(void) {
    static const RDTestName NAMES[] = {
        {0x80482F8, "_init"},
        {0x8048320, "imp___libc_start_main"},
        {0x8048330, "imp_scanf"},
        {0x8048340, "imp_printf"},
        {0x8048350, "imp_strcmp"},
        {0x8048360, "_start"},
        {0x8048414, "main"},
        {0x80484A0, "__libc_csu_init"},
        {0x8048510, "__libc_csu_fini"},
        {0x8048515, "__i686.get_pc_thunk.bx"},
        {0x8048544, "_fini"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x8048568, .name = "char", .count = 25},
        {0x8048581, .name = "char", .count = 11},
        {0x804858c, .name = "char", .count = 3},
        {0x804858f, .name = "char", .count = 7},
        {0x8048596, .name = "char", .count = 19},
        {0x80485a9, .name = "char", .count = 16},
        {0x804A000, .name = "u32"},
        {0x804A004, .name = "u32"},
        {0x804A008, .name = "u32"},
        {0x804A00C, .name = "u32"},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x80482F8, 0xE73440F8}, {0x8048320, 0xA4F640AE},
        {0x8048330, 0xD7D6B7C3}, {0x8048340, 0x378501E9},
        {0x8048350, 0xFE0F64B5}, {0x8048360, 0x49A63832},
        {0x8048384, 0x992CC90A}, {0x804838D, 0x57E264B5},
        {0x80483B0, 0xEFE6ACC0}, {0x80483E0, 0x75BF09E9},
        {0x8048414, 0x4B0F42A5}, {0x80484A0, 0xD6F14FB},
        {0x8048510, 0xE37DBFBB}, {0x8048515, 0x641C9B37},
        {0x8048520, 0xE90A4FD7}, {0x8048544, 0x941D23B9},
        {0x804854D, 0x15451780}, {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x8048414, {.address = 0x8048377, .type = RD_DR_ADDRESS}},
        {0x8048568, {.address = 0x8048430, .type = RD_DR_ADDRESS}},
        {0x8048581, {.address = 0x804843c, .type = RD_DR_ADDRESS}},
        {0x804858c, {.address = 0x804844f, .type = RD_DR_ADDRESS}},
        {0x804858c, {.address = 0x804844f, .type = RD_DR_ADDRESS}},
        {0x804858f, {.address = 0x804845e, .type = RD_DR_ADDRESS}},
        {0x804858f, {.address = 0x804845e, .type = RD_DR_ADDRESS}},
        {0x8048596, {.address = 0x8048472, .type = RD_DR_ADDRESS}},
        {0x80485a9, {.address = 0x8048480, .type = RD_DR_ADDRESS}},
        {0x804a000, {.address = 0x8048320, .type = RD_DR_READ}},
        {0x804a004, {.address = 0x8048330, .type = RD_DR_READ}},
        {0x804a008, {.address = 0x8048340, .type = RD_DR_READ}},
        {0x804a00c, {.address = 0x8048350, .type = RD_DR_READ}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x80482F8, .name = "_init"},
        {.kind = RD_EXT_EXPORTED, .address = 0x8048360, .name = "_start"},
        {.kind = RD_EXT_EXPORTED, .address = 0x8048414, .name = "main"},
        {.kind = RD_EXT_EXPORTED, .address = 0x80484A0, .name = "__libc_csu_init"},
        {.kind = RD_EXT_EXPORTED, .address = 0x8048510, .name = "__libc_csu_fini"},
        {.kind = RD_EXT_EXPORTED, .address = 0x8048515, .name = "__i686.get_pc_thunk.bx"},
        {.kind = RD_EXT_EXPORTED, .address = 0x8048544, .name = "_fini"},
        {.kind = RD_EXT_EXPORTED, .address = 0x8048560, .name = "_fp_hw"},
        {.kind = RD_EXT_EXPORTED, .address = 0x8048564, .name = "_IO_stdin_used"},
        {.kind = RD_EXT_EXPORTED, .address = 0x804A014, .name = "__dso_handle"},
        {.kind = RD_EXT_IMPORTED, .address = 0x8049FF0, .name = "__gmon_start__"},
        {.kind = RD_EXT_IMPORTED, .address = 0x804A000, .name = "__libc_start_main"},
        {.kind = RD_EXT_IMPORTED, .address = 0x804A004, .name = "scanf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x804A008, .name = "printf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x804A00C, .name = "strcmp"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "ioli/linux/crackme0x00",
        .loader_id = "elf",
        .processor_id = "x86_32",
        .entry_point =
            {
                .value = 0x08048360,
                .has_value = true,
                .no_ret = true,
            },
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
        .externals = EXTERNALS,
        .n_functions = 15,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_0x00_pocketpc(void) {
    static const RDTestName NAMES[] = {
        {0x11000, "WinMainCRTStartup"},
        {0x11050, "main"},
        {0x110DC, "_pei386_runtime_relocator"},
        {0x11120, "__atexit_first"},
        {0x11130, "closeall_streams"},
        {0x1113C, "_exit"},
        {0x11154, "_c_exit"},
        {0x11160, "__dllonexit"},
        {0x111D4, "_onexit"},
        {0x111F0, "atexit"},
        {0x11218, "__dll_exit"},
        {0x11290, "_cexit"},
        {0x112A0, "exit"},
        {0x112B8, "__atexit_init"},
        {0x112F4, "__do_global_dtors"},
        {0x11338, "__do_global_ctors"},
        {0x113A0, "__gccmain"},
        {0x113C8, "WinMain"},
        {0x11620, "imp__fpreset"},
        {0x1162C, "imp_TerminateProcess"},
        {0x11638, "imp_puts"},
        {0x11644, "imp_strcmp"},
        {0x11650, "imp_scanf"},
        {0x1165C, "imp_printf"},
        {0x11668, "imp_malloc"},
        {0x11674, "imp_free"},
        {0x11680, "imp_fflush"},
        {0x1168C, "imp_realloc"},
        {0x11698, "imp__fcloseall"},
        {0x116A4, "imp_strspn"},
        {0x116B0, "imp_strchr"},
        {0x116BC, "imp_strcspn"},
        {0x116C8, "imp_GetModuleFileNameW"},
        {0x116D4, "imp_wcslen"},
        {0x116E0, "imp_strlen"},
        {0x116EC, "imp_wcstombs"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x13000, .name = "char", .count = 24},
        {0x13018, .name = "char", .count = 11},
        {0x13024, .name = "char", .count = 3},
        {0x13028, .name = "char", .count = 7},
        {0x13030, .name = "char", .count = 18},
        {0x13044, .name = "char", .count = 15},
        {0x110c4, .name = "u32", .mod = RD_TYPE_PTR},
        {0x110c8, .name = "u32", .mod = RD_TYPE_PTR},
        {0x110cc, .name = "u32", .mod = RD_TYPE_PTR},
        {0x110d0, .name = "u32", .mod = RD_TYPE_PTR},
        {0x110d4, .name = "u32", .mod = RD_TYPE_PTR},
        {0x110d8, .name = "u32", .mod = RD_TYPE_PTR},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x11000, 0x18286789},
        {0x11050, 0xC64F76EB},
        {0x110DC, 0x4BCAE00A},
        {0x11120, 0xCEBBD2A9},
        {0x11130, 0xF398AB52},
        {0x1113C, 0x68015B0E},
        {0x11154, 0xCB67D69C},
        {0x11160, 0x4986A68E},
        {0x111D4, 0x75D1FF45},
        {0x111F0, 0x3AD13781},
        {0x11218, 0xA3C9A5F6},
        {0x11290, 0xACF9A864},
        {0x112A0, 0xF9F75F78},
        {0x112B8, 0x522975C0},
        {0x112F4, 0x1D50FE0},
        {0x11338, 0x23864C76},
        {0x113A0, 0xAF3366A},
        {0x113C8, 0x1035B9EB},
        {0x11620, 0x977EC34A},
        {0x1162C, 0x94C053D2},
        {0x11638, 0xFD9D1911},
        {0x11644, 0x6277EEB2},
        {0x11650, 0x2D3914AE},
        {0x1165C, 0xABDF387},
        {0x11668, 0xEFA1461D},
        {0x11674, 0xDE9BD127},
        {0x11680, 0x3E3F970A},
        {0x1168C, 0x39F56AA3},
        {0x11698, 0x53DD59E1},
        {0x116A4, 0xC5895DF7},
        {0x116B0, 0x488E73A9},
        {0x116BC, 0x2F9526D4},
        {0x116C8, 0x32AFBFFF},
        {0x116D4, 0xCC91BE90},
        {0x116E0, 0x9981A445},
        {0x116EC, 0xBA0B5B3A},
        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x13000, {.address = 0x110c4, .type = RD_DR_ADDRESS}},
        {0x13018, {.address = 0x110c8, .type = RD_DR_ADDRESS}},
        {0x13024, {.address = 0x110cc, .type = RD_DR_ADDRESS}},
        {0x13028, {.address = 0x110d0, .type = RD_DR_ADDRESS}},
        {0x13030, {.address = 0x110d4, .type = RD_DR_ADDRESS}},
        {0x13044, {.address = 0x110d8, .type = RD_DR_ADDRESS}},
        {0x13054, {.address = 0x11618, .type = RD_DR_ADDRESS}},
        {0x110c4, {.address = 0x11064, .type = RD_DR_READ}},
        {0x110c8, {.address = 0x1106c, .type = RD_DR_READ}},
        {0x110cc, {.address = 0x11078, .type = RD_DR_READ}},
        {0x110d0, {.address = 0x1108c, .type = RD_DR_READ}},
        {0x110d4, {.address = 0x110a0, .type = RD_DR_READ}},
        {0x110d8, {.address = 0x110ac, .type = RD_DR_READ}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x11000, .name = "WinMainCRTStartup"},
        {.kind = RD_EXT_IMPORTED, .address = 0x1507C, .module = "COREDLL", .name = "GetModuleFileNameW"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15080, .module = "COREDLL", .name = "TerminateProcess"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15084, .module = "COREDLL", .name = "_fcloseall"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15088, .module = "COREDLL", .name = "_fpreset"},
        {.kind = RD_EXT_IMPORTED, .address = 0x1508C, .module = "COREDLL", .name = "fflush"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15090, .module = "COREDLL", .name = "free"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15094, .module = "COREDLL", .name = "malloc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15098, .module = "COREDLL", .name = "printf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x1509C, .module = "COREDLL", .name = "puts"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150A0, .module = "COREDLL", .name = "realloc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150A4, .module = "COREDLL", .name = "scanf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150A8, .module = "COREDLL", .name = "strchr"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150AC, .module = "COREDLL", .name = "strcmp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150B0, .module = "COREDLL", .name = "strcspn"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150B4, .module = "COREDLL", .name = "strlen"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150B8, .module = "COREDLL", .name = "strspn"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150BC, .module = "COREDLL", .name = "wcslen"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150C0, .module = "COREDLL", .name = "wcstombs"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "ioli/pocketpc/crackme0x00.arm.exe",
        .loader_id = "win_pe",
        .processor_id = "arm32_le",
        .entry_point =
            {
                .value = 0x11000,
                .has_value = true,
                .no_ret = true,
            },
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
        .externals = EXTERNALS,
        .n_functions = 36,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_0x00_pocketpc_stripped(void) {
    static const RDTestName NAMES[] = {
        {0x11000, "win_pe_entry_point_11000"},
        {0x11620, "imp__fpreset"},
        {0x1162C, "imp_TerminateProcess"},
        {0x11638, "imp_puts"},
        {0x11644, "imp_strcmp"},
        {0x11650, "imp_scanf"},
        {0x1165C, "imp_printf"},
        {0x11668, "imp_malloc"},
        {0x11674, "imp_free"},
        {0x11680, "imp_fflush"},
        {0x1168C, "imp_realloc"},
        {0x11698, "imp__fcloseall"},
        {0x116A4, "imp_strspn"},
        {0x116B0, "imp_strchr"},
        {0x116BC, "imp_strcspn"},
        {0x116C8, "imp_GetModuleFileNameW"},
        {0x116D4, "imp_wcslen"},
        {0x116E0, "imp_strlen"},
        {0x116EC, "imp_wcstombs"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x13000, .name = "char", .count = 24},
        {0x13018, .name = "char", .count = 11},
        {0x13024, .name = "char", .count = 3},
        {0x13028, .name = "char", .count = 7},
        {0x13030, .name = "char", .count = 18},
        {0x13044, .name = "char", .count = 15},
        {0x110c4, .name = "u32", .mod = RD_TYPE_PTR},
        {0x110c8, .name = "u32", .mod = RD_TYPE_PTR},
        {0x110cc, .name = "u32", .mod = RD_TYPE_PTR},
        {0x110d0, .name = "u32", .mod = RD_TYPE_PTR},
        {0x110d4, .name = "u32", .mod = RD_TYPE_PTR},
        {0x110d8, .name = "u32", .mod = RD_TYPE_PTR},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x11000, 0x18286789},
        {0x11050, 0xC64F76EB},
        {0x110DC, 0x4BCAE00A},
        {0x11130, 0xF398AB52},
        {0x11160, 0x4986A68E},
        {0x111F0, 0x3AD13781},
        {0x11218, 0xA3C9A5F6},
        {0x11290, 0xACF9A864},
        {0x112B8, 0x522975C0},
        {0x11338, 0x23864C76},
        {0x113A0, 0xAF3366A},
        {0x113C8, 0x1035B9EB},
        {0x11620, 0x977EC34A},
        {0x1162C, 0x94C053D2},
        {0x11638, 0xFD9D1911},
        {0x11644, 0x6277EEB2},
        {0x11650, 0x2D3914AE},
        {0x1165C, 0xABDF387},
        {0x11668, 0xEFA1461D},
        {0x11674, 0xDE9BD127},
        {0x11680, 0x3E3F970A},
        {0x1168C, 0x39F56AA3},
        {0x11698, 0x53DD59E1},
        {0x116A4, 0xC5895DF7},
        {0x116B0, 0x488E73A9},
        {0x116BC, 0x2F9526D4},
        {0x116C8, 0x32AFBFFF},
        {0x116D4, 0xCC91BE90},
        {0x116E0, 0x9981A445},
        {0x116EC, 0xBA0B5B3A},
        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x13000, {.address = 0x110c4, .type = RD_DR_ADDRESS}},
        {0x13018, {.address = 0x110c8, .type = RD_DR_ADDRESS}},
        {0x13024, {.address = 0x110cc, .type = RD_DR_ADDRESS}},
        {0x13028, {.address = 0x110d0, .type = RD_DR_ADDRESS}},
        {0x13030, {.address = 0x110d4, .type = RD_DR_ADDRESS}},
        {0x13044, {.address = 0x110d8, .type = RD_DR_ADDRESS}},
        {0x13054, {.address = 0x11618, .type = RD_DR_ADDRESS}},
        {0x110c4, {.address = 0x11064, .type = RD_DR_READ}},
        {0x110c8, {.address = 0x1106c, .type = RD_DR_READ}},
        {0x110cc, {.address = 0x11078, .type = RD_DR_READ}},
        {0x110d0, {.address = 0x1108c, .type = RD_DR_READ}},
        {0x110d4, {.address = 0x110a0, .type = RD_DR_READ}},
        {0x110d8, {.address = 0x110ac, .type = RD_DR_READ}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x11000},
        {.kind = RD_EXT_IMPORTED, .address = 0x1507C, .module = "COREDLL", .name = "GetModuleFileNameW"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15080, .module = "COREDLL", .name = "TerminateProcess"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15084, .module = "COREDLL", .name = "_fcloseall"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15088, .module = "COREDLL", .name = "_fpreset"},
        {.kind = RD_EXT_IMPORTED, .address = 0x1508C, .module = "COREDLL", .name = "fflush"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15090, .module = "COREDLL", .name = "free"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15094, .module = "COREDLL", .name = "malloc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15098, .module = "COREDLL", .name = "printf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x1509C, .module = "COREDLL", .name = "puts"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150A0, .module = "COREDLL", .name = "realloc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150A4, .module = "COREDLL", .name = "scanf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150A8, .module = "COREDLL", .name = "strchr"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150AC, .module = "COREDLL", .name = "strcmp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150B0, .module = "COREDLL", .name = "strcspn"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150B4, .module = "COREDLL", .name = "strlen"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150B8, .module = "COREDLL", .name = "strspn"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150BC, .module = "COREDLL", .name = "wcslen"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150C0, .module = "COREDLL", .name = "wcstombs"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "ioli/pocketpc/crackme0x00_stripped.arm.exe",
        .loader_id = "win_pe",
        .processor_id = "arm32_le",
        .entry_point =
            {
                .value = 0x11000,
                .has_value = true,
                .no_ret = true,
            },
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
        .externals = EXTERNALS,
        .n_functions = 30,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_0x00_win32(void) {
    static const RDTestName NAMES[] = {
        {0x401000, "__gnu_exception_handler@4"},
        {0x401140, "___mingw_CRTStartup"},
        {0x401260, "_mainCRTStartup"},
        {0x401280, "_WinMainCRTStartup"},
        {0x4012A0, "_atexit"},
        {0x4012B0, "__onexit"},
        {0x4012C0, "___do_frame_init"},
        {0x4012F0, "___do_frame_fini"},
        {0x401310, "_main"},
        {0x4013A0, "__pei386_runtime_relocator"},
        {0x4013D0, "_fpreset"},
        {0x4013E0, "___do_global_dtors"},
        {0x401410, "___do_global_ctors"},
        {0x401470, "___main"},
        {0x401490, "_size_of_encoded_value"},
        {0x4014E0, "_read_uleb128"},
        {0x401530, "_read_sleb128"},
        {0x4015A0, "_read_encoded_value_with_base"},
        {0x401670, "_init_object_mutex"},
        {0x4016C0, "_init_object_mutex_once"},
        {0x401750, "___register_frame_info_bases"},
        {0x401840, "___register_frame_info"},
        {0x401870, "___register_frame"},
        {0x4018B0, "___register_frame_info_table_bases"},
        {0x4019B0, "___register_frame_info_table"},
        {0x4019E0, "___register_frame_table"},
        {0x401A10, "___deregister_frame_info_bases"},
        {0x401BA0, "j____deregister_frame_info_bases"},
        {0x401BB0, "___deregister_frame"},
        {0x401BE0, "_base_from_object"},
        {0x401C40, "_get_cie_encoding"},
        {0x401CD0, "_fde_unencoded_compare"},
        {0x401CF0, "_fde_single_encoding_compare"},
        {0x401D70, "_fde_mixed_encoding_compare"},
        {0x401E00, "_frame_downheap"},
        {0x401E90, "_frame_heapsort"},
        {0x401F00, "_classify_object_over_fdes"},
        {0x402020, "_add_fdes"},
        {0x402120, "_linear_search_fdes"},
        {0x402240, "_search_object"},
        {0x4027C0, "__Unwind_Find_FDE"},
        {0x4029A0, "___w32_sharedptr_default_unexpected"},
        {0x4029B0, "___w32_sharedptr_get"},
        {0x402A50, "___w32_sharedptr_initialize"},
        {0x402CF0, "imp___p__fmode"},
        {0x402D00, "imp___getmainargs"},
        {0x402D10, "imp_strcmp"},
        {0x402D20, "imp_scanf"},
        {0x402D30, "imp_printf"},
        {0x402D40, "imp_strlen"},
        {0x402D50, "imp_free"},
        {0x402D60, "imp_malloc"},
        {0x402D70, "imp_abort"},
        {0x402D80, "imp__assert"},
        {0x402D90, "imp_ExitProcess"},
        {0x402DA0, "imp_SetUnhandledExceptionFilter"},
        {0x402DB0, "imp_ReleaseSemaphore"},
        {0x402DC0, "imp_InterlockedDecrement"},
        {0x402DD0, "imp_WaitForSingleObject"},
        {0x402DE0, "imp_CreateSemaphoreA"},
        {0x402DF0, "imp_Sleep"},
        {0x402E00, "imp_InterlockedIncrement"},
        {0x402E10, "imp_GetAtomNameA"},
        {0x402E20, "imp_AddAtomA"},
        {0x402E30, "imp_FindAtomA"},
        {0x402E40, "j____do_frame_init"},
        {0x402E50, "j____do_frame_fini"},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x401000, 0xEC2917F6}, {0x401140, 0x649404DA}, {0x401260, 0xBF3731A6},
        {0x401280, 0xE769E2D1}, {0x4012A0, 0x92AF83E6}, {0x4012B0, 0xD424FC3D},
        {0x4012C0, 0x65D1C21B}, {0x4012F0, 0xB642BA11}, {0x401310, 0xB93344F6},
        {0x4013A0, 0x45BAD261}, {0x4013D0, 0xB194E212}, {0x4013E0, 0xE24D5A1F},
        {0x401410, 0x9F59570C}, {0x401470, 0xE9D40D8B}, {0x401490, 0x87D9DEC2},
        {0x4014E0, 0x9B4CFE19}, {0x401530, 0xCE6978C3}, {0x4015A0, 0x96FDDBE6},
        {0x401670, 0xA5EED544}, {0x4016C0, 0x6768C8D5}, {0x401750, 0xC7D470A},
        {0x401840, 0x59364DAD}, {0x401870, 0x4335163C}, {0x4018B0, 0xF63DB916},
        {0x4019B0, 0xC2135E3B}, {0x4019E0, 0xB3C7D094}, {0x401A10, 0x88F5400D},
        {0x401BA0, 0x316D63B6}, {0x401BB0, 0x486A04C7}, {0x401BE0, 0xB32950BA},
        {0x401C40, 0xA9AE6ACA}, {0x401CD0, 0x2B7CA683}, {0x401CF0, 0x58FD31D},
        {0x401D70, 0x28AD38FE}, {0x401E00, 0xD5CD1319}, {0x401E90, 0x5ED6B1CE},
        {0x401F00, 0x2FEEE25D}, {0x402020, 0x6BF2B1AA}, {0x402120, 0xB7620B4C},
        {0x402240, 0x5C164F94}, {0x4027C0, 0x6B7B9AD7}, {0x4029A0, 0x6B3B17E6},
        {0x4029B0, 0x7A37380C}, {0x402A50, 0xE4E41C01}, {0x402C70, 0x93195D76},
        {0x402CF0, 0x35BBCDE},  {0x402D00, 0x5696BFCB}, {0x402D10, 0xC20EE550},
        {0x402D20, 0xA4A911B3}, {0x402D30, 0x993D3528}, {0x402D40, 0xC06EBE08},
        {0x402D50, 0xB857D8E3}, {0x402D60, 0x69D364C6}, {0x402D70, 0x88B84E19},
        {0x402D80, 0x808B08CB}, {0x402D90, 0xED17AEBB}, {0x402DA0, 0x997AA1C9},
        {0x402DB0, 0xBBF3DAA6}, {0x402DC0, 0x574CFA97}, {0x402DD0, 0xD46218A8},
        {0x402DE0, 0xC5B3299C}, {0x402DF0, 0x658BF030}, {0x402E00, 0x13C6747D},
        {0x402E10, 0x18498CB5}, {0x402E20, 0x8446CEC8}, {0x402E30, 0xD78FD18A},
        {0x402E40, 0xD4F1E9B9}, {0x402E50, 0xFA8D417B}, {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x404000, {.address = 0x40133a, .type = RD_DR_ADDRESS}},
        {0x404019, {.address = 0x401346, .type = RD_DR_ADDRESS}},
        {0x404024, {.address = 0x401359, .type = RD_DR_ADDRESS}},
        {0x404027, {.address = 0x401368, .type = RD_DR_ADDRESS}},
        {0x40402e, {.address = 0x40137c, .type = RD_DR_ADDRESS}},
        {0x404041, {.address = 0x40138a, .type = RD_DR_ADDRESS}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x401260, .name = "_mainCRTStartup"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060C4, .module = "KERNEL32.dll", .name = "AddAtomA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060C8, .module = "KERNEL32.dll", .name = "CreateSemaphoreA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060CC, .module = "KERNEL32.dll", .name = "ExitProcess"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060D0, .module = "KERNEL32.dll", .name = "FindAtomA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060D4, .module = "KERNEL32.dll", .name = "GetAtomNameA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060D8, .module = "KERNEL32.dll", .name = "InterlockedDecrement"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060DC, .module = "KERNEL32.dll", .name = "InterlockedIncrement"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060E0, .module = "KERNEL32.dll", .name = "ReleaseSemaphore"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060E4, .module = "KERNEL32.dll", .name = "SetUnhandledExceptionFilter"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060E8, .module = "KERNEL32.dll", .name = "Sleep"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060EC, .module = "KERNEL32.dll", .name = "WaitForSingleObject"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060F8, .module = "msvcrt.dll", .name = "__getmainargs"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060FC, .module = "msvcrt.dll", .name = "__p__environ"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406100, .module = "msvcrt.dll", .name = "__p__fmode"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406104, .module = "msvcrt.dll", .name = "__set_app_type"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406108, .module = "msvcrt.dll", .name = "_assert"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40610C, .module = "msvcrt.dll", .name = "_cexit"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406110, .module = "msvcrt.dll", .name = "_iob"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406114, .module = "msvcrt.dll", .name = "_onexit"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406118, .module = "msvcrt.dll", .name = "_setmode"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40611C, .module = "msvcrt.dll", .name = "abort"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406120, .module = "msvcrt.dll", .name = "atexit"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406124, .module = "msvcrt.dll", .name = "free"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406128, .module = "msvcrt.dll", .name = "malloc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40612C, .module = "msvcrt.dll", .name = "printf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406130, .module = "msvcrt.dll", .name = "scanf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406134, .module = "msvcrt.dll", .name = "signal"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406138, .module = "msvcrt.dll", .name = "strcmp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40613C, .module = "msvcrt.dll", .name = "strlen"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "ioli/win32/crackme0x00.exe",
        .loader_id = "win_pe",
        .processor_id = "x86_32",
        .entry_point =
            {
                .value = 0x401260,
                .has_value = true,
                .no_ret = true,
            },
        .names = NAMES,
        .graphs = GRAPHS,
        .externals = EXTERNALS,
        .n_functions = 68,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static const RDTest K_TESTS[] = {
    {"test_0x00_linux", test_0x00_linux},
    {"test_0x00_pocketpc", test_0x00_pocketpc},
    {"test_0x00_pocketpc_stripped", test_0x00_pocketpc_stripped},
    {"test_0x00_win32", test_0x00_win32},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("ioli", K_TESTS);
    rdtest_deinit();
    return result;
}
