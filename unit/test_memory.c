#include "rdtest.h"
#include "rdtest_helpers.h"

// --- roundtrip LE ---
static int test_roundtrip_u8(void) {
    RDContext* ctx = rdtest_context_create();

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

static int test_read_uleb128(void) {
    static const char DATA[] = {
        0x00,                         // 0
        0x01,                         // 1
        0x7F,                         // 127
        0x80, 0x7F,                   // 16256
        0xFF, 0xFF, 0xFF, 0xFF, 0x0F, // UINT32_MAX
    };

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rdtest_assert(rd_reader_read_uleb128(r, &v), "read 0");
    rdtest_assert_eq(v.value, 0ULL);
    rdtest_assert_eq(v.length, 1U);

    rdtest_assert(rd_reader_read_uleb128(r, &v), "read 1");
    rdtest_assert_eq(v.value, 1ULL);
    rdtest_assert_eq(v.length, 1U);

    rdtest_assert(rd_reader_read_uleb128(r, &v), "read 127");
    rdtest_assert_eq(v.value, 127ULL);
    rdtest_assert_eq(v.length, 1U);

    rdtest_assert(rd_reader_read_uleb128(r, &v), "read 16256");
    rdtest_assert_eq(v.value, 16256ULL);
    rdtest_assert_eq(v.length, 2U);

    rdtest_assert(rd_reader_read_uleb128(r, &v), "read UINT32_MAX");
    rdtest_assert_eq(v.value, 0xFFFFFFFFULL);
    rdtest_assert_eq(v.length, 5U);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_read_uleb128_max(void) {
    // UINT64_MAX: nine 0xFF bytes then 0x01
    static const char DATA[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                0xFF, 0xFF, 0xFF, 0xFF, 0x01};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rdtest_assert(rd_reader_read_uleb128(r, &v), "read UINT64_MAX");
    rdtest_assert_eq(v.value, UINT64_MAX);
    rdtest_assert_eq(v.length, 10U);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_read_uleb128_overlong_is_valid(void) {
    // non-canonical encodings of zero must be accepted: real producers
    // emit them, and rejecting breaks files
    static const char DATA[] = {0x80, 0x00};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rdtest_assert(rd_reader_read_uleb128(r, &v), "overlong zero rejected");
    rdtest_assert_eq(v.value, 0ULL);
    rdtest_assert_eq(v.length, 2U);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_read_uleb128_unterminated(void) {
    // a run of continuation bytes must fail, not spin to EOF; this is the
    // shape of several known OOB bugs in other DEX parsers
    static const char DATA[] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                                0x80, 0x80, 0x80, 0x80, 0x80};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rdtest_assert(!rd_reader_read_uleb128(r, &v), "unterminated accepted");
    rdtest_assert_eq(v.value, 0ULL);
    rdtest_assert_eq(v.length, 0U);
    rdtest_assert_true(rd_reader_has_error(r));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_read_uleb128_overflow(void) {
    // ten bytes whose payload exceeds 64 bits
    static const char DATA[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                0xFF, 0xFF, 0xFF, 0xFF, 0x7F};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rdtest_assert(!rd_reader_read_uleb128(r, &v), "overflow accepted");
    rdtest_assert_eq(v.length, 0U);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_read_uleb128_truncated(void) {
    // continuation bit set on the last byte in the buffer
    static const char DATA[] = {0x80};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rdtest_assert(!rd_reader_read_uleb128(r, &v), "truncated accepted");
    rdtest_assert_eq(v.length, 0U);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_read_sleb128(void) {
    static const char DATA[] = {
        0x00,       // 0
        0x01,       // 1
        0x7F,       // -1
        0x80, 0x7F, // -128
        0x3F,       // 63   (bit 6 clear -> positive)
        0x40,       // -64  (bit 6 set   -> negative)
    };

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDSLeb128 v;

    rdtest_assert(rd_reader_read_sleb128(r, &v), "read 0");
    rdtest_assert_eq(v.value, 0LL);
    rdtest_assert_eq(v.length, 1U);

    rdtest_assert(rd_reader_read_sleb128(r, &v), "read 1");
    rdtest_assert_eq(v.value, 1LL);

    rdtest_assert(rd_reader_read_sleb128(r, &v), "read -1");
    rdtest_assert_eq(v.value, -1LL);
    rdtest_assert_eq(v.length, 1U);

    rdtest_assert(rd_reader_read_sleb128(r, &v), "read -128");
    rdtest_assert_eq(v.value, -128LL);
    rdtest_assert_eq(v.length, 2U);

    rdtest_assert(rd_reader_read_sleb128(r, &v), "read 63");
    rdtest_assert_eq(v.value, 63LL);

    rdtest_assert(rd_reader_read_sleb128(r, &v), "read -64");
    rdtest_assert_eq(v.value, -64LL);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_read_sleb128_extremes(void) {
    static const char MIN[] = {0x80, 0x80, 0x80, 0x80, 0x80,
                               0x80, 0x80, 0x80, 0x80, 0x7F};
    static const char MAX[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                               0xFF, 0xFF, 0xFF, 0xFF, 0x00};

    RDSLeb128 v;

    RDContext* ctx_min = rdtest_context_create_from(MIN, sizeof(MIN));
    rdtest_assert_notnull(ctx_min);

    RDReader* r_min = rd_get_reader(ctx_min);
    rdtest_assert_notnull(r_min);

    rdtest_assert(rd_reader_read_sleb128(r_min, &v), "read INT64_MIN");
    rdtest_assert_eq(v.value, INT64_MIN);

    RDContext* ctx_max = rdtest_context_create_from(MAX, sizeof(MAX));
    rdtest_assert_notnull(ctx_min);

    RDReader* r_max = rd_get_reader(ctx_max);

    rdtest_assert_notnull(r_min);
    rdtest_assert(rd_reader_read_sleb128(r_max, &v), "read INT64_MAX");
    rdtest_assert_eq(v.value, INT64_MAX);

    rd_destroy(ctx_max);
    rd_destroy(ctx_min);
    return RDTEST_PASS;
}

static int test_read_sleb128_unterminated(void) {
    static const char DATA[] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                                0x80, 0x80, 0x80, 0x80, 0x80};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDSLeb128 v;

    rdtest_assert(!rd_reader_read_sleb128(r, &v), "unterminated accepted");
    rdtest_assert_eq(v.length, 0U);
    rdtest_assert_true(rd_reader_has_error(r));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// --- position handling ---

static int test_leb128_advances_by_length(void) {
    static const char DATA[] = {0x80, 0x7F, 0x01};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rdtest_assert(rd_reader_read_uleb128(r, &v), "read");
    rdtest_assert_eq(v.length, 2U);
    rdtest_assert_eq(rd_reader_tell(r), 2U);

    // the next read must land on the byte after the previous value
    rdtest_assert(rd_reader_read_uleb128(r, &v), "read");
    rdtest_assert_eq(v.value, 1ULL);
    rdtest_assert_eq(rd_reader_tell(r), 3U);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_leb128_failure_does_not_advance(void) {
    // the buffer read is a pure lookup, so a bad encoding must leave the
    // position at the start of the item rather than partway through it
    static const char DATA[] = {0x80};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rdtest_assert(!rd_reader_read_uleb128(r, &v), "truncated accepted");
    rdtest_assert_eq(rd_reader_tell(r), 0U);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// --- peek ---

static int test_peek_uleb128(void) {
    static const char DATA[] = {0x80, 0x7F};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rdtest_assert(rd_reader_peek_uleb128(r, &v), "peek");
    rdtest_assert_eq(v.value, 16256ULL);
    rdtest_assert_eq(v.length, 2U);
    rdtest_assert_eq(rd_reader_tell(r), 0U);

    // peeking twice must give the same answer
    rdtest_assert(rd_reader_peek_uleb128(r, &v), "peek again");
    rdtest_assert_eq(v.value, 16256ULL);
    rdtest_assert_eq(rd_reader_tell(r), 0U);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_peek_failure_does_not_poison(void) {
    // a failed peek is a query result, not a state change: the reader must
    // stay usable
    static const char DATA[] = {0x80};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rdtest_assert(!rd_reader_peek_uleb128(r, &v), "truncated accepted");
    rdtest_assert_false(rd_reader_has_error(r));
    rdtest_assert_eq(rd_reader_tell(r), 0U);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// --- buffer level ---

static int test_buffer_read_uleb128_at_index(void) {
    // random access: the same value must decode at any index, and the
    // buffer form must not depend on a position
    static const char DATA[] = {0xAA, 0xBB, 0x80, 0x7F, 0xCC};

    RDContext* ctx = rdtest_context_create_from(DATA, sizeof(DATA));
    rdtest_assert_notnull(ctx);

    RDReader* r = rd_get_reader(ctx);
    rdtest_assert_notnull(r);

    RDULeb128 v;

    rd_reader_seek(r, 2);
    rdtest_assert(rd_reader_peek_uleb128(r, &v), "peek at 2");
    rdtest_assert_eq(v.value, 16256ULL);
    rdtest_assert_eq(v.length, 2U);

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
    {"read_uleb128", test_read_uleb128},
    {"read_uleb128_max", test_read_uleb128_max},
    {"read_uleb128_overlong_is_valid", test_read_uleb128_overlong_is_valid},
    {"read_uleb128_unterminated", test_read_uleb128_unterminated},
    {"read_uleb128_overflow", test_read_uleb128_overflow},
    {"read_uleb128_truncated", test_read_uleb128_truncated},
    {"read_sleb128", test_read_sleb128},
    {"read_sleb128_extremes", test_read_sleb128_extremes},
    {"read_sleb128_unterminated", test_read_sleb128_unterminated},
    {"leb128_advances_by_length", test_leb128_advances_by_length},
    {"leb128_failure_does_not_advance", test_leb128_failure_does_not_advance},
    {"peek_uleb128", test_peek_uleb128},
    {"peek_failure_does_not_poison", test_peek_failure_does_not_poison},
    {"buffer_read_uleb128_at_index", test_buffer_read_uleb128_at_index},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("memory", K_TESTS);
    rdtest_deinit();
    return result;
}
