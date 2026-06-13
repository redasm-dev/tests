#include "rdtest.h"
#include "rdtest_helpers.h"

// --- roundtrip LE ---
static int test_roundtrip_u8(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rdtest_assert(rd_write_byte(ctx, 0x0, 0xAB), "write failed");
    u8 v;
    rdtest_assert(rd_read_byte(ctx, 0x0, &v), "read failed");
    rdtest_assert_eq(v, 0xAB);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_roundtrip_le16(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rdtest_assert(rd_write_le16(ctx, 0x0, 0x1234), "write failed");
    u16 v;
    rdtest_assert(rd_read_le16(ctx, 0x0, &v), "read failed");
    rdtest_assert_eq(v, 0x1234);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_roundtrip_le32(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rdtest_assert(rd_write_le32(ctx, 0x0, 0x12345678), "write failed");
    u32 v;
    rdtest_assert(rd_read_le32(ctx, 0x0, &v), "read failed");
    rdtest_assert_eq(v, 0x12345678);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_roundtrip_le64(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rdtest_assert(rd_write_le64(ctx, 0x0, 0x123456789ABCDEF0ULL),
                  "write failed");
    u64 v;
    rdtest_assert(rd_read_le64(ctx, 0x0, &v), "read failed");
    rdtest_assert_eq(v, 0x123456789ABCDEF0ULL);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// --- roundtrip BE ---

static int test_roundtrip_be16(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rdtest_assert(rd_write_be16(ctx, 0x0, 0x1234), "write failed");
    u16 v;
    rdtest_assert(rd_read_be16(ctx, 0x0, &v), "read failed");
    rdtest_assert_eq(v, 0x1234);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_roundtrip_be32(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rdtest_assert(rd_write_be32(ctx, 0x0, 0x12345678), "write failed");
    u32 v;
    rdtest_assert(rd_read_be32(ctx, 0x0, &v), "read failed");
    rdtest_assert_eq(v, 0x12345678);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_roundtrip_be64(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rdtest_assert(rd_write_be64(ctx, 0x0, 0x123456789ABCDEF0ULL),
                  "write failed");
    u64 v;
    rdtest_assert(rd_read_be64(ctx, 0x0, &v), "read failed");
    rdtest_assert_eq(v, 0x123456789ABCDEF0ULL);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// --- endianness correctness ---

static int test_le_vs_be_32(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rdtest_assert(rd_write_le32(ctx, 0x0, 0x12345678), "write failed");

    u32 le, be;
    rdtest_assert(rd_read_le32(ctx, 0x0, &le), "le read failed");
    rdtest_assert(rd_read_be32(ctx, 0x0, &be), "be read failed");
    rdtest_assert_ne(le, be);
    rdtest_assert_eq(le, 0x12345678);
    rdtest_assert_eq(be, 0x78563412);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// --- boundary ---

static int test_read_last_byte(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rdtest_assert(rd_write_byte(ctx, RDTEST_BUFFER_SIZE - 1, 0xFF),
                  "write at last byte failed");
    u8 v;
    rdtest_assert(rd_read_byte(ctx, RDTEST_BUFFER_SIZE - 1, &v),
                  "read at last byte failed");
    rdtest_assert_eq(v, 0xFF);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_oob_read(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    u8 v;
    rdtest_assert(!rd_read_byte(ctx, RDTEST_BUFFER_SIZE, &v),
                  "oob read should fail");

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_oob_write(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rdtest_assert(!rd_write_byte(ctx, RDTEST_BUFFER_SIZE, 0xFF),
                  "oob write should fail");

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// --- expect_* ---

static int test_expect_match(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rd_write_le32(ctx, 0x0, 0x12345678);
    rdtest_assert(rd_expect_le32(ctx, 0x0, 0x12345678), "expect match failed");

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_expect_mismatch(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    rd_write_le32(ctx, 0x0, 0x12345678);
    rdtest_assert(!rd_expect_le32(ctx, 0x0, 0xDEADBEEF),
                  "expect mismatch should fail");

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// --- no segment ---

static int test_no_segment(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // address outside the mapped segment should fail
    u8 v;
    rdtest_assert(!rd_read_byte(ctx, 0xFFFFFFFF, &v),
                  "read outside segment should fail");
    rdtest_assert(!rd_write_byte(ctx, 0xFFFFFFFF, 0xFF),
                  "write outside segment should fail");

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static const RDTest K_TESTS[] = {
    {"roundtrip_u8", test_roundtrip_u8},
    {"roundtrip_le16", test_roundtrip_le16},
    {"roundtrip_le32", test_roundtrip_le32},
    {"roundtrip_le64", test_roundtrip_le64},
    {"roundtrip_be16", test_roundtrip_be16},
    {"roundtrip_be32", test_roundtrip_be32},
    {"roundtrip_be64", test_roundtrip_be64},
    {"le_vs_be_32", test_le_vs_be_32},
    {"read_last_byte", test_read_last_byte},
    {"oob_read", test_oob_read},
    {"oob_write", test_oob_write},
    {"expect_match", test_expect_match},
    {"expect_mismatch", test_expect_mismatch},
    {"no_segment", test_no_segment},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("memory", K_TESTS);
    rdtest_deinit();
    return result;
}
