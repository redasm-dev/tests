#include "rdtest.h"
#include "rdtest_helpers.h"
#include <redasm/redasm.h>

static int test_vb5crkme(void) {
    static const RDTestName NAMES[] = {
        {0x4010C0, "__vbaExitProc"},
        {0x4010C6, "__vbaFreeVarList"},
        {0x4010CC, "__vbaVarDup"},
        {0x4010D2, "rtcMsgBox"},
        {0x4010D8, "__vbaFreeObj"},
        {0x4010DE, "__vbaFreeStr"},
        {0x4010E4, "__vbaHresultCheckObj"},
        {0x4010EA, "__vbaObjSet"},
        {0x4010F0, "__vbaR8Str"},
        {0x4010F6, "__vbaOnError"},
        {0x4010FC, "EVENT_SINK_QueryInterface"},
        {0x401102, "EVENT_SINK_AddRef"},
        {0x401108, "EVENT_SINK_Release"},
        {0x40110E, "ThunRTMain"},
        {0x401114, "win_pe_entry_point_401114"},
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
        {0x4010C0, 0xEC439FC0},
        {0x4010C6, 0xE5D66CA3},
        {0x4010CC, 0xDCF3FF19},
        {0x4010D2, 0xD193A5BE},
        {0x4010D8, 0x48E1C1B8},
        {0x4010DE, 0x39E72A6C},
        {0x4010E4, 0x48FF582D},
        {0x4010EA, 0x3E79DCC6},
        {0x4010F0, 0x9F4D3297},
        {0x4010F6, 0x69AB3085},
        {0x4010FC, 0xBB05099F},
        {0x401102, 0x4FFAD929},
        {0x401108, 0x374925AB},
        {0x40110E, 0x9F3263DD},
        {0x401114, 0x3A478CFA},
        {0x4020C4, 0xE23C76F3},
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
        {.kind = RD_EXT_IMPORTED, .address = 0x4040C4, .module = "MSVBVM50.DLL", .name = "__imp__CIcos"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040C8, .module = "MSVBVM50.DLL", .name = "__imp__adj_fptan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040CC, .module = "MSVBVM50.DLL", .name = "__imp___vbaFreeVarList"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040D0, .module = "MSVBVM50.DLL", .name = "__imp__adj_fdiv_m64"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040D4, .module = "MSVBVM50.DLL", .name = "__imp__adj_fprem1"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040D8, .module = "MSVBVM50.DLL", .name = "__imp___vbaHresultCheckObj"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040DC, .module = "MSVBVM50.DLL", .name = "__imp__adj_fdiv_m32"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040E0, .module = "MSVBVM50.DLL", .name = "__imp___vbaExitProc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040E4, .module = "MSVBVM50.DLL", .name = "__imp___vbaOnError"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040E8, .module = "MSVBVM50.DLL", .name = "__imp___vbaObjSet"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040EC, .module = "MSVBVM50.DLL", .name = "__imp_rtcMsgBox", .ordinal = {.has_value = true, .value = 595}},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040F0, .module = "MSVBVM50.DLL", .name = "__imp__adj_fdiv_m16i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040F4, .module = "MSVBVM50.DLL", .name = "__imp__adj_fdivr_m16i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040F8, .module = "MSVBVM50.DLL", .name = "__imp__CIsin"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4040FC, .module = "MSVBVM50.DLL", .name = "__imp___vbaChkstk"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404100, .module = "MSVBVM50.DLL", .name = "__imp_EVENT_SINK_AddRef"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404104, .module = "MSVBVM50.DLL", .name = "__imp__adj_fpatan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404108, .module = "MSVBVM50.DLL", .name = "__imp_EVENT_SINK_Release"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40410C, .module = "MSVBVM50.DLL", .name = "__imp__CIsqrt"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404110, .module = "MSVBVM50.DLL", .name = "__imp_EVENT_SINK_QueryInterface"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404114, .module = "MSVBVM50.DLL", .name = "__imp___vbaExceptHandler"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404118, .module = "MSVBVM50.DLL", .name = "__imp__adj_fprem"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40411C, .module = "MSVBVM50.DLL", .name = "__imp__adj_fdivr_m64"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404120, .module = "MSVBVM50.DLL", .name = "__imp___vbaFPException"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404124, .module = "MSVBVM50.DLL", .name = "__imp__CIlog"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404128, .module = "MSVBVM50.DLL", .name = "__imp___vbaR8Str"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40412C, .module = "MSVBVM50.DLL", .name = "__imp__adj_fdiv_m32i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404130, .module = "MSVBVM50.DLL", .name = "__imp__adj_fdivr_m32i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404134, .module = "MSVBVM50.DLL", .name = "__imp__adj_fdivr_m32"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404138, .module = "MSVBVM50.DLL", .name = "__imp__adj_fdiv_r"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40413C, .module = "MSVBVM50.DLL", .name = "__imp_ThunRTMain", .ordinal = {.has_value = true, .value = 100}},
        {.kind = RD_EXT_IMPORTED, .address = 0x404140, .module = "MSVBVM50.DLL", .name = "__imp___vbaVarDup"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404144, .module = "MSVBVM50.DLL", .name = "__imp__CIatan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404148, .module = "MSVBVM50.DLL", .name = "__imp__allmul"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40414C, .module = "MSVBVM50.DLL", .name = "__imp__CItan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404150, .module = "MSVBVM50.DLL", .name = "__imp__CIexp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404154, .module = "MSVBVM50.DLL", .name = "__imp___vbaFreeStr"},
        {.kind = RD_EXT_IMPORTED, .address = 0x404158, .module = "MSVBVM50.DLL", .name = "__imp___vbaFreeObj"},
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
        {0x4012A0, "__vbaChkstk"},
        {0x4013D2, "EVENT_SINK_QueryInterface"},
        {0x4013D8, "EVENT_SINK_AddRef"},
        {0x4013DE, "EVENT_SINK_Release"},
        {0x4013E4, "ThunRTMain"},
        {0x4013EC, "win_pe_entry_point_4013ec"},
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
        {0x4012A0, 0xA3A70690},
        {0x4013D2, 0xC6E01298},
        {0x4013D8, 0xB0583F0A},
        {0x4013DE, 0x9A62DED3},
        {0x4013E4, 0x5169185C},
        {0x4013EC, 0x20C1BA73},
        {0x403BB0, 0x97EF31EB},
        {0x403D20, 0x7D743186},
        {0x403DE0, 0x5A729676},
        {0x403EA0, 0xF6C1A8D0},
        {0x404970, 0xE72E4A0E},
        {0x404A30, 0x483F7825},
        {0x404AF0, 0x8D3789F0},
        {0x404BB0, 0xCEE700D0},
        {0x404C70, 0x4F4C1076},
        {0x404D30, 0x933F8AE7},
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
        {.kind = RD_EXT_IMPORTED, .address = 0x401000, .module = "MSVBVM60.DLL", .name = "__imp___vbaStrI2"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401004, .module = "MSVBVM60.DLL", .name = "__imp__CIcos"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401008, .module = "MSVBVM60.DLL", .name = "__imp__adj_fptan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40100C, .module = "MSVBVM60.DLL", .name = "__imp___vbaStrI4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401010, .module = "MSVBVM60.DLL", .name = "__imp___vbaLenBstr"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401014, .module = "MSVBVM60.DLL", .name = "__imp___vbaFreeVarList"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401018, .module = "MSVBVM60.DLL", .name = "__imp__adj_fdiv_m64"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40101C, .module = "MSVBVM60.DLL", .name = "__imp_rtcAnsiValueBstr", .ordinal = {.has_value = true, .value = 516}},
        {.kind = RD_EXT_IMPORTED, .address = 0x401020, .module = "MSVBVM60.DLL", .name = "__imp__adj_fprem1"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401024, .module = "MSVBVM60.DLL", .name = "__imp___vbaStrCat"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401028, .module = "MSVBVM60.DLL", .name = "__imp___vbaSetSystemError"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40102C, .module = "MSVBVM60.DLL", .name = "__imp___vbaHresultCheckObj"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401030, .module = "MSVBVM60.DLL", .name = "__imp__adj_fdiv_m32"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401034, .module = "MSVBVM60.DLL", .name = "__imp___vbaObjSet"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401038, .module = "MSVBVM60.DLL", .name = "__imp___vbaOnError"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40103C, .module = "MSVBVM60.dll", .name = "__imp_rtcMsgBox", .ordinal = {.has_value = true, .value = 595}},
        {.kind = RD_EXT_IMPORTED, .address = 0x401040, .module = "MSVBVM60.DLL", .name = "__imp__adj_fdiv_m16i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401044, .module = "MSVBVM60.DLL", .name = "__imp___vbaObjSetAddref"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401048, .module = "MSVBVM60.DLL", .name = "__imp__adj_fdivr_m16i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40104C, .module = "MSVBVM60.DLL", .name = "__imp__CIsin"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401050, .module = "MSVBVM60.DLL", .name = "__imp_rtcMidCharVar", .ordinal = {.has_value = true, .value = 632}},
        {.kind = RD_EXT_IMPORTED, .address = 0x401054, .module = "MSVBVM60.DLL", .name = "__imp___vbaChkstk"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401058, .module = "MSVBVM60.DLL", .name = "__imp_EVENT_SINK_AddRef"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40105C, .module = "MSVBVM60.DLL", .name = "__imp___vbaStrCmp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401060, .module = "MSVBVM60.DLL", .name = "__imp___vbaI2I4"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401064, .module = "MSVBVM60.DLL", .name = "__imp_DllFunctionCall"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401068, .module = "MSVBVM60.DLL", .name = "__imp__adj_fpatan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40106C, .module = "MSVBVM60.DLL", .name = "__imp_EVENT_SINK_Release"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401070, .module = "MSVBVM60.DLL", .name = "__imp___vbaUI1I2"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401074, .module = "MSVBVM60.DLL", .name = "__imp__CIsqrt"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401078, .module = "MSVBVM60.DLL", .name = "__imp_EVENT_SINK_QueryInterface"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40107C, .module = "MSVBVM60.DLL", .name = "__imp___vbaExceptHandler"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401080, .module = "MSVBVM60.DLL", .name = "__imp__adj_fprem"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401084, .module = "MSVBVM60.DLL", .name = "__imp__adj_fdivr_m64"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401088, .module = "MSVBVM60.DLL", .name = "__imp___vbaFPException"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40108C, .module = "MSVBVM60.DLL", .name = "__imp___vbaStrVarVal"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401090, .module = "MSVBVM60.DLL", .name = "__imp__CIlog"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401094, .module = "MSVBVM60.DLL", .name = "__imp___vbaErrorOverflow"},
        {.kind = RD_EXT_IMPORTED, .address = 0x401098, .module = "MSVBVM60.DLL", .name = "__imp___vbaNew2"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40109C, .module = "MSVBVM60.DLL", .name = "__imp__adj_fdiv_m32i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010A0, .module = "MSVBVM60.DLL", .name = "__imp__adj_fdivr_m32i"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010A4, .module = "MSVBVM60.DLL", .name = "__imp___vbaI4Str"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010A8, .module = "MSVBVM60.DLL", .name = "__imp___vbaFreeStrList"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010AC, .module = "MSVBVM60.DLL", .name = "__imp__adj_fdivr_m32"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010B0, .module = "MSVBVM60.DLL", .name = "__imp__adj_fdiv_r"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010B4, .module = "MSVBVM60.DLL", .name = "__imp_ThunRTMain", .ordinal = {.has_value = true, .value = 100}},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010B8, .module = "MSVBVM60.DLL", .name = "__imp___vbaStrToAnsi"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010BC, .module = "MSVBVM60.DLL", .name = "__imp___vbaVarDup"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010C0, .module = "MSVBVM60.DLL", .name = "__imp__CIatan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010C4, .module = "MSVBVM60.DLL", .name = "__imp___vbaStrMove"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010C8, .module = "MSVBVM60.DLL", .name = "__imp__allmul"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010CC, .module = "MSVBVM60.DLL", .name = "__imp__CItan"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010D0, .module = "MSVBVM60.DLL", .name = "__imp__CIexp"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010D4, .module = "MSVBVM60.DLL", .name = "__imp___vbaFreeObj"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4010D8, .module = "MSVBVM60.DLL", .name = "__imp___vbaFreeStr"},
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
