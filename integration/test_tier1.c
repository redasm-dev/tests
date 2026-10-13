#include "rdtest.h"
#include "rdtest_helpers.h"
#include <redasm/redasm.h>

static int test_helloworld32(void) {
    static const RDTestName NAMES[] = {
        {0x80482A8, "_init"},
        {0x80482E0, "imp_puts"},
        {0x80482F0, "imp___libc_start_main"},
        {0x8048300, "imp___gmon_start__"},
        {0x8048310, "_start"},
        {0x8048340, "__x86.get_pc_thunk.bx"},
        {0x804840B, "main"},
        {0x8048440, "__libc_csu_init"},
        {0x80484A0, "__libc_csu_fini"},
        {0x80484A4, "_fini"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x080484c0, .name = "char", .count = 12},
        {0x08049ffc, .name = "u32"},
        {0x0804a00c, .name = "u32"},
        {0x0804a010, .name = "u32"},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x80482A8, 0xFB1A91AD},
        {0x80482E0, 0xE45612F},
        {0x80482F0, 0xEB27947D},
        {0x8048300, 0x79E0E624},
        {0x8048310, 0x6A1F084F},
        {0x8048340, 0x378501E9},
        {0x804840B, 0xEC33885B},
        {0x8048440, 0x40C7709B},
        {0x80484A0, 0x91144A50},
        {0x80484A4, 0xD408A42D},
        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x804840b, {.address = 0x8048327, .type = RD_DR_ADDRESS}},
        {0x8048440, {.address = 0x8048320, .type = RD_DR_ADDRESS}},
        {0x80484a0, {.address = 0x804831b, .type = RD_DR_ADDRESS}},
        {0x80484c0, {.address = 0x804841f, .type = RD_DR_ADDRESS}},
        {0x8049ffc, {.address = 0x8048300, .type = RD_DR_READ}},
        {0x804a00c, {.address = 0x80482e0, .type = RD_DR_READ}},
        {0x804a010, {.address = 0x80482f0, .type = RD_DR_READ}},
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
        {.kind = RD_EXT_IMPORTED, .address = 0x8049FFC, .name = "__gmon_start__"},
        {.kind = RD_EXT_IMPORTED, .address = 0x804A00C, .name = "puts"},
        {.kind = RD_EXT_IMPORTED, .address = 0x804A010, .name = "__libc_start_main"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "elf/helloworld32",
        .loader_id = "elf",
        .processor_id = "x86_32",
        .entry_point = {.value = 0x08048310, .has_value = true, .no_ret = true},
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .externals = EXTERNALS,
        .n_functions = 10,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_helloworld32_stripped(void) {
    static const RDTestName NAMES[] = {
        {0x80482E0, "imp_puts"},
        {0x80482F0, "imp___libc_start_main"},
        {0x8048300, "imp___gmon_start__"},
        {0x804840B, "main_804840B"},
        {0x8048440, "init_8048440"},
        {0x80484A0, "fini_80484A0"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x080484c0, .name = "char", .count = 12},
        {0x08049ffc, .name = "u32"},
        {0x0804a00c, .name = "u32"},
        {0x0804a010, .name = "u32"},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x80482A8, 0xFB1A91AD}, {0x80482E0, 0xE45612F},
        {0x80482F0, 0xEB27947D}, {0x8048300, 0x79E0E624},
        {0x8048310, 0x6A1F084F}, {0x8048340, 0x378501E9},
        {0x804840B, 0xEC33885B}, {0x8048440, 0x40C7709B},
        {0x80484A0, 0x91144A50}, {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x804840b, {.address = 0x8048327, .type = RD_DR_ADDRESS}},
        {0x8048440, {.address = 0x8048320, .type = RD_DR_ADDRESS}},
        {0x80484a0, {.address = 0x804831b, .type = RD_DR_ADDRESS}},
        {0x80484c0, {.address = 0x804841f, .type = RD_DR_ADDRESS}},
        {0x8049ffc, {.address = 0x8048300, .type = RD_DR_READ}},
        {0x804a00c, {.address = 0x80482e0, .type = RD_DR_READ}},
        {0x804a010, {.address = 0x80482f0, .type = RD_DR_READ}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x80484BC, .name = "_IO_stdin_used"},
        {.kind = RD_EXT_IMPORTED, .address = 0x8049FFC, .name = "__gmon_start__"},
        {.kind = RD_EXT_IMPORTED, .address = 0x804A00C, .name = "puts"},
        {.kind = RD_EXT_IMPORTED, .address = 0x804A010, .name = "__libc_start_main"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "elf/helloworld32_stripped",
        .loader_id = "elf",
        .processor_id = "x86_32",
        .entry_point = {.value = 0x08048310, .has_value = true, .no_ret = true},
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .externals = EXTERNALS,
        .n_functions = 9,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_helloworld64(void) {
    static const RDTestName NAMES[] = {
        {0x4003c8, "_init"},           {0x4003f0, "imp_puts"},
        {0x400400, "_start"},          {0x4004f6, "main"},
        {0x400510, "__libc_csu_init"}, {0x400580, "__libc_csu_fini"},
        {0x400584, "_fini"},           {0},
    };

    static const RDTestType TYPES[] = {
        {0x400594, .name = "char", .count = 12},
        {0x600ff0, .name = "u64"},
        {0x600ff8, .name = "u64"},
        {0x601018, .name = "u64"},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x4003C8, 0x2D442DD1}, {0x4003F0, 0xCA2357AD},
        {0x400400, 0x6076DD0C}, {0x4004F6, 0x8A95C59C},
        {0x400510, 0x7027B7C9}, {0x400580, 0x11786CB0},
        {0x400584, 0xBEB092AE}, {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x4004f6, {.address = 0x40041d, .type = RD_DR_ADDRESS}},
        {0x400510, {.address = 0x400416, .type = RD_DR_ADDRESS}},
        {0x400580, {.address = 0x40040f, .type = RD_DR_ADDRESS}},
        {0x400594, {.address = 0x4004fa, .type = RD_DR_ADDRESS}},
        {0x600ff0, {.address = 0x400424, .type = RD_DR_READ}},
        {0x600ff8, {.address = 0x4003cc, .type = RD_DR_READ}},
        {0x601018, {.address = 0x4003f0, .type = RD_DR_READ}},
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
        {.kind = RD_EXT_IMPORTED, .address = 0x600ff0, .name = "__libc_start_main"},
        {.kind = RD_EXT_IMPORTED, .address = 0x600ff8, .name = "__gmon_start__"},
        {.kind = RD_EXT_IMPORTED, .address = 0x601018, .name = "puts"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "elf/helloworld64",
        .loader_id = "elf",
        .processor_id = "x86_64",
        .entry_point = {.value = 0x400400, .has_value = true, .no_ret = true},
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
        .externals = EXTERNALS,
        .n_functions = 7,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_hitpsx(void) {
    static const RDTestName NAMES[] = {
        {0x80121C10, "memset"},
        {0x80121C20, "printf"},
        {0x80126104, "memcpy"},
        {0x8012A690, "GPU_cw"},
        {0x8012C79C, "puts"},
        {0x8012CE3C, "StopPAD2"},
        {0x8012CE4C, "PAD_dr"},
        {0x8012CE5C, "ChangeClearPAD"},
        {0x8012CF84, "PAD_init2"},
        {0x8012CFE4, "FlushCache"},
        {0x8012D1D8, "ChangeClearRCnt"},
        {0x8012F604, "InitHeap"},
        {0x8012F7A8, "OpenEvent"},
        {0x8012F7B8, "EnableEvent"},
        {0x80130414, "CloseEvent"},
        {0x80130424, "DisableEvent"},
        {0x80130E88, "TestEvent"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x8012001c, .name = "char", .count = 29},
        {0x8012003c, .name = "char", .count = 25},
        {0x80120058, .name = "char", .count = 40},
        {0x80120080, .name = "char", .count = 41},
        {0x801200ac, .name = "char", .count = 40},
        {0x801200d4, .name = "char", .count = 40},
        {0x801200fc, .name = "char", .count = 40},
        {0x80120124, .name = "char", .count = 38},
        {0x8012014c, .name = "char", .count = 38},
        {0x80120174, .name = "char", .count = 39},
        {0x8012019c, .name = "char", .count = 40},
        {0x801201c4, .name = "char", .count = 39},
        {0x801201ec, .name = "char", .count = 38},
        {0x801203b4, .name = "char", .count = 30},
        {0x801203d4, .name = "char", .count = 19},
        {0x801203e8, .name = "char", .count = 24},
        {0x8012045c, .name = "char", .count = 20},
        {0x80120484, .name = "char", .count = 12},
        {0x80120490, .name = "char", .count = 17},
        {0x801204b4, .name = "char", .count = 10},
        {0x801204f0, .name = "char", .count = 24},
        {0x80120508, .name = "char", .count = 19},
        {0x8012051c, .name = "char", .count = 21},
        {0x80120550, .name = "char", .count = 21},
        {0x801205cc, .name = "char", .count = 21},
        {0x80120650, .name = "char", .count = 13},
        {0x80120728, .name = "char", .count = 13},
        {0x80120738, .name = "char", .count = 27},
        {0x80120754, .name = "char", .count = 12},
        {0x80120760, .name = "char", .count = 25},
        {0x8012077c, .name = "char", .count = 20},
        {0x80120790, .name = "char", .count = 6},
        {0x801207b0, .name = "char", .count = 8},
        {0x801207c4, .name = "char", .count = 7},
        {0x801207cc, .name = "char", .count = 14},
        {0x801207dc, .name = "char", .count = 6},
        {0x80120818, .name = "char", .count = 9},
        {0x80120824, .name = "char", .count = 11},
        {0x80120880, .name = "char", .count = 16},
        {0x80120880, .name = "char", .count = 16},
        {0x80120928, .name = "char", .count = 42},
        {0x80120954, .name = "char", .count = 14},
        {0x80120964, .name = "char", .count = 13},
        {0x80120974, .name = "char", .count = 19},
        {0x80120988, .name = "char", .count = 20},
        {0x80120790, .name = "char", .count = 6},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x80120000, 0xFF9C0944},
        {0x801209DC, 0x4289FD9B},
        {0x80120E88, 0xC963CA8A},
        {0x80120FA4, 0xAE6D11DF},
        {0x80121030, 0xEBBD4EED},
        {0x8012128C, 0x5774BA88},
        {0x80121314, 0x6486D31B},
        {0x801213DC, 0x4BD44092},
        {0x801219D4, 0x68712671},
        {0x80121C10, 0x1501907F},
        {0x80121C20, 0xBC49F3B},
        {0x80121C30, 0x4A26A52E},
        {0x80121C70, 0xF2310FC},
        {0x80121D14, 0xC3E98E2C},
        {0x80121FCC, 0xC24C13FC},
        {0x801222F0, 0xEEB6A4E9},
        {0x801226E0, 0x95E561E3},
        {0x80122834, 0x81ED3459},
        {0x801229A8, 0xD06118B3},
        {0x80122A70, 0x544EC7B8},
        {0x80122BD0, 0x47102DE2},
        {0x80122C6C, 0xC1D614E},
        {0x80122C84, 0x830C44D},
        {0x80122D74, 0x1A6A985F},
        {0x80122DEC, 0x2DC89559},
        {0x80122EA8, 0x3561849},
        {0x801232A0, 0xAD655A51},
        {0x801233FC, 0x68AE9434},
        {0x80123628, 0xD441F8E2},
        {0x801236A4, 0xBD223FF7},
        {0x801237AC, 0x473D7B70},
        {0x80123A04, 0x9DD0A88B},
        {0x80123C48, 0x132A7049},
        {0x80123D40, 0xAF1513F8},
        {0x80123DB4, 0xF2DA3670},
        {0x80123F8C, 0xADD466DE},
        {0x801245D4, 0xFFA5CA54},
        {0x8012462C, 0xCB56A4CE},
        {0x801248D0, 0xB810D311},
        {0x8012492C, 0xBE315862},
        {0x801249F8, 0x34BD3332},
        {0x80124AC4, 0x74D66D01},
        {0x80124B0C, 0x7132ABC7},
        {0x80124B90, 0xBBEDF62},
        {0x80125BB8, 0xCCF89F7B},
        {0x80125FFC, 0xB48DE1F},
        {0x801260D8, 0x93F78291},
        {0x80126104, 0x4951FEC},
        {0x80126114, 0xA88E6EFA},
        {0x80126144, 0x3D77BFBA},
        {0x80126208, 0xDC163DCE},
        {0x80126238, 0x1FD642B6},
        {0x80126320, 0x987D7220},
        {0x80126340, 0xB04CD052},
        {0x80126390, 0x849DCFD0},
        {0x801265F4, 0x994F803B},
        {0x80127924, 0x6B5447B3},
        {0x80127C6C, 0x975F4595},
        {0x80128010, 0x7643E3B6},
        {0x80128128, 0x4E7CC830},
        {0x80128390, 0x1E3A463E},
        {0x801285A4, 0x1838824D},
        {0x8012895C, 0x52C9B392},
        {0x80128D24, 0xB354E822},
        {0x80128E20, 0x27B5E21A},
        {0x801292F0, 0x83154BD},
        {0x80129334, 0xB2ED4014},
        {0x80129404, 0xF9EF98AA},
        {0x80129FB8, 0xED50CF8},
        {0x8012A07C, 0x71787792},
        {0x8012A100, 0x80D038AF},
        {0x8012A19C, 0x46DC5064},
        {0x8012A5A4, 0x21E9462F},
        {0x8012A664, 0xD8BEAAB2},
        {0x8012A690, 0xE1574E17},
        {0x8012A6A0, 0x8F4EB92C},
        {0x8012A6B0, 0xD4C4C334},
        {0x8012A6C0, 0x7D037326},
        {0x8012A800, 0x85931EBB},
        {0x8012A838, 0x76BAAE38},
        {0x8012A990, 0x89BA267B},
        {0x8012AADC, 0xCBCC6121},
        {0x8012AAF4, 0x5CE1BADC},
        {0x8012B0C4, 0x2F41938F},
        {0x8012B63C, 0xAC8AF7DE},
        {0x8012BB88, 0xC4CA2FE3},
        {0x8012C028, 0xE3B6BBE7},
        {0x8012C108, 0x11FB1CF2},
        {0x8012C1FC, 0x9AC3F889},
        {0x8012C24C, 0x190DD813},
        {0x8012C79C, 0x5BA8F321},
        {0x8012CD84, 0xA3B037F6},
        {0x8012CD9C, 0xB3B47A70},
        {0x8012CDEC, 0xB382B408},
        {0x8012CE1C, 0x910147C6},
        {0x8012CE3C, 0x44702A5E},
        {0x8012CE4C, 0xE0A7DDA8},
        {0x8012CE5C, 0x4A90B074},
        {0x8012CE6C, 0xEC332EDF},
        {0x8012CF84, 0xFD88C9C1},
        {0x8012CF94, 0xBFA10DF9},
        {0x8012CFE4, 0x79FFDC0D},
        {0x8012CFF4, 0x783A71BE},
        {0x8012D13C, 0x30E56BDB},
        {0x8012D1D8, 0xEBBF9C60},
        {0x8012D1E8, 0x448386F},
        {0x8012D218, 0xE2FFC4F8},
        {0x8012D248, 0x317C7EA},
        {0x8012D278, 0x8AA0DE1D},
        {0x8012D2DC, 0x7E5CE0AA},
        {0x8012D33C, 0xDE651F0B},
        {0x8012D364, 0x5294798F},
        {0x8012DD28, 0x54B2558D},
        {0x8012DD40, 0x353F8021},
        {0x8012DD50, 0xA07B04D5},
        {0x8012E24C, 0x5CDA667E},
        {0x8012E27C, 0xE30DCDF4},
        {0x8012E3AC, 0xBC25775D},
        {0x8012E3DC, 0x96C64D2C},
        {0x8012E3FC, 0xB1470ED2},
        {0x8012E40C, 0x2B92F047},
        {0x8012E48C, 0x559494C7},
        {0x8012E71C, 0x4E54B905},
        {0x8012E7B0, 0xC4CBFEC},
        {0x8012E828, 0x2A2CFFB0},
        {0x8012EA10, 0xAD4B06A6},
        {0x8012EC8C, 0x888B7359},
        {0x8012EDC8, 0x30C75703},
        {0x8012EDD8, 0xA76E94C5},
        {0x8012EEEC, 0xBF884B02},
        {0x8012EF0C, 0x22F947DB},
        {0x8012EF9C, 0x8F0BCC6F},
        {0x8012F060, 0xFA7A03F3},
        {0x8012F100, 0xB2E716EF},
        {0x8012F178, 0x905E8440},
        {0x8012F19C, 0x7B06B1B4},
        {0x8012F1F4, 0x65EE4CF8},
        {0x8012F248, 0xA7BD5CF8},
        {0x8012F2D0, 0x9F49C32D},
        {0x8012F2F0, 0x4FE22A20},
        {0x8012F390, 0xDB1C39B9},
        {0x8012F474, 0xC8841188},
        {0x8012F484, 0x90A7E59},
        {0x8012F52C, 0x29EC6706},
        {0x8012F604, 0x48E65BAB},
        {0x8012F614, 0x5C92F7DF},
        {0x8012F634, 0x37B846CE},
        {0x8012F72C, 0x3B638D35},
        {0x8012F7A8, 0x7AB5F316},
        {0x8012F7B8, 0xF4E06BEE},
        {0x8012F7C8, 0x5EE76AF1},
        {0x8012FA5C, 0xE3B3C3AA},
        {0x8012FDB4, 0xD40B01CB},
        {0x80130044, 0xFC4F4D84},
        {0x80130134, 0xD7FC64BE},
        {0x8013017C, 0x358D6D2C},
        {0x801302BC, 0x33384AA},
        {0x801302E8, 0xE0F2BB02},
        {0x80130314, 0xF9D3FF49},
        {0x80130374, 0xBA5C17D9},
        {0x80130398, 0xF7792666},
        {0x80130414, 0x7A401457},
        {0x80130424, 0x1716AEE6},
        {0x80130434, 0xA7547DFD},
        {0x80130488, 0xDDB22A61},
        {0x80130750, 0x3E8A8BA5},
        {0x80130A50, 0xACAA134A},
        {0x80130ACC, 0x578B47C3},
        {0x80130AF0, 0x7D5E5838},
        {0x80130CF8, 0x21E862D6},
        {0x80130D58, 0x68D26F79},
        {0x80130DAC, 0x63130C42},
        {0x80130DE0, 0xF5E831DD},
        {0x80130E88, 0xBFC4735E},
        {0x80130E98, 0x9E384E4A},
        {0x80130EC4, 0xC1CCA6EA},
        {0x80130EDC, 0xCEF5F7B7},

        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x8012001c, {.address = 0x80120a38, .type = RD_DR_ADDRESS}},
        {0x8012003c, {.address = 0x80120a6c, .type = RD_DR_ADDRESS}},
        {0x80120058, {.address = 0x80120b28, .type = RD_DR_ADDRESS}},
        {0x80120080, {.address = 0x80120b48, .type = RD_DR_ADDRESS}},
        {0x801200ac, {.address = 0x80120b58, .type = RD_DR_ADDRESS}},
        {0x801200d4, {.address = 0x80120b68, .type = RD_DR_ADDRESS}},
        {0x801200fc, {.address = 0x80120b78, .type = RD_DR_ADDRESS}},
        {0x80120124, {.address = 0x80120b90, .type = RD_DR_ADDRESS}},
        {0x8012014c, {.address = 0x80120ba0, .type = RD_DR_ADDRESS}},
        {0x80120174, {.address = 0x80120bb0, .type = RD_DR_ADDRESS}},
        {0x8012019c, {.address = 0x80120bc0, .type = RD_DR_ADDRESS}},
        {0x801201c4, {.address = 0x80120bd0, .type = RD_DR_ADDRESS}},
        {0x801201ec, {.address = 0x80120be0, .type = RD_DR_ADDRESS}},
        {0x801203d4, {.address = 0x801233b0, .type = RD_DR_ADDRESS}},
        {0x801203e8, {.address = 0x80123434, .type = RD_DR_ADDRESS}},
        {0x8012045c, {.address = 0x801236d4, .type = RD_DR_ADDRESS}},
        {0x80120484, {.address = 0x80123870, .type = RD_DR_ADDRESS}},
        {0x80120490, {.address = 0x801238b4, .type = RD_DR_ADDRESS}},
        {0x801204b4, {.address = 0x80123a1c, .type = RD_DR_ADDRESS}},
        {0x801204f0, {.address = 0x80123c74, .type = RD_DR_ADDRESS}},
        {0x80120508, {.address = 0x80123d64, .type = RD_DR_ADDRESS}},
        {0x8012051c, {.address = 0x80123de8, .type = RD_DR_ADDRESS}},
        {0x80120550, {.address = 0x80123fc8, .type = RD_DR_ADDRESS}},
        {0x801205cc, {.address = 0x8012a8a4, .type = RD_DR_ADDRESS}},
        {0x80120650, {.address = 0x80124138, .type = RD_DR_ADDRESS}},
        {0x80120728, {.address = 0x8012b6f4, .type = RD_DR_ADDRESS}},
        {0x80120728, {.address = 0x8012bde8, .type = RD_DR_ADDRESS}},
        {0x80120738, {.address = 0x8012b748, .type = RD_DR_ADDRESS}},
        {0x80120738, {.address = 0x8012be3c, .type = RD_DR_ADDRESS}},
        {0x80120754, {.address = 0x8012b294, .type = RD_DR_ADDRESS}},
        {0x80120760, {.address = 0x8012b2e0, .type = RD_DR_ADDRESS}},
        {0x80120760, {.address = 0x8012b2e0, .type = RD_DR_ADDRESS}},
        {0x8012077c, {.address = 0x8012b600, .type = RD_DR_ADDRESS}},
        {0x80120790, {.address = 0x8012b614, .type = RD_DR_ADDRESS}},
        {0},
    };

    RDTestSample s = {
        .rel_path = "psx/HITPSX.EXE",
        .loader_id = "psx_exe",
        .processor_id = "mips32_le",
        .entry_point = {.value = 0x8012f484, .has_value = true},
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
        .n_functions = 177,
        .skip_rendering = true, // listing is too big
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static const RDTest K_TESTS[] = {
    {"test_helloworld32", test_helloworld32},
    {"test_helloworld32_stripped", test_helloworld32_stripped},
    {"test_helloworld64", test_helloworld64},
    {"test_hitpsx", test_hitpsx},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("tier1", K_TESTS);
    rdtest_deinit();
    return result;
}
