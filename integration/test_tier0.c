#include "rdtest.h"
#include "rdtest_helpers.h"
#include <redasm/redasm.h>

static int test_masm0(void) {
    static const RDTestName NAMES[] = {
        {0x00401029, "lpDialogFunc_401029", .proto = "DLGPROC"},
        {0x00401052, "imp_ExitProcess"},
        {0x00401058, "imp_GetModuleHandleA"},
        {0x0040105e, "imp_DialogBoxParamA"},
        {0x00401064, "imp_EndDialog"},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x401000, 0xE2FDF257, 0x41DC1CEA},
        {0x401029, 0x5819D20B, 0x247C1E97},
        {0x401052, 0x407C224D, 0x56606E7B},
        {0x401058, 0xD812978A, 0x4F259ACF},
        {0x40105E, 0xEAEC0144, 0xD65A15E5},
        {0x401064, 0x3AA5BADA, 0x247C1E97},
        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x401029, {.address = 0x40100e, .type = RD_DR_ADDRESS}},
        {0x403000, {.address = 0x401007}},
        {0x403000, {.address = 0x401017}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x00401000},
        {.kind = RD_EXT_IMPORTED, .address = 0x00402000, .module = "KERNEL32.dll", .name = "GetModuleHandleA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x00402004, .module = "KERNEL32.dll", .name = "ExitProcess"},
        {.kind = RD_EXT_IMPORTED, .address = 0x0040200c, .module = "user32.dll", .name = "EndDialog"},
        {.kind = RD_EXT_IMPORTED, .address = 0x00402010, .module = "user32.dll", .name = "DialogBoxParamA"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "mz/pe/Masm0.exe",
        .loader_id = "win_pe",
        .processor_id = "x86_32",
        .entry_point =
            {
                .value = 0x00401000,
                .has_value = true,
                .no_ret = true,
            },
        .names = NAMES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
        .externals = EXTERNALS,
        .n_functions = 6,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_cm01(void) {
    static const RDTestName NAMES[] = {
        {0x00401128, "WndProc"},
        {0x00401253, "lpDialogFunc_401253", .proto = "DLGPROC"},
        {0x0040130a, "lpDialogFunc_40130A", .proto = "DLGPROC"},
        {0x0040140a, "imp_LoadCursorA"},
        {0x00401416, "imp_MessageBeep"},
        {0x00401428, "imp_LoadIconA"},
        {0x0040143a, "imp_MessageBoxA"},
        {0x00401440, "imp_PostQuitMessage"},
        {0x0040144c, "imp_InvalidateRect"},
        {0x0040145e, "imp_TranslateMessage"},
        {0x0040146a, "imp_ShowWindow"},
        {0x00401482, "imp_UpdateWindow"},
        {0x00401488, "imp_RegisterClassA"},
        {0x00401494, "imp_CreateWindowExA"},
        {0x0040149a, "imp_DefWindowProcA"},
        {0x004014a0, "imp_DialogBoxParamA"},
        {0x004014a6, "imp_DispatchMessageA"},
        {0x004014be, "imp_FindWindowA"},
        {0x004014d6, "imp_GetMessageA"},
        {0x00401506, "imp_GetModuleHandleA"},
        {0x00401512, "imp_ExitProcess"},
        {0},
    };

    static const RDTestType TYPES[] = {
        {0x004020d6, .name = "char", .count = 17},
        {0x004020e7, .name = "char", .count = 13},
        {0x004020f4, .name = "char", .count = 28},
        {0x00402110, .name = "char", .count = 5},
        {0x00402115, .name = "char", .count = 10},
        {0x0040211f, .name = "char", .count = 10},
        {0x00402129, .name = "char", .count = 11},
        {0x00402134, .name = "char", .count = 44},
        {0x00402160, .name = "char", .count = 9},
        {0x00402169, .name = "char", .count = 21},
        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x40130a, {.address = 0x4011ef, .type = RD_DR_ADDRESS}},
        {0x401253, {.address = 0x40120b, .type = RD_DR_ADDRESS}},
        {0x4020ca, {.address = 0x401007}},
        {0x4020ca, {.address = 0x401045}},
        {0x4020ca, {.address = 0x401097}},
        {0x4020ca, {.address = 0x4011fc}},
        {0x4020ca, {.address = 0x401218}},
        {0x4020e7, {.address = 0x4010b7, .type = RD_DR_ADDRESS}},
        {0x4020f4, {.address = 0x40100e, .type = RD_DR_ADDRESS}},
        {0x4020f4, {.address = 0x401081, .type = RD_DR_ADDRESS}},
        {0x4020f4, {.address = 0x4010bc, .type = RD_DR_ADDRESS}},
        {0x402110, {.address = 0x401077, .type = RD_DR_ADDRESS}},
        {0x402115, {.address = 0x401213, .type = RD_DR_ADDRESS}},
        {0x40211f, {.address = 0x4011f7, .type = RD_DR_ADDRESS}},
        {0x402129, {.address = 0x40134f, .type = RD_DR_ADDRESS}},
        {0x402134, {.address = 0x401354, .type = RD_DR_ADDRESS}},
        {0x402160, {.address = 0x40136b, .type = RD_DR_ADDRESS}},
        {0x402160, {.address = 0x4013af, .type = RD_DR_ADDRESS}},
        {0x402169, {.address = 0x401370, .type = RD_DR_ADDRESS}},
        {0x402169, {.address = 0x4013b4, .type = RD_DR_ADDRESS}},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x401000, 0x7D6224DC, 0xC806A84E}, {0x401128, 0xA36A7322, 0xA8955050},
        {0x401253, 0x6A9ABA55, 0x1AA21DFC}, {0x40130A, 0xED18B883, 0xBFDE979B},
        {0x40134D, 0x71B2664F, 0xA554BD29}, {0x401362, 0x698EC7BF, 0xEF160F23},
        {0x40137E, 0xF5AA23CA, 0xA44B495A}, {0x4013C2, 0xA92656E6, 0xC0D4827D},
        {0x4013D2, 0x877A6A78, 0x8152F217}, {0x4013D8, 0xC5914EC1, 0xEB44CD01},
        {0x40140A, 0xE2264B77, 0xF81296},   {0x401416, 0x8CC3BFBB, 0xAE78E96E},
        {0x401428, 0x601ACDDD, 0x66BA6826}, {0x401434, 0x27876EAB, 0xBCA43EE5},
        {0x40143A, 0x6ADE80BB, 0xC1F2B37C}, {0x401440, 0x25B9A4D4, 0xAC2FE9EC},
        {0x40144C, 0x10852F01, 0x2B28FC1C}, {0x40145E, 0xDB4091C4, 0x736A737E},
        {0x40146A, 0x19E5AB28, 0x8184C749}, {0x401482, 0x97DA3327, 0xFBC86E47},
        {0x401488, 0x2655D1A7, 0xE2C58B0},  {0x401494, 0x87427FB3, 0x9EA0E0DE},
        {0x40149A, 0x152701C2, 0x1F9CC630}, {0x4014A0, 0x20EC3333, 0xD9325887},
        {0x4014A6, 0xE1795820, 0xDCD04FE9}, {0x4014B2, 0x630153EB, 0x264E5024},
        {0x4014BE, 0x60C4C67, 0xA1EC0CB8},  {0x4014D0, 0x871C856B, 0x1285DDF9},
        {0x4014D6, 0x8DBACE10, 0xFF90B9D6}, {0x401506, 0x9D622649, 0x4A169F42},
        {0x401512, 0x6DDE8770, 0x17E00347}, {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x00401000},
        {.kind = RD_EXT_EXPORTED, .address = 0x00401128, .name = "WndProc", .ordinal = { .has_value = true, .value = 1}},
        {.kind = RD_EXT_IMPORTED, .address = 0x403184, .module = "USER32.dll", .name = "KillTimer"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403188, .module = "USER32.dll", .name = "GetSystemMetrics"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40318C, .module = "USER32.dll", .name = "LoadCursorA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403190, .module = "USER32.dll", .name = "LoadAcceleratorsA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403194, .module = "USER32.dll", .name = "MessageBeep"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403198, .module = "USER32.dll", .name = "GetWindowRect"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40319C, .module = "USER32.dll", .name = "LoadStringA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031A0, .module = "USER32.dll", .name = "LoadIconA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031A4, .module = "USER32.dll", .name = "LoadBitmapA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031A8, .module = "USER32.dll", .name = "SetFocus"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031AC, .module = "USER32.dll", .name = "MessageBoxA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031B0, .module = "USER32.dll", .name = "PostQuitMessage"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031B4, .module = "USER32.dll", .name = "WinHelpA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031B8, .module = "USER32.dll", .name = "InvalidateRect"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031BC, .module = "USER32.dll", .name = "TranslateAcceleratorA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031C0, .module = "USER32.dll", .name = "MoveWindow"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031C4, .module = "USER32.dll", .name = "TranslateMessage"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031C8, .module = "USER32.dll", .name = "LoadMenuA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031CC, .module = "USER32.dll", .name = "ShowWindow"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031D0, .module = "USER32.dll", .name = "SendMessageA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031D4, .module = "USER32.dll", .name = "SetTimer"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031D8, .module = "USER32.dll", .name = "SetWindowPos"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031DC, .module = "USER32.dll", .name = "UpdateWindow"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031E0, .module = "USER32.dll", .name = "RegisterClassA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031E4, .module = "USER32.dll", .name = "BeginPaint"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031E8, .module = "USER32.dll", .name = "CreateWindowExA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031EC, .module = "USER32.dll", .name = "DefWindowProcA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031F0, .module = "USER32.dll", .name = "DialogBoxParamA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031F4, .module = "USER32.dll", .name = "DispatchMessageA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031F8, .module = "USER32.dll", .name = "DrawMenuBar"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031FC, .module = "USER32.dll", .name = "EndDialog"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403200, .module = "USER32.dll", .name = "EndPaint"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403204, .module = "USER32.dll", .name = "FindWindowA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403208, .module = "USER32.dll", .name = "GetDC"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40320C, .module = "USER32.dll", .name = "GetDlgItem"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403210, .module = "USER32.dll", .name = "GetDlgItemTextA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403214, .module = "USER32.dll", .name = "GetMessageA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40321C, .module = "KERNEL32.dll", .name = "GetLocalTime"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403220, .module = "KERNEL32.dll", .name = "OpenFile"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403224, .module = "KERNEL32.dll", .name = "GlobalFree"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403228, .module = "KERNEL32.dll", .name = "GlobalAlloc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40322C, .module = "KERNEL32.dll", .name = "lstrlen"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403230, .module = "KERNEL32.dll", .name = "CloseHandle"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403234, .module = "KERNEL32.dll", .name = "WriteFile"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403238, .module = "KERNEL32.dll", .name = "GetModuleHandleA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40323C, .module = "KERNEL32.dll", .name = "ReadFile"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403240, .module = "KERNEL32.dll", .name = "ExitProcess"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403248, .module = "COMCTL32.DLL", .name = "InitCommonControls"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40324C, .module = "COMCTL32.DLL", .name = "CreateToolbarEx"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403250, .module = "COMCTL32.DLL", .name = "CreateToolbar"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403258, .module = "GDI32.dll", .name = "TextOutA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40325C, .module = "GDI32.dll", .name = "StartPage"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403260, .module = "GDI32.dll", .name = "StartDocA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403264, .module = "GDI32.dll", .name = "GetTextMetricsA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403268, .module = "GDI32.dll", .name = "GetStockObject"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40326C, .module = "GDI32.dll", .name = "EndPage"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403270, .module = "GDI32.dll", .name = "EndDoc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403274, .module = "GDI32.dll", .name = "DeleteObject"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403278, .module = "GDI32.dll", .name = "DeleteDC"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403280, .module = "COMDLG32.dll", .name = "GetSaveFileNameA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403284, .module = "COMDLG32.dll", .name = "GetOpenFileNameA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403288, .module = "COMDLG32.dll", .name = "PrintDlgA"},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "mz/pe/CM01.exe",
        .loader_id = "win_pe",
        .processor_id = "x86_32",
        .entry_point = {.value = 0x00401000, .has_value = true},
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
        .externals = EXTERNALS,
        .n_functions = 31,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_testfwd(void) {
    static const RDTestName NAMES[] = {
        {0x10001000, "NormalExport"},
        {0x10001008, "exp_testfwd.dll_#102"},
        {0x10001010, "AnotherNormalExport"},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x10001000, 0xB3FFA57A, 0x20C06860},
        {0x10001008, 0x459D27C0, 0x60D1CD9E},
        {0x10001010, 0x68786030, 0xB190E754},
        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x403000, {.address = 0x401007}},
        {0x403000, {.address = 0x401017}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x10001000, .module = "testfwd.dll", .name = "NormalExport", .ordinal = { .has_value = true, .value = 100 }},
        {.kind = RD_EXT_EXPORTED, .address = 0x10001008, .module = "testfwd.dll", .ordinal = { .has_value = true, .value = 102 }},
        {.kind = RD_EXT_EXPORTED, .address = 0x10001010, .module = "testfwd.dll", .name = "AnotherNormalExport", .ordinal = { .has_value = true, .value = 104 }},
        {.kind = RD_EXT_EXPORTED, .address = 0x100020A4, .module = "testfwd.dll", .name = "ForwardedByName", .ordinal = { .has_value = true, .value = 101 }},
        {.kind = RD_EXT_EXPORTED, .address = 0x100020BC, .module = "testfwd.dll", .name = "ForwardedByOrdinal", .ordinal = { .has_value = true, .value = 103 }},
        {0},
    };
    // clang-format on

    RDTestSample s = {
        .rel_path = "mz/pe/testfwd.dll",
        .loader_id = "win_pe",
        .processor_id = "x86_32",
        .names = NAMES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
        .externals = EXTERNALS,
        .n_functions = 3,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_com_hello(void) {
    static const RDTestType TYPES[] = {
        {0x108, .name = "char", .count = 15},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x100, 0xBBF02CA8, 0xACB6C3BB},
        {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x108, {.address = 0x105}},
        {0},
    };

    RDTestSample s = {
        .rel_path = "mz/com/HELLO.COM",
        .loader_id = "dos_com",
        .processor_id = "x86_16_real",
        .entry_point = {.value = 0x100, .has_value = true},
        .types = TYPES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
        .n_functions = 1,
    };

    rdtest_assert_pass(rdtest_check_sample(&s));
    return RDTEST_PASS;
}

static const RDTest K_TESTS[] = {
    {"test_masm0", test_masm0},
    {"test_cm01", test_cm01},
    {"test_testfwd", test_testfwd},
    {"test_com_hello", test_com_hello},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("tier0", K_TESTS);
    rdtest_deinit();
    return result;
}
