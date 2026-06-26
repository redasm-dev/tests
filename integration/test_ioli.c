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
        {0x8048568, {.name = "char", .count = 25}},
        {0x8048581, {.name = "char", .count = 11}},
        {0x804858c, {.name = "char", .count = 3}},
        {0x804858f, {.name = "char", .count = 7}},
        {0x8048596, {.name = "char", .count = 19}},
        {0x80485a9, {.name = "char", .count = 16}},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x80482F8, 0x6832D00E}, {0x8048320, 0xC138F9CA},
        {0x8048330, 0x16C24442}, {0x8048340, 0x4CBEED99},
        {0x8048350, 0x1860A780}, {0x8048360, 0xE2E0A2D7},
        {0x8048384, 0xE84C5A1D}, {0x80483B0, 0x1CD50766},
        {0x80483E0, 0x899BF69},  {0x8048414, 0xD2DC2C8},
        {0x80484A0, 0xD9D0ED66}, {0x8048510, 0x6BD43B5F},
        {0x8048515, 0xE067DC6C}, {0x8048520, 0x9597E9E1},
        {0x8048544, 0x5FC5C688}, {0},
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
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_0x00_pocketpc(void) {
    static const RDTestName NAMES[] = {
        {0x11050, "main"},
        {0x110DC, "_pei386_runtime_relocator"},
        {0x11160, "__dllonexit"},
        {0x111F0, "atexit"},
        {0x11218, "__dll_exit"},
        {0x11290, "_cexit"},
        {0x112B8, "__atexit_init"},
        {0x11338, "__do_global_ctors"},
        {0x113A0, "__gccmain"},
        {0x113C8, "WinMain"},
        {0x11620, "_fpreset"},
        {0x1162C, "TerminateProcess"},
        {0x11638, "puts"},
        {0x11644, "strcmp"},
        {0x11650, "scanf"},
        {0x1165C, "printf"},
        {0x11668, "malloc"},
        {0x11674, "free"},
        {0x11680, "fflush"},
        {0x1168C, "realloc"},
        {0x11698, "_fcloseall"},
        {0x116A4, "strspn"},
        {0x116B0, "strchr"},
        {0x116BC, "strcspn"},
        {0x116C8, "GetModuleFileNameW"},
        {0x116D4, "wcslen"},
        {0x116E0, "strlen"},
        {0x116EC, "wcstombs"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x13000, {.name = "char", .count = 24}},
        {0x13018, {.name = "char", .count = 11}},
        {0x13024, {.name = "char", .count = 3}},
        {0x13028, {.name = "char", .count = 7}},
        {0x13030, {.name = "char", .count = 18}},
        {0x13044, {.name = "char", .count = 15}},
        {0x110c4, {.name = "u32", .mod = RD_TYPE_PTR}},
        {0x110c8, {.name = "u32", .mod = RD_TYPE_PTR}},
        {0x110cc, {.name = "u32", .mod = RD_TYPE_PTR}},
        {0x110d0, {.name = "u32", .mod = RD_TYPE_PTR}},
        {0x110d4, {.name = "u32", .mod = RD_TYPE_PTR}},
        {0x110d8, {.name = "u32", .mod = RD_TYPE_PTR}},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x11000, 0xAE7CF9DA},
        {0x11050, 0xEB4EED5F},
        {0x110DC, 0x2A5E58D5},
        {0x11130, 0x84ABE018},
        {0x11160, 0x2BD19F03},
        {0x111F0, 0xE4D9B903},
        {0x11218, 0x66EC222C},
        {0x11290, 0x71B223AE},
        {0x112B8, 0x5602CC3A},
        {0x11338, 0x9A87CC8},
        {0x113A0, 0x3BCA8FFE},
        {0x113C8, 0x49CC40CA},
        {0x11620, 0x7C7BEA3},
        {0x1162C, 0xCECF6B91},
        {0x11638, 0x9DB6BC44},
        {0x11644, 0x78DBA190},
        {0x11650, 0xDC359F8},
        {0x1165C, 0x90C6EFDC},
        {0x11668, 0x9D76BE01},
        {0x11674, 0x309605A5},
        {0x11680, 0xE9B24C2},
        {0x1168C, 0x2FEDD36F},
        {0x11698, 0x1CE75BD5},
        {0x116A4, 0x7FAE5033},
        {0x116B0, 0x1A08E0A5},
        {0x116BC, 0x8532DF3C},
        {0x116C8, 0xB9D8242F},
        {0x116D4, 0xA2DA836C},
        {0x116E0, 0x3322719D},
        {0x116EC, 0xB7F06F3F},
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
        {.kind = RD_EXT_IMPORTED, .address = 0x1507C, .module = "COREDLL", .name = "__imp_GetModuleFileNameW"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15080, .module = "COREDLL", .name = "__imp_TerminateProcess"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15084, .module = "COREDLL", .name = "__imp__fcloseall"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15088, .module = "COREDLL", .name = "__imp__fpreset"},
        {.kind = RD_EXT_IMPORTED, .address = 0x1508C, .module = "COREDLL", .name = "__imp_fflush"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15090, .module = "COREDLL", .name = "__imp_free"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15094, .module = "COREDLL", .name = "__imp_malloc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x15098, .module = "COREDLL", .name = "__imp_printf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x1509C, .module = "COREDLL", .name = "__imp_puts"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150A0, .module = "COREDLL", .name = "__imp_realloc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150A4, .module = "COREDLL", .name = "__imp_scanf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150A8, .module = "COREDLL", .name = "__imp_strchr"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150AC, .module = "COREDLL", .name = "__imp_strcmp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150B0, .module = "COREDLL", .name = "__imp_strcspn"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150B4, .module = "COREDLL", .name = "__imp_strlen"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150B8, .module = "COREDLL", .name = "__imp_strspn"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150BC, .module = "COREDLL", .name = "__imp_wcslen"},
        {.kind = RD_EXT_IMPORTED, .address = 0x150C0, .module = "COREDLL", .name = "__imp_wcstombs"},
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
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_0x00_win32(void) {
    static const RDTestName NAMES[] = {
        {0x401140, "sub_401140"},
        {0x4012A0, "_atexit"},
        {0x401310, "_main"},
        {0x4013A0, "__pei386_runtime_relocator"},
        {0x4013D0, "__fpreset"},
        {0x401470, "___main"},
        {0x402C70, "__alloca"},
        {0x402CF0, "___p__fmode"},
        {0x402D00, "___getmainargs"},
        {0x402D10, "_strcmp"},
        {0x402D20, "_scanf"},
        {0x402D30, "_printf"},
        {0x402D90, "_ExitProcess@4"},
        {0x402DA0, "_SetUnhandledExceptionFilter@4"},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x401140, 0xAA72EA4B}, {0x401260, 0xE2A51A04},
        {0x4012A0, 0xE2B0B2FA}, {0x401310, 0x277C039E},
        {0x4013A0, 0x69B88F6D}, {0x4013D0, 0x225822E5},
        {0x401470, 0xA509BC4B}, {0x402C70, 0x53CF4957},
        {0x402CF0, 0xB76758B},  {0x402D00, 0x6A2238D2},
        {0x402D10, 0xE0402BFD}, {0x402D20, 0x781ADA7E},
        {0x402D30, 0x96EB361D}, {0x402D90, 0x1EABBCBF},
        {0x402DA0, 0xAFB578AE}, {0},
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
        {.kind = RD_EXT_EXPORTED, .address = 0x401260},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060C4, .module = "KERNEL32.dll", .name = "__imp__AddAtomA@4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060C8, .module = "KERNEL32.dll", .name = "__imp__CreateSemaphoreA@16"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060CC, .module = "KERNEL32.dll", .name = "__imp__ExitProcess@4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060D0, .module = "KERNEL32.dll", .name = "__imp__FindAtomA@4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060D4, .module = "KERNEL32.dll", .name = "__imp__GetAtomNameA@12"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060D8, .module = "KERNEL32.dll", .name = "__imp__InterlockedDecrement@4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060DC, .module = "KERNEL32.dll", .name = "__imp__InterlockedIncrement@4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060E0, .module = "KERNEL32.dll", .name = "__imp__ReleaseSemaphore@12"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060E4, .module = "KERNEL32.dll", .name = "__imp__SetUnhandledExceptionFilter@4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060E8, .module = "KERNEL32.dll", .name = "__imp__Sleep@4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060EC, .module = "KERNEL32.dll", .name = "__imp__WaitForSingleObject@8"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060F8, .module = "msvcrt.dll", .name = "__imp____getmainargs"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4060FC, .module = "msvcrt.dll", .name = "__imp____p__environ"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406100, .module = "msvcrt.dll", .name = "__imp____p__fmode"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406104, .module = "msvcrt.dll", .name = "__imp____set_app_type"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406108, .module = "msvcrt.dll", .name = "__imp___assert"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40610C, .module = "msvcrt.dll", .name = "__imp___cexit"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406110, .module = "msvcrt.dll", .name = "__imp___iob"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406114, .module = "msvcrt.dll", .name = "__imp___onexit"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406118, .module = "msvcrt.dll", .name = "__imp___setmode"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40611C, .module = "msvcrt.dll", .name = "__imp__abort"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406120, .module = "msvcrt.dll", .name = "__imp__atexit"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406124, .module = "msvcrt.dll", .name = "__imp__free"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406128, .module = "msvcrt.dll", .name = "__imp__malloc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40612C, .module = "msvcrt.dll", .name = "__imp__printf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406130, .module = "msvcrt.dll", .name = "__imp__scanf"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406134, .module = "msvcrt.dll", .name = "__imp__signal"},
        {.kind = RD_EXT_IMPORTED, .address = 0x406138, .module = "msvcrt.dll", .name = "__imp__strcmp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40613C, .module = "msvcrt.dll", .name = "__imp__strlen"},
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
            },
        .names = NAMES,
        .graphs = GRAPHS,
        .externals = EXTERNALS,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static const RDTest K_TESTS[] = {
    {"test_0x00_linux", test_0x00_linux},
    {"test_0x00_pocketpc", test_0x00_pocketpc},
    {"test_0x00_win32", test_0x00_win32},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("ioli", K_TESTS);
    rdtest_deinit();
    return result;
}
