#include "rdtest.h"
#include "rdtest_helpers.h"
#include <redasm/support/byteorder.h>
#include <string.h>

/*
 * Round-trip tests alone are not enough here: a transposed index in both load
 * and store cancels out and the round-trip still passes. Every test below
 * anchors against a fixed byte pattern with a known value.
 */

// 0x0A0B0C0D 0x0E0F1011 as big-endian bytes
static const u8 K_BYTES[8] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11};

// --- load: fixed vectors ---

static int test_load_be(void) {
    rdtest_assert_eq(rd_loadbe16(K_BYTES), 0x0A0B);
    rdtest_assert_eq(rd_loadbe32(K_BYTES), 0x0A0B0C0DU);
    rdtest_assert_eq(rd_loadbe64(K_BYTES), 0x0A0B0C0D0E0F1011ULL);
    return RDTEST_PASS;
}

static int test_load_le(void) {
    rdtest_assert_eq(rd_loadle16(K_BYTES), 0x0B0A);
    rdtest_assert_eq(rd_loadle32(K_BYTES), 0x0D0C0B0AU);
    rdtest_assert_eq(rd_loadle64(K_BYTES), 0x11100F0E0D0C0B0AULL);
    return RDTEST_PASS;
}

static int test_load_me(void) {
    // middle-endian: LE 16-bit words, most significant word first
    rdtest_assert_eq(rd_loadme32(K_BYTES), 0x0B0A0D0CU);
    rdtest_assert_eq(rd_loadme64(K_BYTES), 0x0B0A0D0C0F0E1110ULL);
    return RDTEST_PASS;
}

static int test_load_orders_differ(void) {
    // the same four bytes must produce three distinct values; this is the
    // check that catches an implementation accidentally aliasing another
    u32 be = rd_loadbe32(K_BYTES);
    u32 le = rd_loadle32(K_BYTES);
    u32 me = rd_loadme32(K_BYTES);

    rdtest_assert_ne(be, le);
    rdtest_assert_ne(be, me);
    rdtest_assert_ne(le, me);
    return RDTEST_PASS;
}

// --- store: fixed vectors ---

static int test_store_be(void) {
    u8 b[8] = {0};

    rd_storebe16(b, 0x0A0B);
    rdtest_assert(!memcmp(b, K_BYTES, 2), "storebe16 layout");

    rd_storebe32(b, 0x0A0B0C0DU);
    rdtest_assert(!memcmp(b, K_BYTES, 4), "storebe32 layout");

    rd_storebe64(b, 0x0A0B0C0D0E0F1011ULL);
    rdtest_assert(!memcmp(b, K_BYTES, 8), "storebe64 layout");
    return RDTEST_PASS;
}

static int test_store_le(void) {
    u8 b[8] = {0};

    rd_storele16(b, 0x0B0A);
    rdtest_assert(!memcmp(b, K_BYTES, 2), "storele16 layout");

    rd_storele32(b, 0x0D0C0B0AU);
    rdtest_assert(!memcmp(b, K_BYTES, 4), "storele32 layout");

    rd_storele64(b, 0x11100F0E0D0C0B0AULL);
    rdtest_assert(!memcmp(b, K_BYTES, 8), "storele64 layout");
    return RDTEST_PASS;
}

static int test_store_me(void) {
    u8 b[8] = {0};

    rd_storeme32(b, 0x0B0A0D0CU);
    rdtest_assert(!memcmp(b, K_BYTES, 4), "storeme32 layout");

    rd_storeme64(b, 0x0B0A0D0C0F0E1110ULL);
    rdtest_assert(!memcmp(b, K_BYTES, 8), "storeme64 layout");
    return RDTEST_PASS;
}

// --- unaligned access ---

static int test_unaligned(void) {
    // every load/store must work at any byte offset; a u32* cast would fault
    // or silently misbehave here on strict-alignment targets
    u8 b[16] = {0};

    for(usize off = 0; off < 8; off++) {
        rd_storebe32(&b[off], 0xDEADBEEFU);
        rdtest_assert_eq(rd_loadbe32(&b[off]), 0xDEADBEEFU);

        rd_storele64(&b[off], 0x0102030405060708ULL);
        rdtest_assert_eq(rd_loadle64(&b[off]), 0x0102030405060708ULL);
    }

    return RDTEST_PASS;
}

static int test_store_does_not_overrun(void) {
    u8 b[8];
    memset(b, 0xCC, sizeof(b));

    rd_storebe32(&b[2], 0);
    rdtest_assert_eq(b[0], 0xCC);
    rdtest_assert_eq(b[1], 0xCC);
    rdtest_assert_eq(b[6], 0xCC);
    rdtest_assert_eq(b[7], 0xCC);
    return RDTEST_PASS;
}

// --- swap ---

static int test_swap(void) {
    rdtest_assert_eq(rd_swap16(0x1234), 0x3412);
    rdtest_assert_eq(rd_swap32(0x12345678U), 0x78563412U);
    rdtest_assert_eq(rd_swap64(0x0123456789ABCDEFULL), 0xEFCDAB8967452301ULL);
    return RDTEST_PASS;
}

static int test_swap_involution(void) {
    rdtest_assert_eq(rd_swap16(rd_swap16(0x1234)), 0x1234);
    rdtest_assert_eq(rd_swap32(rd_swap32(0x12345678U)), 0x12345678U);
    rdtest_assert_eq(rd_swap64(rd_swap64(0x0123456789ABCDEFULL)),
                     0x0123456789ABCDEFULL);
    return RDTEST_PASS;
}

// --- converters ---

static int test_convert_matches_load(void) {
    // rd_from* on a host-order load must agree with the corresponding
    // rd_load*; this ties the two tiers together
    u32 host;
    memcpy(&host, K_BYTES, sizeof(host));

    rdtest_assert_eq(rd_frombe32(host), rd_loadbe32(K_BYTES));
    rdtest_assert_eq(rd_fromle32(host), rd_loadle32(K_BYTES));
    return RDTEST_PASS;
}

static int test_convert_is_involution(void) {
    rdtest_assert_eq(rd_frombe32(rd_frombe32(0x12345678U)), 0x12345678U);
    rdtest_assert_eq(rd_fromle32(rd_fromle32(0x12345678U)), 0x12345678U);
    return RDTEST_PASS;
}

static int test_to_from_are_aliases(void) {
    // documented as the same operation; if these ever diverge, one of them
    // has picked up the wrong ternary sense
    rdtest_assert_eq(rd_tobe16(0x1234), rd_frombe16(0x1234));
    rdtest_assert_eq(rd_tobe32(0x12345678U), rd_frombe32(0x12345678U));
    rdtest_assert_eq(rd_tobe64(0x0123456789ABCDEFULL),
                     rd_frombe64(0x0123456789ABCDEFULL));
    rdtest_assert_eq(rd_tole16(0x1234), rd_fromle16(0x1234));
    rdtest_assert_eq(rd_tole32(0x12345678U), rd_fromle32(0x12345678U));
    rdtest_assert_eq(rd_tole64(0x0123456789ABCDEFULL),
                     rd_fromle64(0x0123456789ABCDEFULL));
    return RDTEST_PASS;
}

static int test_host_order_is_consistent(void) {
    // exactly one of the two must hold, and it must agree with what a
    // byte-wise inspection of a known value shows
    rdtest_assert(RD_IS_LITTLE_ENDIAN != RD_IS_BIG_ENDIAN,
                  "host order macros disagree");

    u32 v = 0x01020304U;
    u8 b[sizeof(v)];
    memcpy(b, &v, sizeof(v));

    if(RD_IS_LITTLE_ENDIAN)
        rdtest_assert_eq(b[0], 0x04);
    else
        rdtest_assert_eq(b[0], 0x01);

    return RDTEST_PASS;
}

// --- edge values ---

static int test_extremes(void) {
    u8 b[8];

    rd_storebe64(b, 0);
    rdtest_assert_eq(rd_loadbe64(b), 0ULL);

    rd_storebe64(b, UINT64_MAX);
    rdtest_assert_eq(rd_loadbe64(b), UINT64_MAX);
    rdtest_assert_eq(rd_loadle64(b), UINT64_MAX);

    rd_storele16(b, 0xFFFF);
    rdtest_assert_eq(rd_loadle16(b), 0xFFFF);
    rdtest_assert_eq(rd_loadbe16(b), 0xFFFF);
    return RDTEST_PASS;
}

static const RDTest K_TESTS[] = {
    {"load_be", test_load_be},
    {"load_le", test_load_le},
    {"load_me", test_load_me},
    {"load_orders_differ", test_load_orders_differ},
    {"store_be", test_store_be},
    {"store_le", test_store_le},
    {"store_me", test_store_me},
    {"unaligned", test_unaligned},
    {"store_does_not_overrun", test_store_does_not_overrun},
    {"swap", test_swap},
    {"swap_involution", test_swap_involution},
    {"convert_matches_load", test_convert_matches_load},
    {"convert_is_involution", test_convert_is_involution},
    {"to_from_are_aliases", test_to_from_are_aliases},
    {"host_order_is_consistent", test_host_order_is_consistent},
    {"extremes", test_extremes},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("byteorder", K_TESTS);
    rdtest_deinit();
    return result;
}
