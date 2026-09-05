#include "rdtest.h"
#include "rdtest_helpers.h"
#include <redasm/support/byteorder.h>
#include <redasm/support/hash.h>
#include <string.h>

static const char* _hex(const u8* d, usize n) {
    static char buf[128];
    static const char* const HEX = "0123456789abcdef";

    if(n * 2 >= sizeof(buf)) return "<too long>";

    for(usize i = 0; i < n; i++) {
        buf[i * 2] = HEX[d[i] >> 4];
        buf[(i * 2) + 1] = HEX[d[i] & 0xF];
    }

    buf[n * 2] = '\0';
    return buf;
}

static const char* _sha1_hex(const void* data, usize n) {
    RDHash* h = rd_hash_create(RD_HASH_SHA1);
    if(!h) return NULL;

    rd_hash_update(h, data, n);
    const u8* d = rd_hash_final(h);
    const char* s = d ? _hex(d, RD_HASH_SHA1_LENGTH) : NULL;
    rd_hash_destroy(h);
    return s;
}

static bool _adler32(const void* data, usize n, u32* v) {
    RDHash* h = rd_hash_create(RD_HASH_ADLER32);
    if(!h) return false;

    rd_hash_update(h, data, n);

    u8 d[RD_HASH_ADLER32_LENGTH];
    bool ok = rd_hash_final_to(h, d, sizeof(d));
    if(ok) *v = rd_loadbe32(d);

    rd_hash_destroy(h);
    return ok;
}

// --- SHA-1 known vectors (FIPS 180-1) ---

static int test_sha1_empty(void) {
    rdtest_assert_streq(_sha1_hex("", 0),
                        "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    return RDTEST_PASS;
}

static int test_sha1_abc(void) {
    rdtest_assert_streq(_sha1_hex("abc", 3),
                        "a9993e364706816aba3e25717850c26c9cd0d89d");
    return RDTEST_PASS;
}

static int test_sha1_two_blocks(void) {
    // 56 bytes: padding pushes the length field into a second block, so this
    // exercises the multi-block path that "abc" does not
    static const char MSG[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmn"
                              "omnopnopq";
    rdtest_assert_eq(strlen(MSG), 56);
    rdtest_assert_streq(_sha1_hex(MSG, strlen(MSG)),
                        "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
    return RDTEST_PASS;
}

static int test_sha1_chunked_matches_whole(void) {
    // feeding the same bytes in uneven pieces must produce the same digest;
    // this is what catches a broken buffer/offset in update()
    static const char MSG[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmn"
                              "omnopnopq";
    usize n = strlen(MSG);

    RDHash* h = rd_hash_create(RD_HASH_SHA1);
    rdtest_assert_notnull(h);

    static const usize CHUNKS[] = {1, 7, 13, 35};
    usize off = 0;

    for(usize i = 0; i < sizeof(CHUNKS) / sizeof(CHUNKS[0]); i++) {
        usize k = CHUNKS[i];
        if(off + k > n) k = n - off;
        rd_hash_update(h, &MSG[off], k);
        off += k;
    }

    if(off < n) rd_hash_update(h, &MSG[off], n - off);

    const u8* d = rd_hash_final(h);
    rdtest_assert_notnull(d);
    rdtest_assert_streq(_hex(d, RD_HASH_SHA1_LENGTH),
                        "84983e441c3bd26ebaae4aa1f95129e5e54670f1");

    rd_hash_destroy(h);
    return RDTEST_PASS;
}

// --- Adler-32 known vectors ---

static int test_adler32_empty(void) {
    u32 v = 0;
    rdtest_assert(_adler32("", 0, &v), "adler32 failed");
    rdtest_assert_eq(v, 1U); // a=1, b=0
    return RDTEST_PASS;
}

static int test_adler32_abc(void) {
    u32 v = 0;
    rdtest_assert(_adler32("abc", 3, &v), "adler32 failed");
    rdtest_assert_eq(v, 0x024D0127U);
    return RDTEST_PASS;
}

static int test_adler32_wikipedia(void) {
    u32 v = 0;
    rdtest_assert(_adler32("Wikipedia", 9, &v), "adler32 failed");
    rdtest_assert_eq(v, 0x11E60398U);
    return RDTEST_PASS;
}

static int test_adler32_modulo_wrap(void) {
    // long enough to cross the NMAX deferred-modulo boundary (5552); a
    // missing or misplaced modulo shows up here and nowhere shorter
    static u8 data[16384];
    for(usize i = 0; i < sizeof(data); i++)
        data[i] = (u8)(i & 0xFF);

    u32 v = 0;
    rdtest_assert(_adler32(data, sizeof(data), &v), "adler32 failed");

    // recomputed independently, byte at a time
    u32 a = 1, b = 0;
    for(usize i = 0; i < sizeof(data); i++) {
        a = (a + data[i]) % 65521;
        b = (b + a) % 65521;
    }

    rdtest_assert_eq(v, (b << 16) | a);
    return RDTEST_PASS;
}

static int test_adler32_chunked_matches_whole(void) {
    static const char MSG[] = "Wikipedia";

    RDHash* h = rd_hash_create(RD_HASH_ADLER32);
    rdtest_assert_notnull(h);

    rd_hash_update(h, MSG, 4);
    rd_hash_update(h, MSG + 4, 5);

    u32 v = 0;
    u8 d[RD_HASH_ADLER32_LENGTH];
    rdtest_assert(rd_hash_final_to(h, d, sizeof(d)), "final_to failed");
    v = rd_loadbe32(d);
    rdtest_assert_eq(v, 0x11E60398U);

    rd_hash_destroy(h);
    return RDTEST_PASS;
}

// --- lifecycle ---

static int test_final_is_idempotent(void) {
    RDHash* h = rd_hash_create(RD_HASH_SHA1);
    rdtest_assert_notnull(h);

    rd_hash_update(h, "abc", 3);

    u8 first[RD_HASH_SHA1_LENGTH];
    rdtest_assert(rd_hash_final_to(h, first, sizeof(first)), "first final");

    const u8* second = rd_hash_final(h);
    rdtest_assert_notnull(second);
    rdtest_assert(!memcmp(first, second, sizeof(first)),
                  "second final differs");

    rd_hash_destroy(h);
    return RDTEST_PASS;
}

static int test_update_after_final_is_ignored(void) {
    RDHash* h = rd_hash_create(RD_HASH_SHA1);
    rdtest_assert_notnull(h);

    rd_hash_update(h, "abc", 3);

    u8 before[RD_HASH_SHA1_LENGTH];
    rdtest_assert(rd_hash_final_to(h, before, sizeof(before)), "final failed");

    rd_hash_update(h, "more data", 9); // must not corrupt the digest

    u8 after[RD_HASH_SHA1_LENGTH];
    rdtest_assert(rd_hash_final_to(h, after, sizeof(after)), "final failed");
    rdtest_assert(!memcmp(before, after, sizeof(before)),
                  "update after final changed the digest");

    rd_hash_destroy(h);
    return RDTEST_PASS;
}

static int test_reset(void) {
    RDHash* h = rd_hash_create(RD_HASH_SHA1);
    rdtest_assert_notnull(h);

    rd_hash_update(h, "garbage", 7);
    (void)rd_hash_final(h);

    rd_hash_reset(h);
    rd_hash_update(h, "abc", 3);

    const u8* d = rd_hash_final(h);
    rdtest_assert_notnull(d);
    rdtest_assert_streq(_hex(d, RD_HASH_SHA1_LENGTH),
                        "a9993e364706816aba3e25717850c26c9cd0d89d");

    rd_hash_destroy(h);
    return RDTEST_PASS;
}

static int test_get_length(void) {
    RDHash* sha1 = rd_hash_create(RD_HASH_SHA1);
    RDHash* adler = rd_hash_create(RD_HASH_ADLER32);
    rdtest_assert_notnull(sha1);
    rdtest_assert_notnull(adler);

    rdtest_assert_eq(rd_hash_get_length(sha1), RD_HASH_SHA1_LENGTH);
    rdtest_assert_eq(rd_hash_get_length(adler), RD_HASH_ADLER32_LENGTH);

    rd_hash_destroy(sha1);
    rd_hash_destroy(adler);
    return RDTEST_PASS;
}

static int test_final_to_rejects_small_buffer(void) {
    RDHash* h = rd_hash_create(RD_HASH_SHA1);
    rdtest_assert_notnull(h);

    rd_hash_update(h, "abc", 3);

    u8 tooshort[RD_HASH_SHA1_LENGTH - 1];
    rdtest_assert(!rd_hash_final_to(h, tooshort, sizeof(tooshort)),
                  "final_to must reject an undersized buffer");

    rd_hash_destroy(h);
    return RDTEST_PASS;
}

static int test_null_safety(void) {
    rdtest_assert_null(rd_hash_final(NULL));
    rdtest_assert(!rd_hash_final_to(NULL, NULL, 0), "final_to(NULL)");

    rd_hash_update(NULL, "x", 1); // must not crash
    rd_hash_destroy(NULL);        // must not crash
    rd_hash_reset(NULL);          // must not crash

    return RDTEST_PASS;
}

static int test_invalid_kind(void) {
    rdtest_assert_null(rd_hash_create((RDHashKind)0x7FFF));
    return RDTEST_PASS;
}

static const RDTest K_TESTS[] = {
    {"sha1_empty", test_sha1_empty},
    {"sha1_abc", test_sha1_abc},
    {"sha1_two_blocks", test_sha1_two_blocks},
    {"sha1_chunked_matches_whole", test_sha1_chunked_matches_whole},
    {"adler32_empty", test_adler32_empty},
    {"adler32_abc", test_adler32_abc},
    {"adler32_wikipedia", test_adler32_wikipedia},
    {"adler32_modulo_wrap", test_adler32_modulo_wrap},
    {"adler32_chunked_matches_whole", test_adler32_chunked_matches_whole},
    {"final_is_idempotent", test_final_is_idempotent},
    {"update_after_final_is_ignored", test_update_after_final_is_ignored},
    {"reset", test_reset},
    {"get_length", test_get_length},
    {"final_to_rejects_small_buffer", test_final_to_rejects_small_buffer},
    {"null_safety", test_null_safety},
    {"invalid_kind", test_invalid_kind},
    {NULL, NULL},
};

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("hash", K_TESTS);
    rdtest_deinit();
    return result;
}
