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

    RDTestSample s = {
        .rel_path = "mz/pe/CM01.exe",
        .loader_id = "win_pe",
        .processor_id = "x86_32",
        .entry_point = {.value = 0x00401000, .has_value = true},
        .names = NAMES,
        .types = TYPES,
        .graphs = GRAPHS,
        .xrefs = XREFS,
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
