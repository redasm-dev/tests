#include "rdtest.h"
#include "rdtest_helpers.h"

#define RD_TEST_X86_BASE_ADDRESS 0x401000

static const RDProcessorPlugin* x86_32 = NULL;

static bool _fails(const char* text) {
    const char* err;
    return rd_encode_instruction(text, RD_TEST_X86_BASE_ADDRESS, x86_32,
                                 &err) == NULL;
}

static bool _encode(const char* text, const u8* expected, usize n) {
    const RDScratchBuffer* buf =
        rd_encode_instruction(text, RD_TEST_X86_BASE_ADDRESS, x86_32, NULL);
    return buf && rd_scratch_length(buf) == n &&
           !memcmp(rd_scratch_data(buf), expected, n);
}

static int test_mnemonic_only(void) {
    rdtest_assert(_encode("ret", (u8[]){0xC3}, 1), "ret");
    rdtest_assert(_encode("leave", (u8[]){0xC9}, 1), "leave");
    return RDTEST_PASS;
}

static int test_reg_reg(void) {
    rdtest_assert(_encode("mov eax, ecx", (u8[]){0x89, 0xC8}, 2),
                  "mov eax,ecx");
    return RDTEST_PASS;
}

static int test_null_is_nop(void) {
    RDScratchBuffer* a = rd_scratch_create();
    RDScratchBuffer* b = rd_scratch_create();

    bool ok1 = rd_encode_instruction_to(NULL, RD_TEST_X86_BASE_ADDRESS, x86_32,
                                        a, NULL);
    bool ok2 = rd_encode_instruction_to("nop", RD_TEST_X86_BASE_ADDRESS, x86_32,
                                        b, NULL);

    rdtest_assert(ok1 && ok2, "both should succeed");
    rdtest_assert_eq(rd_scratch_length(a), rd_scratch_length(b));
    rdtest_assert(
        !memcmp(rd_scratch_data(a), rd_scratch_data(b), rd_scratch_length(a)),
        "NULL must equal explicit 'nop'");

    rd_scratch_destroy(a);
    rd_scratch_destroy(b);
    return RDTEST_PASS;
}

static int test_default_hex(void) {
    // x86 sets default_base=16, bare "10" means 0x10, not decimal 10
    rdtest_assert(_encode("add eax, 10", (u8[]){0x83, 0xC0, 0x10}, 3),
                  "bare = hex");
    rdtest_assert(_encode("add eax, 0n10", (u8[]){0x83, 0xC0, 0x0A}, 3),
                  "0n = decimal");
    return RDTEST_PASS;
}

static int test_displ_plus(void) {
    rdtest_assert(_encode("mov [eax+4], ecx", (u8[]){0x89, 0x48, 0x04}, 3),
                  "base+disp");
    return RDTEST_PASS;
}

static int test_displ_minus(void) {
    // isolates the MINUS branch specifically, the inverted check rejected
    // both signs identically, so a passing '+' test alone wouldn't catch this
    rdtest_assert(_encode("mov [eax-4], ecx", (u8[]){0x89, 0x48, 0xFC}, 3),
                  "base-disp");
    return RDTEST_PASS;
}

static int test_displ_scaled_index(void) {
    rdtest_assert(_encode("mov [eax+ecx*4], edx", (u8[]){0x89, 0x14, 0x88}, 3),
                  "SIB");
    return RDTEST_PASS;
}

static int test_displ_no_base(void) {
    // the original bug report that started the size-inference discussion
    rdtest_assert(_fails("mov [0n1000000], eax") == false, "should encode");
    return RDTEST_PASS;
}

static int test_esp_cannot_be_index(void) {
    rdtest_assert(_fails("mov [ecx+esp], eax"),
                  "ESP as index must be rejected");
    return RDTEST_PASS;
}

static int test_ambiguous_size_rejected(void) {
    rdtest_assert(_fails("mov [eax], 5"),
                  "no register sibling, no ptr keyword");
    return RDTEST_PASS;
}

static int test_explicit_size_accepted(void) {
    rdtest_assert(!_fails("mov dword ptr [eax], 5"),
                  "explicit ptr resolves ambiguity");
    return RDTEST_PASS;
}

static int test_unknown_mnemonic(void) {
    rdtest_assert(_fails("xyzzy eax, ecx"), "unknown mnemonic");
    return RDTEST_PASS;
}

static int test_malformed_number(void) {
    rdtest_assert(_fails("mov eax, 3zzz"), "3zzz must not silently split");
    return RDTEST_PASS;
}

static int test_dangling_comma(void) {
    rdtest_assert(_fails("mov eax, ecx,"), "trailing comma with no operand");
    return RDTEST_PASS;
}

static int test_trailing_garbage(void) {
    rdtest_assert(_fails("mov eax, ecx foo"),
                  "no comma, unexpected token after");
    return RDTEST_PASS;
}

static const RDTest TESTS[] = {
    {"mnemonic_only", test_mnemonic_only},
    {"reg_reg", test_reg_reg},
    {"null_is_nop", test_null_is_nop},
    {"default_hex", test_default_hex},
    {"displ_plus", test_displ_plus},
    {"displ_minus", test_displ_minus},
    {"displ_scaled_index", test_displ_scaled_index},
    {"displ_no_base", test_displ_no_base},
    {"esp_cannot_be_index", test_esp_cannot_be_index},
    {"ambiguous_size_rejected", test_ambiguous_size_rejected},
    {"explicit_size_accepted", test_explicit_size_accepted},
    {"unknown_mnemonic", test_unknown_mnemonic},
    {"malformed_number", test_malformed_number},
    {"dangling_comma", test_dangling_comma},
    {"trailing_garbage", test_trailing_garbage},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);

    x86_32 = rd_processor_find("x86_32");
    rdtest_assert_notnull(x86_32);

    int result = rdtest_run("x86_encode", TESTS);
    rdtest_deinit();

    return result;
}
