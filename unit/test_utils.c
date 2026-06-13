#include "rdtest.h"
#include <redasm/redasm.h>

// --- rd_format ---
static int test_format_basic(void) {
    rdtest_assert_str(rd_format("hello"), "hello");
    return RDTEST_PASS;
}

static int test_format_string_arg(void) {
    rdtest_assert_str(rd_format("%s", "world"), "world");
    return RDTEST_PASS;
}

static int test_format_multiple_args(void) {
    rdtest_assert_str(rd_format("%s_%d", "test", 42), "test_42");
    return RDTEST_PASS;
}

// rd_format uses a single shared buffer, second call overwrites the first.
// This test documents the intended behavior, not a bug.
static int test_format_shared_buffer(void) {
    const char* a = rd_format("hello");
    const char* b = rd_format("world");
    rdtest_assert_str(a, "world"); // a and b point to the same buffer
    rdtest_assert_str(b, "world");
    return RDTEST_PASS;
}

// --- rd_to_hex ---

static int test_to_hex_zero(void) {
    rdtest_assert_str(rd_to_hex(0), "0");
    return RDTEST_PASS;
}

static int test_to_hex_byte(void) {
    rdtest_assert_str(rd_to_hex(0xFF), "ff");
    return RDTEST_PASS;
}

static int test_to_hex_word(void) {
    rdtest_assert_str(rd_to_hex(0x1234), "1234");
    return RDTEST_PASS;
}

static int test_to_hex_no_prefix(void) {
    // rd_to_hex must not produce "0x" prefix
    const char* s = rd_to_hex(0xAB);
    rdtest_assert(s[0] != '0' || s[1] != 'x', "unexpected 0x prefix");
    return RDTEST_PASS;
}

// --- rd_to_dec ---

static int test_to_dec_zero(void) {
    rdtest_assert_str(rd_to_dec(0), "0");
    return RDTEST_PASS;
}

static int test_to_dec_positive(void) {
    rdtest_assert_str(rd_to_dec(42), "42");
    return RDTEST_PASS;
}

static int test_to_dec_large(void) {
    rdtest_assert_str(rd_to_dec(1000000), "1000000");
    return RDTEST_PASS;
}

// --- rd_align_up ---

static int test_align_up_already_aligned(void) {
    rdtest_assert_eq(rd_align_up(16, 16), 16);
    rdtest_assert_eq(rd_align_up(256, 4), 256);
    return RDTEST_PASS;
}

static int test_align_up_unaligned(void) {
    rdtest_assert_eq(rd_align_up(1, 4), 4);
    rdtest_assert_eq(rd_align_up(5, 4), 8);
    rdtest_assert_eq(rd_align_up(13, 8), 16);
    return RDTEST_PASS;
}

static int test_align_up_zero(void) {
    rdtest_assert_eq(rd_align_up(0, 4), 0);
    return RDTEST_PASS;
}

// --- rd_stricmp / rd_strnicmp ---
static int test_stricmp_equal(void) {
    rdtest_assert_eq(rd_stricmp("hello", "hello"), 0);
    return RDTEST_PASS;
}

static int test_stricmp_case_insensitive(void) {
    rdtest_assert_eq(rd_stricmp("Hello", "hello"), 0);
    rdtest_assert_eq(rd_stricmp("HELLO", "hello"), 0);
    return RDTEST_PASS;
}

static int test_stricmp_different(void) {
    rdtest_assert_ne(rd_stricmp("hello", "world"), 0);
    return RDTEST_PASS;
}

static int test_strnicmp_partial(void) {
    rdtest_assert_eq(rd_strnicmp("hello_world", "hello_xyz", 5), 0);
    rdtest_assert_ne(rd_strnicmp("hello_world", "hello_xyz", 7), 0);
    return RDTEST_PASS;
}

static const RDTest K_TESTS[] = {
    {"format_basic", test_format_basic},
    {"format_string_arg", test_format_string_arg},
    {"format_multiple_args", test_format_multiple_args},
    {"format_shared_buffer", test_format_shared_buffer},
    {"to_hex_zero", test_to_hex_zero},
    {"to_hex_byte", test_to_hex_byte},
    {"to_hex_word", test_to_hex_word},
    {"to_hex_no_prefix", test_to_hex_no_prefix},
    {"to_dec_zero", test_to_dec_zero},
    {"to_dec_positive", test_to_dec_positive},
    {"to_dec_large", test_to_dec_large},
    {"align_up_already_aligned", test_align_up_already_aligned},
    {"align_up_unaligned", test_align_up_unaligned},
    {"align_up_zero", test_align_up_zero},
    {"stricmp_equal", test_stricmp_equal},
    {"stricmp_case_insensitive", test_stricmp_case_insensitive},
    {"stricmp_different", test_stricmp_different},
    {"strnicmp_partial", test_strnicmp_partial},
    {NULL, NULL},
};

int main(void) { return rdtest_run("utils", K_TESTS); }
