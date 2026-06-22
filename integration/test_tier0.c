#include "rdtest.h"
#include "rdtest_helpers.h"
#include <redasm/redasm.h>

static int test_masm0(void) {
    static const RDTestName NAMES[] = {
        {0x00401052, "ExitProcess"},
        {0x00401058, "GetModuleHandleA"},
        {0x0040105e, "DialogBoxParamA"},
        {0x00401064, "EndDialog"},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x401000, 0xC0522CBB}, {0x401052, 0x33C21829}, {0x401058, 0xAA88DFAC},
        {0x40105E, 0x24436A10}, {0x401064, 0xAFD95992}, {0},
    };

    static const RDTestXRef XREFS[] = {
        {0x403000, {.address = 0x401007}},
        {0x403000, {.address = 0x401017}},
        {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x00401000},
        {.kind = RD_EXT_IMPORTED, .address = 0x00402000, .module = "KERNEL32.dll", .name = "__imp_GetModuleHandleA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x00402004, .module = "KERNEL32.dll", .name = "__imp_ExitProcess"},
        {.kind = RD_EXT_IMPORTED, .address = 0x0040200c, .module = "user32.dll", .name = "__imp_EndDialog"},
        {.kind = RD_EXT_IMPORTED, .address = 0x00402010, .module = "user32.dll", .name = "__imp_DialogBoxParamA"},
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
        {0x00401128, "WndProc"},          {0x0040140a, "LoadCursorA"},
        {0x00401416, "MessageBeep"},      {0x00401428, "LoadIconA"},
        {0x0040143a, "MessageBoxA"},      {0x00401440, "PostQuitMessage"},
        {0x0040144c, "InvalidateRect"},   {0x0040145e, "TranslateMessage"},
        {0x0040146a, "ShowWindow"},       {0x00401482, "UpdateWindow"},
        {0x00401488, "RegisterClassA"},   {0x00401494, "CreateWindowExA"},
        {0x0040149a, "DefWindowProcA"},   {0x004014a0, "DialogBoxParamA"},
        {0x004014a6, "DispatchMessageA"}, {0x004014be, "FindWindowA"},
        {0x004014d6, "GetMessageA"},      {0x00401506, "GetModuleHandleA"},
        {0x00401512, "ExitProcess"},      {0},
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
        {0x401000, 0x28D488BE}, {0x401128, 0x9C12887F}, {0x40134D, 0x18561A37},
        {0x401362, 0x1EF3CB9C}, {0x40137E, 0xF535A00F}, {0x4013C2, 0x87EEBF8B},
        {0x4013D2, 0xE6805074}, {0x4013D8, 0xDC4D5154}, {0x40140A, 0x7D95DFB5},
        {0x401416, 0xE447FA86}, {0x401428, 0xEBAB5026}, {0x40143A, 0x301C8152},
        {0x401440, 0xD673EA30}, {0x40144C, 0x47FB61AA}, {0x40145E, 0xD832F6F8},
        {0x40146A, 0xAE8AB381}, {0x401482, 0x41ECC50B}, {0x401488, 0x9C10F5FC},
        {0x401494, 0xD48A08CC}, {0x40149A, 0x2A91796B}, {0x4014A0, 0x47CA8780},
        {0x4014A6, 0x07315D6A}, {0x4014BE, 0xAB96A69F}, {0x4014D6, 0xE64AA736},
        {0x401506, 0x644DFC7D}, {0x401512, 0xB24509EA}, {0},
    };

    // clang-format off
    static const RDTestExternal EXTERNALS[] = {
        {.kind = RD_EXT_EXPORTED, .address = 0x00401000},
        {.kind = RD_EXT_EXPORTED, .address = 0x00401128, .name = "WndProc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403184, .module = "USER32.dll", .name = "__imp_KillTimer"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403188, .module = "USER32.dll", .name = "__imp_GetSystemMetrics"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40318C, .module = "USER32.dll", .name = "__imp_LoadCursorA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403190, .module = "USER32.dll", .name = "__imp_LoadAcceleratorsA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403194, .module = "USER32.dll", .name = "__imp_MessageBeep"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403198, .module = "USER32.dll", .name = "__imp_GetWindowRect"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40319C, .module = "USER32.dll", .name = "__imp_LoadStringA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031A0, .module = "USER32.dll", .name = "__imp_LoadIconA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031A4, .module = "USER32.dll", .name = "__imp_LoadBitmapA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031A8, .module = "USER32.dll", .name = "__imp_SetFocus"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031AC, .module = "USER32.dll", .name = "__imp_MessageBoxA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031B0, .module = "USER32.dll", .name = "__imp_PostQuitMessage"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031B4, .module = "USER32.dll", .name = "__imp_WinHelpA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031B8, .module = "USER32.dll", .name = "__imp_InvalidateRect"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031BC, .module = "USER32.dll", .name = "__imp_TranslateAcceleratorA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031C0, .module = "USER32.dll", .name = "__imp_MoveWindow"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031C4, .module = "USER32.dll", .name = "__imp_TranslateMessage"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031C8, .module = "USER32.dll", .name = "__imp_LoadMenuA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031CC, .module = "USER32.dll", .name = "__imp_ShowWindow"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031D0, .module = "USER32.dll", .name = "__imp_SendMessageA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031D4, .module = "USER32.dll", .name = "__imp_SetTimer"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031D8, .module = "USER32.dll", .name = "__imp_SetWindowPos"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031DC, .module = "USER32.dll", .name = "__imp_UpdateWindow"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031E0, .module = "USER32.dll", .name = "__imp_RegisterClassA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031E4, .module = "USER32.dll", .name = "__imp_BeginPaint"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031E8, .module = "USER32.dll", .name = "__imp_CreateWindowExA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031EC, .module = "USER32.dll", .name = "__imp_DefWindowProcA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031F0, .module = "USER32.dll", .name = "__imp_DialogBoxParamA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031F4, .module = "USER32.dll", .name = "__imp_DispatchMessageA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031F8, .module = "USER32.dll", .name = "__imp_DrawMenuBar"},
        {.kind = RD_EXT_IMPORTED, .address = 0x4031FC, .module = "USER32.dll", .name = "__imp_EndDialog"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403200, .module = "USER32.dll", .name = "__imp_EndPaint"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403204, .module = "USER32.dll", .name = "__imp_FindWindowA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403208, .module = "USER32.dll", .name = "__imp_GetDC"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40320C, .module = "USER32.dll", .name = "__imp_GetDlgItem"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403210, .module = "USER32.dll", .name = "__imp_GetDlgItemTextA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403214, .module = "USER32.dll", .name = "__imp_GetMessageA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40321C, .module = "KERNEL32.dll", .name = "__imp_GetLocalTime"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403220, .module = "KERNEL32.dll", .name = "__imp_OpenFile"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403224, .module = "KERNEL32.dll", .name = "__imp_GlobalFree"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403228, .module = "KERNEL32.dll", .name = "__imp_GlobalAlloc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40322C, .module = "KERNEL32.dll", .name = "__imp_lstrlen"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403230, .module = "KERNEL32.dll", .name = "__imp_CloseHandle"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403234, .module = "KERNEL32.dll", .name = "__imp_WriteFile"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403238, .module = "KERNEL32.dll", .name = "__imp_GetModuleHandleA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40323C, .module = "KERNEL32.dll", .name = "__imp_ReadFile"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403240, .module = "KERNEL32.dll", .name = "__imp_ExitProcess"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403248, .module = "COMCTL32.DLL", .name = "__imp_InitCommonControls"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40324C, .module = "COMCTL32.DLL", .name = "__imp_CreateToolbarEx"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403250, .module = "COMCTL32.DLL", .name = "__imp_CreateToolbar"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403258, .module = "GDI32.dll", .name = "__imp_TextOutA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40325C, .module = "GDI32.dll", .name = "__imp_StartPage"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403260, .module = "GDI32.dll", .name = "__imp_StartDocA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403264, .module = "GDI32.dll", .name = "__imp_GetTextMetricsA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403268, .module = "GDI32.dll", .name = "__imp_GetStockObject"},
        {.kind = RD_EXT_IMPORTED, .address = 0x40326C, .module = "GDI32.dll", .name = "__imp_EndPage"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403270, .module = "GDI32.dll", .name = "__imp_EndDoc"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403274, .module = "GDI32.dll", .name = "__imp_DeleteObject"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403278, .module = "GDI32.dll", .name = "__imp_DeleteDC"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403280, .module = "COMDLG32.dll", .name = "__imp_GetSaveFileNameA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403284, .module = "COMDLG32.dll", .name = "__imp_GetOpenFileNameA"},
        {.kind = RD_EXT_IMPORTED, .address = 0x403288, .module = "COMDLG32.dll", .name = "__imp_PrintDlgA"},
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

static int test_com_hello(void) {
    static const RDTestType TYPES[] = {
        {0x108, {.name = "char", .count = 15}},
        {0},
    };

    static const RDTestGraph GRAPHS[] = {
        {0x100, 0x1C51CF6D},
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
    {"test_com_hello", test_com_hello},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("tier0", K_TESTS);
    rdtest_deinit();
    return result;
}
