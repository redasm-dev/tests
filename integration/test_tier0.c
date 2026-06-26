#include "rdtest.h"
#include "rdtest_helpers.h"
#include <redasm/redasm.h>

static int test_masm0(void) {
    static const RDTestName NAMES[] = {
        {0x00401052, "imp_ExitProcess"},
        {0x00401058, "imp_GetModuleHandleA"},
        {0x0040105e, "imp_DialogBoxParamA"},
        {0x00401064, "imp_EndDialog"},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x401000, 0x742A23D4}, {0x401052, 0x158F87D0}, {0x401058, 0x7586538D},
        {0x40105E, 0xDA83F70A}, {0x401064, 0x4435A896}, {0},
    };

    static const RDTestXRef XREFS[] = {
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
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_cm01(void) {
    static const RDTestName NAMES[] = {
        {0x00401128, "WndProc"},
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
        {0x004020d6, {.name = "char", .count = 17}},
        {0x004020e7, {.name = "char", .count = 13}},
        {0x004020f4, {.name = "char", .count = 28}},
        {0x00402110, {.name = "char", .count = 5}},
        {0x00402115, {.name = "char", .count = 10}},
        {0x0040211f, {.name = "char", .count = 10}},
        {0x00402129, {.name = "char", .count = 11}},
        {0x00402134, {.name = "char", .count = 44}},
        {0x00402160, {.name = "char", .count = 9}},
        {0x00402169, {.name = "char", .count = 21}},
        {0},
    };

    static const RDTestXRef XREFS[] = {
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
        {0x401000, 0xC5AD580},  {0x401128, 0x6A48841},  {0x40134D, 0x6BBED3B8},
        {0x401362, 0x244A59C4}, {0x40137E, 0x4DCDB5C4}, {0x4013C2, 0x75E71F5A},
        {0x4013D2, 0xFF469AA1}, {0x4013D8, 0x3C5B708D}, {0x40140A, 0x85EC0278},
        {0x401416, 0x5C22B6B9}, {0x401428, 0x7DC1ACCC}, {0x40143A, 0x2D247B5E},
        {0x401440, 0x742C9DCA}, {0x40144C, 0x3E2169AD}, {0x40145E, 0x355A7A51},
        {0x40146A, 0x9DE7FCD7}, {0x401482, 0xB1E8676C}, {0x401488, 0x455BC201},
        {0x401494, 0xF029A071}, {0x40149A, 0x2F9E47C0}, {0x4014A0, 0x72F18277},
        {0x4014A6, 0x600491D},  {0x4014BE, 0x2FD74DAB}, {0x4014D6, 0xD7CA7989},
        {0x401506, 0x3FCC1C11}, {0x401512, 0x3F29DE74}, {0},
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
        {0x10001000, 0xC05041AD},
        {0x10001008, 0x585FDEBF},
        {0x10001010, 0x7D417CBD},
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
    };

    rdtest_assert_pass(rdtest_check_sample(&s));

    return RDTEST_PASS;
}

static int test_com_hello(void) {
    static const RDTestType TYPES[] = {
        {0x108, {.name = "char", .count = 15}},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x100, 0xD985B6C8},
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
