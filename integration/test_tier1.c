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
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x80482A8, 0xE2F05F8C},
        {0x80482E0, 0x75D15E12},
        {0x80482F0, 0x792248B2},
        {0x8048300, 0xDD75C1A3},
        {0x8048310, 0x6904099B},
        {0x8048340, 0x63E3DE89},
        {0x804840B, 0x9368989D},
        {0x8048440, 0xCD801CAA},
        {0x80484A0, 0xD8579EFA},
        {0x80484A4, 0xDE95D1FF},
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

static int test_helloworld64(void) {
    static const RDTestName NAMES[] = {
        {0x4003c8, "_init"},           {0x4003f0, "imp_puts"},
        {0x400400, "_start"},          {0x4004f6, "main"},
        {0x400510, "__libc_csu_init"}, {0x400580, "__libc_csu_fini"},
        {0x400584, "_fini"},           {0},
    };

    static const RDTestType TYPES[] = {
        {0x400594, .name = "char", .count = 12},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x4003C8, 0xB249B7CF}, {0x4003F0, 0x4B57624D},
        {0x400400, 0x13922C05}, {0x4004F6, 0xF606FA76},
        {0x400510, 0x42DAEA43}, {0x400580, 0x98A4E5},
        {0x400584, 0xB5FD9D9B}, {0},
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
        {0x80120000, 0xB6A498E0}, {0x801209DC, 0x9B8EDEDC},
        {0x80120E88, 0x6C0F39E7}, {0x80120FA4, 0x208CB82E},
        {0x80121030, 0x2A40D7A6}, {0x8012128C, 0x33235E35},
        {0x80121314, 0x4AF61DDA}, {0x801213DC, 0x342FEB50},
        {0x801219D4, 0x3B4E8AE6}, {0x80121C10, 0x821C5331},
        {0x80121C20, 0x6F4BDAED}, {0x80121C30, 0x885D06C5},
        {0x80121C70, 0x43C315AB}, {0x80121D14, 0xFB8173DB},
        {0x80121FCC, 0x9F47BD6E}, {0x801222F0, 0xFF7415F4},
        {0x801226E0, 0x279180EE}, {0x80122834, 0x584364A3},
        {0x801229A8, 0xE775EC7F}, {0x80122A70, 0x18D35164},
        {0x80122BD0, 0xE75632A9}, {0x80122C6C, 0xE0F24639},
        {0x80122C84, 0x25ED9DDF}, {0x80122D74, 0x647796D9},
        {0x80122DEC, 0x60812DA7}, {0x80122EA8, 0x936F0286},
        {0x801232A0, 0x190E4DA4}, {0x801233FC, 0xDD13ACBC},
        {0x80123628, 0xF32FC9BC}, {0x801236A4, 0xE3E742F4},
        {0x801237AC, 0xCD365FAD}, {0x80123A04, 0x51F26FFE},
        {0x80123C48, 0xB60A2C0E}, {0x80123D40, 0x5EDCD89A},
        {0x80123DB4, 0xC7549D76}, {0x80123F8C, 0x9C5F91F0},
        {0x801245D4, 0x4EE1DB9D}, {0x8012462C, 0x41319A1D},
        {0x801248D0, 0x17EB3695}, {0x8012492C, 0x941BFF26},
        {0x801249F8, 0xB31280DE}, {0x80124AC4, 0xB1B9418A},
        {0x80124B0C, 0x564904E2}, {0x80124B90, 0x991956C9},
        {0x80125BB8, 0x8EA42CF7}, {0x80125FFC, 0x96B749B3},
        {0x801260D8, 0x568977ED}, {0x80126104, 0xA6D4EEBB},
        {0x80126114, 0x6460CA51}, {0x80126144, 0x22CD9625},
        {0x80126208, 0xE48EF4DB}, {0x80126238, 0xBB0BE917},
        {0x80126320, 0xBD6A8E46}, {0x80126340, 0x8000202C},
        {0x80126390, 0x6841554A}, {0x801265F4, 0x302FF8D4},
        {0x80127924, 0x81E27B71}, {0x80127C6C, 0x463A6D52},
        {0x80128010, 0xD9A819CE}, {0x80128128, 0x68F01503},
        {0x80128390, 0x8274AAD1}, {0x801285A4, 0xD9F134BB},
        {0x8012895C, 0xCFD183D4}, {0x80128D24, 0x7367697F},
        {0x80128E20, 0x1C7D9023}, {0x801292F0, 0xAA3EB6B5},
        {0x80129334, 0xE47B3080}, {0x80129404, 0xD8893B9E},
        {0x80129FB8, 0xCC3AB615}, {0x8012A07C, 0x345816B7},
        {0x8012A100, 0xE2062EB},  {0x8012A19C, 0x50FCA578},
        {0x8012A5A4, 0x3065F927}, {0x8012A664, 0x8EBEB9FD},
        {0x8012A690, 0x1896CB45}, {0x8012A6A0, 0xF827B12D},
        {0x8012A6B0, 0xF9A15B92}, {0x8012A6C0, 0xFDDB4278},
        {0x8012A800, 0x2DA82D4D}, {0x8012A838, 0x54A31CE7},
        {0x8012A990, 0x7FB712F},  {0x8012AADC, 0x552F4C57},
        {0x8012AAF4, 0xD939BCDD}, {0x8012B0C4, 0x7412B555},
        {0x8012B63C, 0x1441BF8D}, {0x8012BB88, 0xCAF3C9D9},
        {0x8012C028, 0x708691A0}, {0x8012C108, 0xD4B3A7BB},
        {0x8012C1FC, 0xD978E9BB}, {0x8012C24C, 0x211F22FB},
        {0x8012C79C, 0xB760958B}, {0x8012CD84, 0x33315DD1},
        {0x8012CD9C, 0xC9611CFF}, {0x8012CDEC, 0x26CCFD3},
        {0x8012CE1C, 0xB1729EA0}, {0x8012CE3C, 0xB484895E},
        {0x8012CE4C, 0xC731F0A4}, {0x8012CE5C, 0xCB72174E},
        {0x8012CE6C, 0xCC5835A},  {0x8012CF84, 0xF01FA5D7},
        {0x8012CF94, 0x99DC86AE}, {0x8012CFE4, 0x92CA07E},
        {0x8012CFF4, 0x74497FC1}, {0x8012D13C, 0xC7E41641},
        {0x8012D1D8, 0x2D65E9BC}, {0x8012D1E8, 0xD58567E5},
        {0x8012D218, 0x6527D41A}, {0x8012D248, 0xA00D7195},
        {0x8012D278, 0x497382DB}, {0x8012D2DC, 0x6E13D9E},
        {0x8012D33C, 0xB6B7107F}, {0x8012D364, 0xC8C46FC},
        {0x8012DD28, 0x784785AF}, {0x8012DD40, 0x4AF2712D},
        {0x8012DD50, 0xCF68F1D6}, {0x8012E24C, 0x3C45091A},
        {0x8012E27C, 0x72270BA8}, {0x8012E3AC, 0x3D9B6304},
        {0x8012E3DC, 0xD9014638}, {0x8012E3FC, 0x7E5695FB},
        {0x8012E40C, 0x6EEA5718}, {0x8012E48C, 0x253BEA7A},
        {0x8012E71C, 0xEE07F4A4}, {0x8012E7B0, 0xFA742829},
        {0x8012E828, 0x74FEDA99}, {0x8012EA10, 0x645C9657},
        {0x8012EC8C, 0x33D32A09}, {0x8012EDC8, 0x17A7DDF8},
        {0x8012EDD8, 0x15205803}, {0x8012EEEC, 0xCFEC11E5},
        {0x8012EF0C, 0x2DD3A931}, {0x8012EF9C, 0x20E1EAA2},
        {0x8012F060, 0x9B77E2E1}, {0x8012F100, 0x55F0C6C4},
        {0x8012F178, 0xD8C0EB50}, {0x8012F19C, 0xCE4C3D40},
        {0x8012F1F4, 0xBF588804}, {0x8012F248, 0x22766B40},
        {0x8012F2D0, 0xD1BDB037}, {0x8012F2F0, 0x76C48E27},
        {0x8012F390, 0xD45D86A1}, {0x8012F474, 0xE1C2FCAB},
        {0x8012F484, 0xE3EB75EE}, {0x8012F52C, 0xD8FB6DFF},
        {0x8012F604, 0x400FADB7}, {0x8012F614, 0x9A5B42EA},
        {0x8012F634, 0x56323D6E}, {0x8012F72C, 0x74EC090C},
        {0x8012F7A8, 0xC04AAC7D}, {0x8012F7B8, 0xBA711DE3},
        {0x8012F7C8, 0x539AD361}, {0x8012FA5C, 0xA2FC82C2},
        {0x8012FDB4, 0xD8A53191}, {0x80130044, 0x46911034},
        {0x80130134, 0xC11ECCC0}, {0x8013017C, 0x20B0EA9D},
        {0x801302BC, 0xAFA532E8}, {0x801302E8, 0x2B403BF2},
        {0x80130314, 0x43340460}, {0x80130374, 0x3057C84D},
        {0x80130398, 0x45E7C1D0}, {0x80130414, 0xBD4FDC1C},
        {0x80130424, 0xC7656CC6}, {0x80130434, 0x5B41C767},
        {0x80130488, 0x445B8B58}, {0x80130750, 0xF99124FE},
        {0x80130A50, 0x83F93FFD}, {0x80130ACC, 0x19194797},
        {0x80130AF0, 0xC00F2B06}, {0x80130CF8, 0x719D108B},
        {0x80130D58, 0x3D08945A}, {0x80130DAC, 0xDA09413F},
        {0x80130DE0, 0xEE7ADFC9}, {0x80130E88, 0xF4C34A24},
        {0x80130E98, 0x612761C4}, {0x80130EC4, 0x22F1E5DF},
        {0x80130EDC, 0xAC8C2B6A}, {0},
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
