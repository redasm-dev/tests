#include "rdtest.h"
#include "rdtest_helpers.h"
#include <redasm/redasm.h>

static int test_vb5crkme(void) {
    static const RDTestName NAMES[] = {
        {0x4010C0, "imp___vbaExitProc"},
        {0x4010C6, "imp___vbaFreeVarList"},
        {0x4010CC, "imp___vbaVarDup"},
        {0x4010D2, "imp_rtcMsgBox"},
        {0x4010D8, "imp___vbaFreeObj"},
        {0x4010DE, "imp___vbaFreeStr"},
        {0x4010E4, "imp___vbaHresultCheckObj"},
        {0x4010EA, "imp___vbaObjSet"},
        {0x4010F0, "imp___vbaR8Str"},
        {0x4010F6, "imp___vbaOnError"},
        {0x4010FC, "imp_EVENT_SINK_QueryInterface"},
        {0x401102, "imp_EVENT_SINK_AddRef"},
        {0x401108, "imp_EVENT_SINK_Release"},
        {0x40110E, "imp_ThunRTMain"},
        {0x4020C4, "Form1_Command1_Click"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x40172c, {.name = "PE_VB_HEADER"}},
        {0x401eb8, {.name = "char16", .count = 29}},
        {0x401ef8, {.name = "char16", .count = 6}},
        {0x401f08, {.name = "char16", .count = 27}},
        {0x401f44, {.name = "char16", .count = 10}},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x4010C0, 0xF678B72E},
        {0x4010C6, 0xCDB60DD9},
        {0x4010CC, 0x677DA236},
        {0x4010D2, 0x1A2C60EC},
        {0x4010D8, 0x607CDE03},
        {0x4010DE, 0x413CDB49},
        {0x4010E4, 0xE8249059},
        {0x4010EA, 0xFAF92AEC},
        {0x4010F0, 0xAD8E7E15},
        {0x4010F6, 0x7722C787},
        {0x4010FC, 0x544E083C},
        {0x401102, 0x3414FD27},
        {0x401108, 0x606E5ED9},
        {0x40110E, 0xBFE79D98},
        {0x401114, 0xDAC04E08},
        {0x4020C4, 0xE2A60DFB},
        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x40172c, {.address = 0x401114, .type = RD_DR_ADDRESS}},
        {0x401eb8, {.address = 0x4021fd, .type = RD_DR_ADDRESS}},
        {0x401f08, {.address = 0x4021c2, .type = RD_DR_ADDRESS}},
        {0x401f44, {.address = 0x4021a4, .type = RD_DR_ADDRESS}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x401114},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040C4, .module = "MSVBVM50.DLL", .name = "_CIcos"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040C8, .module = "MSVBVM50.DLL", .name = "_adj_fptan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040CC, .module = "MSVBVM50.DLL", .name = "__vbaFreeVarList"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040D0, .module = "MSVBVM50.DLL", .name = "_adj_fdiv_m64"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040D4, .module = "MSVBVM50.DLL", .name = "_adj_fprem1"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040D8, .module = "MSVBVM50.DLL", .name = "__vbaHresultCheckObj"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040DC, .module = "MSVBVM50.DLL", .name = "_adj_fdiv_m32"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040E0, .module = "MSVBVM50.DLL", .name = "__vbaExitProc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040E4, .module = "MSVBVM50.DLL", .name = "__vbaOnError"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040E8, .module = "MSVBVM50.DLL", .name = "__vbaObjSet"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040EC, .module = "MSVBVM50.DLL", .name = "rtcMsgBox", .ordinal = {.has_value = true, .value = 595}},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040F0, .module = "MSVBVM50.DLL", .name = "_adj_fdiv_m16i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040F4, .module = "MSVBVM50.DLL", .name = "_adj_fdivr_m16i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040F8, .module = "MSVBVM50.DLL", .name = "_CIsin"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040FC, .module = "MSVBVM50.DLL", .name = "__vbaChkstk"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404100, .module = "MSVBVM50.DLL", .name = "EVENT_SINK_AddRef"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404104, .module = "MSVBVM50.DLL", .name = "_adj_fpatan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404108, .module = "MSVBVM50.DLL", .name = "EVENT_SINK_Release"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40410C, .module = "MSVBVM50.DLL", .name = "_CIsqrt"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404110, .module = "MSVBVM50.DLL", .name = "EVENT_SINK_QueryInterface"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404114, .module = "MSVBVM50.DLL", .name = "__vbaExceptHandler"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404118, .module = "MSVBVM50.DLL", .name = "_adj_fprem"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40411C, .module = "MSVBVM50.DLL", .name = "_adj_fdivr_m64"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404120, .module = "MSVBVM50.DLL", .name = "__vbaFPException"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404124, .module = "MSVBVM50.DLL", .name = "_CIlog"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404128, .module = "MSVBVM50.DLL", .name = "__vbaR8Str"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40412C, .module = "MSVBVM50.DLL", .name = "_adj_fdiv_m32i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404130, .module = "MSVBVM50.DLL", .name = "_adj_fdivr_m32i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404134, .module = "MSVBVM50.DLL", .name = "_adj_fdivr_m32"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404138, .module = "MSVBVM50.DLL", .name = "_adj_fdiv_r"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40413C, .module = "MSVBVM50.DLL", .name = "ThunRTMain", .ordinal = {.has_value = true, .value = 100}},
        {.kind = RD_EXT_IMPORTED, .address = 0x404140, .module = "MSVBVM50.DLL", .name = "__vbaVarDup"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404144, .module = "MSVBVM50.DLL", .name = "_CIatan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404148, .module = "MSVBVM50.DLL", .name = "_allmul"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40414C, .module = "MSVBVM50.DLL", .name = "_CItan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404150, .module = "MSVBVM50.DLL", .name = "_CIexp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404154, .module = "MSVBVM50.DLL", .name = "__vbaFreeStr"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404158, .module = "MSVBVM50.DLL", .name = "__vbaFreeObj"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "mz/pe/VB5CRKME.EXE",
        .loader_id = "win_pe",
        .processor_id = "x86_32",
        .entry_point =
            {
                .value = 0x401114,
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

static int test_newbies12(void) {
    static const RDTestName NAMES[] = {
        {0x4012A0, "imp___vbaChkstk"},
        {0x4013D2, "imp_EVENT_SINK_QueryInterface"},
        {0x4013D8, "imp_EVENT_SINK_AddRef"},
        {0x4013DE, "imp_EVENT_SINK_Release"},
        {0x4013E4, "imp_ThunRTMain"},
        {0x403BB0, "main_about_Click"},
        {0x403D20, "main_about_GotFocus"},
        {0x403DE0, "main_about_LostFocus"},
        {0x403EA0, "main_register_Click"},
        {0x404970, "main_register_GotFocus"},
        {0x404A30, "main_register_LostFocus"},
        {0x404AF0, "main_sn_GotFocus"},
        {0x404BB0, "main_sn_LostFocus"},
        {0x404C70, "main_uname_GotFocus"},
        {0x404D30, "main_uname_LostFocus"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x401aa8, {.name = "PE_VB_HEADER"}},
        {0x402ad8, {.name = "char16", .count = 31}},
        {0x402b1c, {.name = "char16", .count = 5}},
        {0x402b2c, {.name = "char16", .count = 29}},
        {0x402b78, {.name = "char16", .count = 53}},
        {0x402be8, {.name = "char16", .count = 9}},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x4012A0, 0xF49CA19E},
        {0x4013D2, 0xEECA365A},
        {0x4013D8, 0x579F3A4A},
        {0x4013DE, 0x4D577BD9},
        {0x4013E4, 0x9EC7435},
        {0x4013EC, 0x37BF4D67},
        {0x403BB0, 0xC8915294},
        {0x403D20, 0x63F5FA7B},
        {0x403DE0, 0xDE042AC4},
        {0x403EA0, 0x7EB46E58},
        {0x404970, 0xBE15FEBA},
        {0x404A30, 0xE523881},
        {0x404AF0, 0x63E4B055},
        {0x404BB0, 0x15E92119},
        {0x404C70, 0x8D0680C0},
        {0x404D30, 0xC5F2F40B},
        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x401aa8, {.address = 0x4013ec, .type = RD_DR_ADDRESS}},
        {0x402ad8, {.address = 0x40401e, .type = RD_DR_ADDRESS}},
        {0x402b1c, {.address = 0x403ffb, .type = RD_DR_ADDRESS}},
        {0x402b1c, {.address = 0x40416f, .type = RD_DR_ADDRESS}},
        {0x402b2c, {.address = 0x404192, .type = RD_DR_ADDRESS}},
        {0x402b2c, {.address = 0x404192, .type = RD_DR_ADDRESS}},
        {0x402b78, {.address = 0x404884, .type = RD_DR_ADDRESS}},
        {0x402be8, {.address = 0x404861, .type = RD_DR_ADDRESS}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x4013ec},
        {.kind = RD_EXT_IMPORTED, .address = 0x401000, .module = "MSVBVM60.DLL", .name = "__vbaStrI2"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401004, .module = "MSVBVM60.DLL", .name = "_CIcos"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401008, .module = "MSVBVM60.DLL", .name = "_adj_fptan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40100C, .module = "MSVBVM60.DLL", .name = "__vbaStrI4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401010, .module = "MSVBVM60.DLL", .name = "__vbaLenBstr"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401014, .module = "MSVBVM60.DLL", .name = "__vbaFreeVarList"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401018, .module = "MSVBVM60.DLL", .name = "_adj_fdiv_m64"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40101C, .module = "MSVBVM60.DLL", .name = "rtcAnsiValueBstr", .ordinal = {.has_value = true, .value = 516}},
        {.kind = RD_EXT_IMPORTED, .address = 0x401020, .module = "MSVBVM60.DLL", .name = "_adj_fprem1"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401024, .module = "MSVBVM60.DLL", .name = "__vbaStrCat"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401028, .module = "MSVBVM60.DLL", .name = "__vbaSetSystemError"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40102C, .module = "MSVBVM60.DLL", .name = "__vbaHresultCheckObj"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401030, .module = "MSVBVM60.DLL", .name = "_adj_fdiv_m32"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401034, .module = "MSVBVM60.DLL", .name = "__vbaObjSet"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401038, .module = "MSVBVM60.DLL", .name = "__vbaOnError"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40103C, .module = "MSVBVM60.dll", .name = "rtcMsgBox", .ordinal = {.has_value = true, .value = 595}},
        {.kind = RD_EXT_IMPORTED, .address = 0x401040, .module = "MSVBVM60.DLL", .name = "_adj_fdiv_m16i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401044, .module = "MSVBVM60.DLL", .name = "__vbaObjSetAddref"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401048, .module = "MSVBVM60.DLL", .name = "_adj_fdivr_m16i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40104C, .module = "MSVBVM60.DLL", .name = "_CIsin"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401050, .module = "MSVBVM60.DLL", .name = "rtcMidCharVar", .ordinal = {.has_value = true, .value = 632}},
        {.kind = RD_EXT_IMPORTED, .address = 0x401054, .module = "MSVBVM60.DLL", .name = "__vbaChkstk"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401058, .module = "MSVBVM60.DLL", .name = "EVENT_SINK_AddRef"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40105C, .module = "MSVBVM60.DLL", .name = "__vbaStrCmp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401060, .module = "MSVBVM60.DLL", .name = "__vbaI2I4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401064, .module = "MSVBVM60.DLL", .name = "DllFunctionCall"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401068, .module = "MSVBVM60.DLL", .name = "_adj_fpatan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40106C, .module = "MSVBVM60.DLL", .name = "EVENT_SINK_Release"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401070, .module = "MSVBVM60.DLL", .name = "__vbaUI1I2"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401074, .module = "MSVBVM60.DLL", .name = "_CIsqrt"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401078, .module = "MSVBVM60.DLL", .name = "EVENT_SINK_QueryInterface"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40107C, .module = "MSVBVM60.DLL", .name = "__vbaExceptHandler"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401080, .module = "MSVBVM60.DLL", .name = "_adj_fprem"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401084, .module = "MSVBVM60.DLL", .name = "_adj_fdivr_m64"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401088, .module = "MSVBVM60.DLL", .name = "__vbaFPException"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40108C, .module = "MSVBVM60.DLL", .name = "__vbaStrVarVal"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401090, .module = "MSVBVM60.DLL", .name = "_CIlog"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401094, .module = "MSVBVM60.DLL", .name = "__vbaErrorOverflow"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401098, .module = "MSVBVM60.DLL", .name = "__vbaNew2"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40109C, .module = "MSVBVM60.DLL", .name = "_adj_fdiv_m32i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010A0, .module = "MSVBVM60.DLL", .name = "_adj_fdivr_m32i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010A4, .module = "MSVBVM60.DLL", .name = "__vbaI4Str"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010A8, .module = "MSVBVM60.DLL", .name = "__vbaFreeStrList"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010AC, .module = "MSVBVM60.DLL", .name = "_adj_fdivr_m32"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010B0, .module = "MSVBVM60.DLL", .name = "_adj_fdiv_r"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010B4, .module = "MSVBVM60.DLL", .name = "ThunRTMain", .ordinal = {.has_value = true, .value = 100}},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010B8, .module = "MSVBVM60.DLL", .name = "__vbaStrToAnsi"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010BC, .module = "MSVBVM60.DLL", .name = "__vbaVarDup"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010C0, .module = "MSVBVM60.DLL", .name = "_CIatan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010C4, .module = "MSVBVM60.DLL", .name = "__vbaStrMove"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010C8, .module = "MSVBVM60.DLL", .name = "_allmul"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010CC, .module = "MSVBVM60.DLL", .name = "_CItan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010D0, .module = "MSVBVM60.DLL", .name = "_CIexp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010D4, .module = "MSVBVM60.DLL", .name = "__vbaFreeObj"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010D8, .module = "MSVBVM60.DLL", .name = "__vbaFreeStr"},
        {0},
    };

    RDTestSample s = {
        .rel_path = "mz/pe/newbies12.exe",
        .loader_id = "win_pe",
        .processor_id = "x86_32",
        .entry_point =
            {
                .value = 0x4013ec,
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

static const RDTest K_TESTS[] = {
    {"test_vb5crkme", test_vb5crkme},
    {"test_newbies12", test_newbies12},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("vb", K_TESTS);
    rdtest_deinit();
    return result;
}
