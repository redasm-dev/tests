#include "rdtest.h"
#include "rdtest_helpers.h"
#include <redasm/redasm.h>

static int test_helloworld32(void) {
    static const RDTestName NAMES[] = {
        {0x80482A8, "_init"},
        {0x80482E0, "puts"},
        {0x80482F0, "__libc_start_main"},
        {0x8048300, "__gmon_start__"},
        {0x8048310, "_start"},
        {0x8048340, "__x86.get_pc_thunk.bx"},
        {0x804840B, "main"},
        {0x8048440, "__libc_csu_init"},
        {0x80484A0, "__libc_csu_fini"},
        {0x80484A4, "_fini"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x080484c0, {.name = "char", .count = 12}},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x80482A8, 0xA1211E8C},
        {0x80482E0, 0x8CCC50E9},
        {0x80482F0, 0xF8A72C95},
        {0x8048300, 0xE6C5DB74},
        {0x8048310, 0x7CF9A20B},
        {0x8048340, 0x7C39B4FC},
        {0x804840B, 0xE7B27634},
        {0x8048440, 0x50B6A6F0},
        {0x80484A0, 0x84BBFBEF},
        {0x80484A4, 0xF91B4142},
        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x804840b, {.address = 0x8048327, .type = RD_DR_ADDRESS}},
        {0x8048440, {.address = 0x8048320, .type = RD_DR_ADDRESS}},
        {0x80484a0, {.address = 0x804831b, .type = RD_DR_ADDRESS}},
        {0x80484c0, {.address = 0x804841f, .type = RD_DR_ADDRESS}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x80482A8, .name = "_init"},
        {.kind = RD_EXT_EXPORTED, .address = 0x8048310, .name = "_start"},
        {.kind = RD_EXT_EXPORTED, .address = 0x8048340, .name = "__x86.get_pc_thunk.bx"},
        {.kind = RD_EXT_EXPORTED, .address = 0x804840B, .name = "main"},
        {.kind = RD_EXT_EXPORTED, .address = 0x8048440, .name = "__libc_csu_init"},
        {.kind = RD_EXT_EXPORTED, .address = 0x80484A0, .name = "__libc_csu_fini"},
        {.kind = RD_EXT_EXPORTED, .address = 0x80484A4, .name = "_fini"},
        {.kind = RD_EXT_EXPORTED, .address = 0x80484B8, .name = "_fp_hw"},
        {.kind = RD_EXT_EXPORTED, .address = 0x80484BC, .name = "_IO_stdin_used"},
        {.kind = RD_EXT_EXPORTED, .address = 0x804A018, .name = "__dso_handle"},
        {.kind = RD_EXT_EXPORTED, .address = 0x804A01C, .name = "__TMC_END__"},
        {.kind = RD_EXT_IMPORTED, .address = 0x8049FFC, .name = "__imp___gmon_start__"},
        {.kind = RD_EXT_IMPORTED, .address = 0x804A00C, .name = "__imp_puts"},
        {.kind = RD_EXT_IMPORTED, .address = 0x804A010, .name = "__imp___libc_start_main"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "elf/helloworld32",
        .loader_id = "elf",
        .processor_id = "x86_32",
        .entry_point = {.value = 0x08048310, .has_value = true},
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .externals = EXTERNALS,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_helloworld64(void) {
    static const RDTestName NAMES[] = {
        {0x4003c8, "_init"},           {0x4003f0, "puts"},
        {0x400400, "_start"},          {0x4004f6, "main"},
        {0x400510, "__libc_csu_init"}, {0x400580, "__libc_csu_fini"},
        {0x400584, "_fini"},           {0},
    };

    static const RDTestType TYPES[] = {
        {0x400594, {.name = "char", .count = 12}},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x4003C8, 0x20E870CF}, {0x4003F0, 0x6658DE17},
        {0x400400, 0x5311FBAD}, {0x4004F6, 0xB2F4500C},
        {0x400510, 0x671E91C5}, {0x400580, 0xE9DD3B5F},
        {0x400584, 0xC24FE81F}, {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x4004f6, {.address = 0x40041d, .type = RD_DR_ADDRESS}},
        {0x400510, {.address = 0x400416, .type = RD_DR_ADDRESS}},
        {0x400580, {.address = 0x40040f, .type = RD_DR_ADDRESS}},
        {0x400594, {.address = 0x4004fa, .type = RD_DR_ADDRESS}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x4003c8, .name = "_init"},
        {.kind = RD_EXT_EXPORTED, .address = 0x400400, .name = "_start"},
        {.kind = RD_EXT_EXPORTED, .address = 0x4004f6, .name = "main"},
        {.kind = RD_EXT_EXPORTED, .address = 0x400510, .name = "__libc_csu_init"},
        {.kind = RD_EXT_EXPORTED, .address = 0x400580, .name = "__libc_csu_fini"},
        {.kind = RD_EXT_EXPORTED, .address = 0x400584, .name = "_fini"},
        {.kind = RD_EXT_EXPORTED, .address = 0x400590, .name = "_IO_stdin_used"},
        {.kind = RD_EXT_EXPORTED, .address = 0x601028, .name = "__dso_handle"},
        {.kind = RD_EXT_EXPORTED, .address = 0x601030, .name = "__TMC_END__"},
        {.kind = RD_EXT_IMPORTED, .address = 0x600ff0, .name = "__imp___libc_start_main"},
        {.kind = RD_EXT_IMPORTED, .address = 0x600ff8, .name = "__imp___gmon_start__"},
        {.kind = RD_EXT_IMPORTED, .address = 0x601018, .name = "__imp_puts"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "elf/helloworld64",
        .loader_id = "elf",
        .processor_id = "x86_64",
        .entry_point = {.value = 0x400400, .has_value = true},
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
        .externals = EXTERNALS,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static const RDTest K_TESTS[] = {
    {"test_helloworld32", test_helloworld32},
    {"test_helloworld64", test_helloworld64},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("tier1", K_TESTS);
    rdtest_deinit();
    return result;
}
