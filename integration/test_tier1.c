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
        {0x80482A8, 0x82877CD7, 0xBF0C4A0E},
        {0x80482E0, 0xA0425ECC, 0x1BF9C8DB},
        {0x80482F0, 0xDA6642A6, 0xB67B2771},
        {0x8048300, 0x685EDE30, 0xD326FF87},
        {0x8048310, 0x9636FB11, 0xB67B2771},
        {0x8048340, 0xAE3BEEB7, 0x3B81623F},
        {0x804840B, 0x569C0344, 0x1BF9C8DB},
        {0x8048440, 0xB06BA52E, 0x66E7171D},
        {0x80484A0, 0x1C881747, 0x75439731},
        {0x80484A4, 0xEEE12D01, 0xA7513230},
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
        {0x80482A8, 0x82877CD7, 0x14C13B23},
        {0x80482E0, 0xA0425ECC, 0x54C60DCF},
        {0x80482F0, 0xDA6642A6, 0xBC23C378},
        {0x8048300, 0x685EDE30, 0x142B9CCA},
        {0x8048310, 0x9636FB11, 0xBC23C378},
        {0x8048340, 0xAE3BEEB7, 0x7DB5779D},
        {0x804840B, 0x569C0344, 0x54C60DCF},
        {0x8048440, 0xB06BA52E, 0x6FBA68E4},
        {0x80484A0, 0x1C881747, 0xDF6FCE37},
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
        {0x4003C8, 0x52C091F1, 0x89956CB5}, {0x4003F0, 0xBA03333E, 0xE3E0A29D},
        {0x400400, 0x58564A71, 0xB194F88D}, {0x4004F6, 0x32A761F9, 0xE3E0A29D},
        {0x400510, 0xFCAECB1B, 0x89956CB5}, {0x400580, 0xBE37D5D2, 0x882BD5F8},
        {0x400584, 0x9C8AB612, 0x8E18BA72}, {0},
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
        {0x80120000, 0x690001E7, 0xFDB59F1C},
        {0x801209DC, 0xD975A365, 0x3DD1E53E},
        {0x80120E88, 0xB59E3EA3, 0xE510E32B},
        {0x80120FA4, 0x61E4ED59, 0x16A82BB0},
        {0x80121030, 0xC93B884A, 0x78333A2},
        {0x8012128C, 0x3CB945BB, 0x8E431120},
        {0x80121314, 0xE6544A8B, 0xE2902F04},
        {0x801213DC, 0x94129C8E, 0xCD1CD572},
        {0x801219D4, 0xAC73EA78, 0x890DDA1D},
        {0x80121C10, 0x940BBA51, 0xC94F34A0},
        {0x80121C20, 0xEAEC61E3, 0xB25232C1},
        {0x80121C30, 0xC5D198A2, 0xD79C0529},
        {0x80121C70, 0x822D571A, 0x59461671},
        {0x80121D14, 0xEC5DF49E, 0x840AC2FB},
        {0x80121FCC, 0x6D8C3796, 0x7F6CF7F1},
        {0x801222F0, 0x739B47E0, 0xFC6D00E4},
        {0x801226E0, 0xF0FDC010, 0xEAB0224},
        {0x80122834, 0x8774AF66, 0xF8AECBCD},
        {0x801229A8, 0xEAE54E7F, 0x3571B109},
        {0x80122A70, 0x16DC1094, 0xCFAB65F6},
        {0x80122BD0, 0xD529447E, 0x8AAA1F70},
        {0x80122C6C, 0xA108279A, 0xC07CE45A},
        {0x80122C84, 0x4CE9C1B3, 0xBF874BAE},
        {0x80122D74, 0xF2BBC889, 0x231AD308},
        {0x80122DEC, 0x244F15FF, 0x233426C6},
        {0x80122EA8, 0x2F783A87, 0xC70A4F92},
        {0x801232A0, 0x2E7F2298, 0xE0600A98},
        {0x801233FC, 0x478D2B5D, 0xEAA63904},
        {0x80123628, 0x921AB1A3, 0x3571B109},
        {0x801236A4, 0x3C4026B3, 0x9B69E0C5},
        {0x801237AC, 0x298E83E5, 0x83CCAAD5},
        {0x80123A04, 0xFA803DB3, 0x83CCAAD5},
        {0x80123C48, 0xF8750272, 0x21771B3},
        {0x80123D40, 0x9977CF24, 0xA0B8FCAE},
        {0x80123DB4, 0xB069D2AB, 0xBF1FEA3B},
        {0x80123F8C, 0x8719DF5D, 0x6F952AF7},
        {0x801245D4, 0x121289E3, 0x1EF9552A},
        {0x8012462C, 0xDF4FF2AD, 0x4B57D57A},
        {0x801248D0, 0xAF40D2A, 0xC1E0A8BC},
        {0x8012492C, 0xCAC15BBF, 0x92328D28},
        {0x801249F8, 0x25CF0F6F, 0x712A852},
        {0x80124AC4, 0x7271A1E7, 0x852AF477},
        {0x80124B0C, 0x36E93025, 0xEECF9A3B},
        {0x80124B90, 0x4F74A502, 0x1B97B66D},
        {0x80125BB8, 0xD3DA14AA, 0xEAE56F8B},
        {0x80125FFC, 0xD2BBBFAC, 0x1C949BD3},
        {0x801260D8, 0x8DE6976F, 0x143F284},
        {0x80126104, 0xB67E99F2, 0xDC80B6C0},
        {0x80126114, 0xFF07C5B0, 0x3C53F04A},
        {0x80126144, 0x2CC53355, 0xD255E203},
        {0x80126208, 0xD91EF032, 0x65E37BBB},
        {0x80126238, 0xD890C672, 0xEF32D8D0},
        {0x80126320, 0xF28A704, 0x9911B6D1},
        {0x80126340, 0x47DD1C7C, 0xB55A7E5B},
        {0x80126390, 0x99F26A4D, 0x5EBDA84E},
        {0x801265F4, 0x26C1DD5F, 0x5EBDA84E},
        {0x80127924, 0x6FD7021E, 0x2652C864},
        {0x80127C6C, 0xDE88C187, 0x6399A318},
        {0x80128010, 0x93437956, 0xF9EFFF22},
        {0x80128128, 0xC30E0D19, 0x5D7A5CFB},
        {0x80128390, 0x304590DA, 0x36B0B48A},
        {0x801285A4, 0x8CCFACD3, 0xB7B49271},
        {0x8012895C, 0x8D583BB9, 0xEF32D8D0},
        {0x80128D24, 0xAFE754C8, 0xE0A616B1},
        {0x80128E20, 0xEA031A09, 0xA652814E},
        {0x801292F0, 0x9EBC92C3, 0xCB7D3D00},
        {0x80129334, 0xBA2D170E, 0xA6DDEBED},
        {0x80129404, 0xE80BBD92, 0x717D394B},
        {0x80129FB8, 0x53DFD3C7, 0xA33FC576},
        {0x8012A07C, 0x6C554BF6, 0xF83E4411},
        {0x8012A100, 0x22F9EC7A, 0x3DA5B024},
        {0x8012A19C, 0xF2ABA904, 0x3DA5B024},
        {0x8012A5A4, 0xBEB75EF0, 0xFA64E82B},
        {0x8012A664, 0x8AB4B53A, 0x1784E333},
        {0x8012A690, 0x9A4E7E7F, 0x8A35178},
        {0x8012A6A0, 0x3A2DBDC, 0xD60BFB8D},
        {0x8012A6B0, 0xB8395912, 0x74F21DE5},
        {0x8012A6C0, 0xB93E2B82, 0xDD27353B},
        {0x8012A800, 0x9F3315E3, 0x3DFB7FD7},
        {0x8012A838, 0x3709DC86, 0x737B8B01},
        {0x8012A990, 0x8CE18A27, 0x797CE477},
        {0x8012AADC, 0x85268F61, 0xF8D88D12},
        {0x8012AAF4, 0xE1154105, 0xB153E6B},
        {0x8012B0C4, 0xE02ACD18, 0x46956E2A},
        {0x8012B63C, 0x4B78D0B1, 0xE30A0B61},
        {0x8012BB88, 0x915F70C8, 0xD0A02CD0},
        {0x8012C028, 0xA51CA8E2, 0x9F20B46A},
        {0x8012C108, 0xCCEEE8C9, 0x18CBD8F6},
        {0x8012C1FC, 0xEFF79C9E, 0x11FB515E},
        {0x8012C24C, 0xB3A5174F, 0xCCAB8DC9},
        {0x8012C79C, 0x1CF77FD4, 0xF15440F0},
        {0x8012CD84, 0xFBE013C9, 0xE5B74394},
        {0x8012CD9C, 0x32EC4FB0, 0xCD0E8F96},
        {0x8012CDEC, 0xE58E9AF5, 0x502553F5},
        {0x8012CE1C, 0x5B6FA8D7, 0xF0C3948F},
        {0x8012CE3C, 0x8201ACBC, 0xF0C3948F},
        {0x8012CE4C, 0x53AF9C3, 0x502553F5},
        {0x8012CE5C, 0xD953BF31, 0x3D706DC1},
        {0x8012CE6C, 0x1E9AFAB5, 0x607348B2},
        {0x8012CF84, 0x4216298A, 0xEEC94EC9},
        {0x8012CF94, 0xE9FC83F6, 0xD13CEACD},
        {0x8012CFE4, 0x590A9578, 0xCA2168F},
        {0x8012CFF4, 0xF208E5D9, 0xBB994E7F},
        {0x8012D13C, 0x7AAE00D7, 0xBB994E7F},
        {0x8012D1D8, 0x30FBFB87, 0xEC498237},
        {0x8012D1E8, 0xE2FB3452, 0xAF963B3E},
        {0x8012D218, 0x84DF059F, 0xF55495E0},
        {0x8012D248, 0xC32171CB, 0x2C3D6A51},
        {0x8012D278, 0x169992D5, 0x80D088B},
        {0x8012D2DC, 0xDD39CF06, 0x87F2A29D},
        {0x8012D33C, 0x1E4B6B16, 0x326F9F93},
        {0x8012D364, 0x369DB266, 0x78C417D4},
        {0x8012DD28, 0x3AFFD3CE, 0x130430D7},
        {0x8012DD40, 0xF2430C0E, 0x29636608},
        {0x8012DD50, 0xEE3714A7, 0xB7FB45EB},
        {0x8012E24C, 0x85701EBF, 0x3AA345EF},
        {0x8012E27C, 0x7363E5D2, 0xD8F237D2},
        {0x8012E3AC, 0xBCD72388, 0xB2B99ADC},
        {0x8012E3DC, 0x1FF276C5, 0x5ED8C4CA},
        {0x8012E3FC, 0x704D18FF, 0xF7664108},
        {0x8012E40C, 0xB198F522, 0xAEDB3889},
        {0x8012E48C, 0x3CDF67A8, 0x33A7E9A9},
        {0x8012E71C, 0xD8C3B9C4, 0x23D1472B},
        {0x8012E7B0, 0x3C45AD47, 0xC47D76E7},
        {0x8012E828, 0x886AFA69, 0x976C765F},
        {0x8012EA10, 0x5805D763, 0xC70A4F92},
        {0x8012EC8C, 0x71621918, 0xBBB81A9C},
        {0x8012EDC8, 0x4D9E8CA1, 0xBD577D6D},
        {0x8012EDD8, 0x45BAB89C, 0xF4B1459F},
        {0x8012EEEC, 0xCC02FBD4, 0xEFABC093},
        {0x8012EF0C, 0x17794ED1, 0x2AA4FA60},
        {0x8012EF9C, 0xA5018E14, 0x857BDC9C},
        {0x8012F060, 0xE99F3B2D, 0x34A97B2C},
        {0x8012F100, 0xBB28443C, 0xBED0AD3C},
        {0x8012F178, 0x63F66CAD, 0x62D32062},
        {0x8012F19C, 0x1FAC315B, 0x21771B3},
        {0x8012F1F4, 0x3BDC8C86, 0x7F9C4764},
        {0x8012F248, 0xC60CA17, 0xF68F4D03},
        {0x8012F2D0, 0xCC4BE8EF, 0x8DD6C377},
        {0x8012F2F0, 0x8524474D, 0xF68F4D03},
        {0x8012F390, 0x1E914383, 0xC92D33C2},
        {0x8012F474, 0xF5D03D03, 0x26936131},
        {0x8012F484, 0x8E8C861, 0x7A2646A9},
        {0x8012F52C, 0xCBCD0D15, 0x9D80D08B},
        {0x8012F604, 0x2AB2CC1E, 0x1F43BDE0},
        {0x8012F614, 0x56D86EB, 0x5DA46F88},
        {0x8012F634, 0x52E71ED, 0x5DA46F88},
        {0x8012F72C, 0xD34D1348, 0x9E00F743},
        {0x8012F7A8, 0x9916B1FD, 0xE808ADBA},
        {0x8012F7B8, 0x407FFFD8, 0x7A19E7CF},
        {0x8012F7C8, 0x361F16B4, 0x819270B5},
        {0x8012FA5C, 0xAAA99467, 0x2E6744DB},
        {0x8012FDB4, 0xB7300F54, 0x11BC84},
        {0x80130044, 0x949BBE40, 0xD356249F},
        {0x80130134, 0x49C04791, 0x92A2F226},
        {0x8013017C, 0xE9EC2E8, 0x48679A58},
        {0x801302BC, 0x482B9E30, 0xE5705648},
        {0x801302E8, 0x5EF81DFD, 0x268D483A},
        {0x80130314, 0x79D7DAB3, 0x800C73E2},
        {0x80130374, 0x859A9811, 0x2C3D6A51},
        {0x80130398, 0x68C6DBF6, 0x9911B6D1},
        {0x80130414, 0x82BCF546, 0xB3FC2FB9},
        {0x80130424, 0x4C7B2A17, 0xE0BBE269},
        {0x80130434, 0xCECB4082, 0x477A93DC},
        {0x80130488, 0xFF4231FB, 0x1F2CCCBA},
        {0x80130750, 0xAF9589CA, 0x883B3860},
        {0x80130A50, 0x92D57C76, 0xF83E4411},
        {0x80130ACC, 0x45E8264A, 0x17333AFE},
        {0x80130AF0, 0xDB045968, 0x17333AFE},
        {0x80130CF8, 0x305A8E2B, 0xD356249F},
        {0x80130D58, 0xF88846A2, 0x48679A58},
        {0x80130DAC, 0x92087F87, 0xF69B7B42},
        {0x80130DE0, 0xBC30AB3B, 0x1784E333},
        {0x80130E88, 0xDBC5CE74, 0x1784E333},
        {0x80130E98, 0x95BAA54B, 0x6D7FBBF9},
        {0x80130EC4, 0x9E4A0272, 0x5AD94A22},
        {0x80130EDC, 0x84A26242, 0xB55A7E5B},
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
