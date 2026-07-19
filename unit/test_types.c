#include "rdtest.h"
#include "rdtest_helpers.h"
#include <string.h>

typedef struct RDTestTypeShape {
    const char* name;
    usize count;
    RDTypeModifier mod;
    usize size;
} RDTestTypeShape;

static int test_primitives(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    static const RDTestTypeShape PRIMITIVES[] = {
        {.name = "bool", .size = sizeof(bool)},
        {.name = "u8", .size = sizeof(u8)},
        {.name = "u16", .size = sizeof(u16)},
        {.name = "u32", .size = sizeof(u32)},
        {.name = "u64", .size = sizeof(u64)},
        {.name = "i8", .size = sizeof(i8)},
        {.name = "i16", .size = sizeof(i16)},
        {.name = "i32", .size = sizeof(i32)},
        {.name = "i64", .size = sizeof(i64)},
        {.name = "char", .size = sizeof(i8)},
        {.name = "char16", .size = sizeof(i16)},
        {0},
    };

    const RDTestTypeShape* tt = PRIMITIVES;

    while(tt->name) {
        RDType t;
        rdtest_assert_true(rd_type_init(&t, tt->name, tt->count, tt->mod, ctx));
        rdtest_assert_notnull(&t.def);
        rdtest_assert_eq(rd_type_size(&t, ctx), tt->size);
        tt++;
    }

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_init_unregistered(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    RDType t;
    rdtest_assert_false(rd_type_init(&t, "DoesNotExist", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_false(rd_type_init(&t, NULL, 0, RD_TYPE_NONE, ctx));
    rdtest_assert_false(rd_type_init(NULL, "u32", 0, RD_TYPE_NONE, ctx));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_struct(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* tdef = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(tdef, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(tdef, "u32", "y", 0, RD_TYPE_NONE, ctx));
    // clang-format on
    rdtest_assert_true(rd_typedef_register(tdef, ctx));

    rdtest_assert_eq(rd_typedef_size(tdef), sizeof(u32) * 2);

    RDParamSlice params = rd_typedef_get_members(tdef);
    rdtest_assert_eq(rd_slice_length(params), 2);

    RDParam x = rd_slice_at(params, 0);
    RDParam y = rd_slice_at(params, 1);
    rdtest_assert_eq(x.field_offset, 0);
    rdtest_assert_eq(y.field_offset, sizeof(u32));
    rdtest_assert_streq(x.name, "x");
    rdtest_assert_streq(y.name, "y");

    RDType t;
    rdtest_assert_true(rd_type_init(&t, "Point", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_eq(rd_type_size(&t, ctx), rd_typedef_size(tdef));

    rdtest_assert_true(rd_type_init(&t, "Point", 10, RD_TYPE_NONE, ctx));
    rdtest_assert_eq(rd_type_size(&t, ctx), rd_typedef_size(tdef) * 10);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_struct_nested(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));

    RDTypeDef* point11 = rd_typedef_create_struct("Point11", ctx);
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "first", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "items", 10, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point11, ctx));
    // clang-format on

    RDParamSlice params = rd_typedef_get_members(point11);

    RDParam first = rd_slice_at(params, 0);
    RDParam items = rd_slice_at(params, 1);
    rdtest_assert_eq(first.field_offset, 0);
    rdtest_assert_eq(items.field_offset, rd_typedef_size(point));

    rdtest_assert_eq(rd_typedef_size(point11),
                     rd_typedef_size(point) + (rd_typedef_size(point) * 10));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_struct_with_pointer_member(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));

    RDTypeDef* node = rd_typedef_create_struct("Node", ctx);
    rdtest_assert_true(rd_typedef_add_member(node, "Point", "pos", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(node, "Point", "next", 0, RD_TYPE_PTR, ctx));
    rdtest_assert_true(rd_typedef_register(node, ctx));
    // clang-format on

    RDParamSlice params = rd_typedef_get_members(node);

    RDParam pos = rd_slice_at(params, 0);
    RDParam next = rd_slice_at(params, 1);

    RDType ptr;
    rdtest_assert_true(rd_type_init(&ptr, "Point", 0, RD_TYPE_PTR, ctx));

    rdtest_assert_eq(next.field_offset, rd_typedef_size(point));
    rdtest_assert_eq(rd_typedef_size(node),
                     rd_typedef_size(point) + rd_type_size(&ptr, ctx));

    (void)pos;
    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_array_primitives(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    static const RDTestTypeShape ARRAYS[] = {
        {.name = "u8", .count = 4, .size = sizeof(u8) * 4},
        {.name = "u16", .count = 4, .size = sizeof(u16) * 4},
        {.name = "u32", .count = 8, .size = sizeof(u32) * 8},
        {.name = "u64", .count = 8, .size = sizeof(u64) * 8},
        {.name = "char", .count = 16, .size = sizeof(i8) * 16},
        {0},
    };

    const RDTestTypeShape* tt = ARRAYS;

    while(tt->name) {
        RDType t;
        rdtest_assert_true(rd_type_init(&t, tt->name, tt->count, tt->mod, ctx));
        rdtest_assert_notnull(t.def);
        rdtest_assert_eq(t.count, tt->count);
        rdtest_assert_eq(rd_type_size(&t, ctx), tt->size);
        tt++;
    }

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_array_count_one_size(void) {
    // A [1] occupies exactly the bytes of its element but it is STILL an
    // array (see count_one_is_array). Size equivalence is a fact about
    // memory; array-ness is a fact about the declaration. Never let the
    // first assumption erase the second.

    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    RDType scalar, one;
    rdtest_assert_true(rd_type_init(&scalar, "u32", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_type_init(&one, "u32", 1, RD_TYPE_NONE, ctx));
    rdtest_assert_eq(rd_type_size(&scalar, ctx), rd_type_size(&one, ctx));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_array_of_struct(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* tdef = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(tdef, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(tdef, "u32", "y", 0, RD_TYPE_NONE, ctx));
    // clang-format on
    rdtest_assert_true(rd_typedef_register(tdef, ctx));

    RDType arr;
    rdtest_assert_true(rd_type_init(&arr, "Point", 10, RD_TYPE_NONE, ctx));
    rdtest_assert_eq(rd_type_size(&arr, ctx), rd_typedef_size(tdef) * 10);

    RDType one;
    rdtest_assert_true(rd_type_init(&one, "Point", 1, RD_TYPE_NONE, ctx));
    rdtest_assert_eq(rd_type_size(&one, ctx), rd_typedef_size(tdef));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_array_of_pointers(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* tdef = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(tdef, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(tdef, "u32", "y", 0, RD_TYPE_NONE, ctx));
    // clang-format on
    rdtest_assert_true(rd_typedef_register(tdef, ctx));

    RDType ptr;
    rdtest_assert_true(rd_type_init(&ptr, "Point", 0, RD_TYPE_PTR, ctx));
    usize ptr_size = rd_type_size(&ptr, ctx);

    RDType arr_of_ptr;
    rdtest_assert_true(
        rd_type_init(&arr_of_ptr, "Point", 10, RD_TYPE_PTR, ctx));
    rdtest_assert_eq(rd_type_size(&arr_of_ptr, ctx), ptr_size * 10);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_pointer_size_consistency(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    RDType p8, p64;
    rdtest_assert_true(rd_type_init(&p8, "u8", 0, RD_TYPE_PTR, ctx));
    rdtest_assert_true(rd_type_init(&p64, "u64", 0, RD_TYPE_PTR, ctx));
    rdtest_assert_eq(rd_type_size(&p8, ctx), rd_type_size(&p64, ctx));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_typedef_resolve_offset_shallow(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* tdef = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(tdef, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(tdef, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(tdef, ctx));
    // clang-format on

    const RDParam* m;
    rdtest_assert_true(rd_typedef_resolve_offset(ctx, tdef, 0, &m));
    rdtest_assert_streq(m->name, "x");

    rdtest_assert_true(rd_typedef_resolve_offset(ctx, tdef, 4, &m));
    rdtest_assert_streq(m->name, "y");

    // past the struct's own size
    rdtest_assert_false(rd_typedef_resolve_offset(ctx, tdef, 8, &m));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_typedef_resolve_offset_union_rejected(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* tdef = rd_typedef_create_union("Value", ctx);
    rdtest_assert_true(rd_typedef_add_member(tdef, "u32", "as_u32", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(tdef, ctx));
    // clang-format on

    // kind != RD_TKIND_STRUCT: must reject regardless of offset validity
    const RDParam* m;
    rdtest_assert_false(rd_typedef_resolve_offset(ctx, tdef, 0, &m));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_coincidence(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));

    RDTypeDef* point11 = rd_typedef_create_struct("Point11", ctx);
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "first", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "items", 10, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point11, ctx));
    // clang-format on

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Point11", 0, RD_TYPE_NONE, ctx));

    RDResolveResult r = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &root, 0, 0, &r));
    rdtest_assert_streq(r.field.name, "first");
    rdtest_assert_streq(rd_typedef_name(r.field.type.def), "Point");
    rdtest_assert_eq(r.field.type.count, 0);
    rdtest_assert_eq(r.depth, 0);
    rdtest_assert(!r.item_idx.has_value,
                  "item_idx must be unset, no array crossed");

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_min_depth(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));

    RDTypeDef* point11 = rd_typedef_create_struct("Point11", ctx);
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "first", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "items", 10, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point11, ctx));
    // clang-format on

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Point11", 0, RD_TYPE_NONE, ctx));

    // min_depth=0
    // stops at the shallowest entity on the offset: first. The container
    // (Point11) is NEVER an answer, resolution searches within
    RDResolveResult r0 = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &root, 0, 0, &r0));
    rdtest_assert_streq(r0.field.name, "first");
    rdtest_assert_eq(r0.depth, 0);

    // min_depth=1
    // Point11's own boundary no longer satisfies the floor (0 >= 1 is false),
    // forced deeper into "first" -> x (rel==0 there, depth==1, 1>=1 satisfied)
    RDResolveResult r1 = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &root, 0, 1, &r1));
    rdtest_assert_streq(r1.field.name, "x");
    rdtest_assert_streq(rd_typedef_name(r1.field.type.def), "u32");
    rdtest_assert_eq(r1.depth, 1);

    // RD_MAX_DEPTH
    // never satisfies the floor at any coincidence point, forced all the way to
    // the true leaf regardless of how deep
    RDResolveResult rmax = {0};
    rdtest_assert_true(
        rd_type_resolve_offset(ctx, &root, 0, RD_MAX_DEPTH, &rmax));
    rdtest_assert_streq(rmax.field.name, "x");
    rdtest_assert_streq(rd_typedef_name(rmax.field.type.def), "u32");
    rdtest_assert_eq(rmax.depth,
                     1); // same leaf as min_depth=1, since x IS the leaf here

    // a genuinely nonzero rel (first.y) ignores min_depth entirely.
    // The walk was never optional at this point regardless of the floor
    RDResolveResult ry = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &root, 4, 0, &ry));
    rdtest_assert_streq(ry.field.name, "y");
    rdtest_assert_eq(ry.depth, 1);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_deep_nested(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));

    RDTypeDef* point11 = rd_typedef_create_struct("Point11", ctx);
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "first", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "items", 10, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point11, ctx));
    // clang-format on

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Point11", 0, RD_TYPE_NONE, ctx));

    RDResolveResult r = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &root, 4, 0, &r)); // first.y
    rdtest_assert_streq(r.field.name, "y");
    rdtest_assert_streq(rd_typedef_name(r.field.type.def), "u32");
    rdtest_assert_eq(r.depth, 1);
    rdtest_assert(!r.item_idx.has_value, "no array crossed");

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_array(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));
    // clang-format on

    RDType arr;
    rdtest_assert_true(rd_type_init(&arr, "Point", 10, RD_TYPE_NONE, ctx));

    usize target_offset =
        (3 * rd_typedef_size(point)) + sizeof(u32); // items[3].y

    RDResolveResult r = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &arr, target_offset, 0, &r));
    rdtest_assert(r.item_idx.has_value, "array was crossed");
    rdtest_assert_eq(r.item_idx.value, 3);
    rdtest_assert_streq(r.field.name, "y");
    rdtest_assert_streq(rd_typedef_name(r.field.type.def), "u32");
    rdtest_assert_eq(r.depth, 1);

    RDResolveResult elem_r = {0};
    rdtest_assert_true(rd_type_resolve_offset(
        ctx, &arr, 3 * rd_typedef_size(point), 0, &elem_r));
    rdtest_assert(elem_r.item_idx.has_value, "array was crossed");
    rdtest_assert_eq(elem_r.item_idx.value, 3);
    rdtest_assert_streq(rd_typedef_name(elem_r.field.type.def), "Point");
    rdtest_assert_eq(elem_r.field.type.count, 0);
    rdtest_assert_eq(elem_r.depth, 0);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_array_of_pointers(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));
    // clang-format on

    RDType arr;
    rdtest_assert_true(rd_type_init(&arr, "Point", 10, RD_TYPE_PTR, ctx));

    RDType ptr;
    rdtest_assert_true(rd_type_init(&ptr, "Point", 0, RD_TYPE_PTR, ctx));
    usize ptr_size = rd_type_size(&ptr, ctx);

    RDResolveResult r = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &arr, 3 * ptr_size, 0, &r));
    rdtest_assert(r.item_idx.has_value, "array was crossed");
    rdtest_assert_eq(r.item_idx.value, 3);
    rdtest_assert_eq(r.field.type.mod, RD_TYPE_PTR);
    rdtest_assert_eq(r.depth, 0);

    RDResolveResult bad = {0};
    rdtest_assert_false(
        rd_type_resolve_offset(ctx, &arr, (3 * ptr_size) + 1, 0, &bad));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_pointer_terminal(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));
    // clang-format on

    RDType ptr;
    rdtest_assert_true(rd_type_init(&ptr, "Point", 0, RD_TYPE_PTR, ctx));

    RDResolveResult r = {0};
    // even with a floor requesting max depth, a pointer
    // terminal is unconditional: min_depth is never consulted there
    rdtest_assert_true(rd_type_resolve_offset(ctx, &ptr, 0, RD_MAX_DEPTH, &r));
    rdtest_assert_eq(r.depth, 0);
    rdtest_assert(!r.item_idx.has_value, "no array involved");

    RDResolveResult bad = {0};
    rdtest_assert_false(rd_type_resolve_offset(ctx, &ptr, 1, 0, &bad));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_primitive_leaf(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    RDType t;
    rdtest_assert_true(rd_type_init(&t, "u32", 0, RD_TYPE_NONE, ctx));

    RDResolveResult r = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &t, 0, RD_MAX_DEPTH, &r));
    rdtest_assert_streq(rd_typedef_name(r.field.type.def), "u32");
    rdtest_assert_eq(r.depth, 0);
    rdtest_assert(!r.item_idx.has_value, "no array involved");

    RDResolveResult bad = {0};
    rdtest_assert_false(rd_type_resolve_offset(ctx, &t, 2, 0, &bad));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_not_found(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));
    // clang-format on

    RDType t;
    rdtest_assert_true(rd_type_init(&t, "Point", 0, RD_TYPE_NONE, ctx));

    RDResolveResult r = {0};
    rdtest_assert_false(rd_type_resolve_offset(ctx, &t, 100, 0, &r));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// ===========================================================================
// Honest-exhaustion contract (the `has_more` guard)
//
// `min_depth` is a FLOOR. When the schema is SHALLOWER than the floor, the
// walk must stop at the deepest REAL entity and report that depth honestly
// (never invent a deeper anonymous clone). Callers detect "nothing that deep
// exists" by comparing out->depth < min_depth after a `true` return.
// ===========================================================================

static int test_type_resolve_offset_exhaustion_scalar(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    RDType t;
    rdtest_assert_true(rd_type_init(&t, "u32", 0, RD_TYPE_NONE, ctx));

    // a bare scalar has chain length 1: any floor > 0 is unsatisfiable.
    // Honest answer: the scalar itself, depth 0 (0 < 1 = caller's signal)
    RDResolveResult r = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &t, 0, 1, &r));
    rdtest_assert_streq(rd_typedef_name(r.field.type.def), "u32");
    rdtest_assert_eq(r.depth, 0);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_exhaustion_struct(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    /* Memory picture (Point = 8 bytes, Point11 = 88 bytes):
     *
     *  off:  0        4        8       16      ...     88
     *        +--------+--------+--------+---     ---+
     *        | first.x| first.y|items[0]|    ...    |
     *        +--------+--------+--------+---     ---+
     *        ^                 ^
     *        Point11           items
     *        first             items[0]
     *        first.x           items[0].x
     *
     * Coincidence chain at off 0: [first (d0), x (d1)], ends at x, a leaf.
     * Forcing min_depth=2 must NOT fabricate a depth-2 anonymous u32. */
    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));

    RDTypeDef* point11 = rd_typedef_create_struct("Point11", ctx);
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "first", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "items", 10, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point11, ctx));
    // clang-format on

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Point11", 0, RD_TYPE_NONE, ctx));

    // floor 2, chain ends at depth 1: honest stop at x, WITH its real name
    RDResolveResult r = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &root, 0, 2, &r));
    rdtest_assert_streq(r.field.name, "x");
    rdtest_assert_eq(r.depth, 1); // 1 < 2: caller sees the chain ended

    // off 4 (first.y): reached through first at rel!=0 (mandatory descent),
    // then y is a leaf. Floor 5 is absurd; honest stop is y at depth 1.
    RDResolveResult ry = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &root, 4, 5, &ry));
    rdtest_assert_streq(ry.field.name, "y");
    rdtest_assert_eq(ry.depth, 1);
    rdtest_assert(!ry.item_idx.has_value, "no array crossed");

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_exhaustion_array(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // u32[4]: an element boundary with a scalar element type. Forcing past
    // it must stop AT the element (depth 0), not inside a phantom clone
    RDType arr_u32;
    rdtest_assert_true(rd_type_init(&arr_u32, "u32", 4, RD_TYPE_NONE, ctx));

    RDResolveResult r = {0};
    rdtest_assert_true(
        rd_type_resolve_offset(ctx, &arr_u32, 2 * sizeof(u32), 3, &r));
    rdtest_assert(r.item_idx.has_value, "array was crossed");
    rdtest_assert_eq(r.item_idx.value, 2);
    rdtest_assert_eq(r.depth, 0); // 0 < 3: chain ends at the element

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));
    // clang-format on

    // Point[10] with RD_MAX_DEPTH, at element 3's boundary. Depth counts
    // recursions taken: the floor rejects the element's own stop (depth 0),
    // one hop enters element 3, whose first member x coincides (rel==0) at
    // depth 1. x is a leaf: honest stop there, infinite floor or not.
    RDType arr_pt;
    rdtest_assert_true(rd_type_init(&arr_pt, "Point", 10, RD_TYPE_NONE, ctx));

    RDResolveResult rp = {0};
    rdtest_assert_true(rd_type_resolve_offset(
        ctx, &arr_pt, 3 * rd_typedef_size(point), RD_MAX_DEPTH, &rp));
    rdtest_assert_streq(rp.field.name, "x");
    rdtest_assert_eq(rp.depth, 1);
    rdtest_assert_eq(rp.item_idx.value, 3);

    // Point*[10]: pointer elements are opaque, the element IS the terminus
    RDType arr_ptr;
    rdtest_assert_true(rd_type_init(&arr_ptr, "Point", 10, RD_TYPE_PTR, ctx));

    RDType ptr;
    rdtest_assert_true(rd_type_init(&ptr, "Point", 0, RD_TYPE_PTR, ctx));

    RDResolveResult rptr = {0};
    rdtest_assert_true(rd_type_resolve_offset(
        ctx, &arr_ptr, 3 * rd_type_size(&ptr, ctx), RD_MAX_DEPTH, &rptr));
    rdtest_assert_eq(rptr.field.type.mod, RD_TYPE_PTR);
    rdtest_assert_eq(rptr.depth, 0);
    rdtest_assert_eq(rptr.item_idx.value, 3);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_array_bounds(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));
    // clang-format on

    RDType arr;
    rdtest_assert_true(rd_type_init(&arr, "Point", 10, RD_TYPE_NONE, ctx));

    // element 10 of a [10] array does not exist: public API must fail
    // cleanly (RD_LOG_FAIL + false), never "succeed" with item_idx = 10
    RDResolveResult r = {0};
    rdtest_assert_false(
        rd_type_resolve_offset(ctx, &arr, 10 * rd_typedef_size(point), 0, &r));

    RDResolveResult r2 = {0};
    rdtest_assert_false(rd_type_resolve_offset(ctx, &arr, 999, 0, &r2));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_count_one_is_array(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // [1] is a REAL array (eg, TOKEN_PRIVILEGES.Privileges[1],
    // IMAGE_IMPORT_BY_NAME.Name[1]) declares "an array that continues".
    // Declaration fidelity: count == 0 is a scalar, count >= 1 an array.
    // No normalization anywhere.
    RDType t;
    rdtest_assert_true(rd_type_init(&t, "u32", 1, RD_TYPE_NONE, ctx));

    RDResolveResult r = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &t, 0, 0, &r));
    rdtest_assert_streq(rd_typedef_name(r.field.type.def), "u32");
    rdtest_assert(r.item_idx.has_value, "a [1] crosses its array");
    rdtest_assert_eq(r.item_idx.value, 0);
    rdtest_assert_eq(r.depth, 0);

    // forcing past the single scalar element exhausts honestly
    RDResolveResult r2 = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &t, 0, 1, &r2));
    rdtest_assert_eq(r2.depth, 0); // 0 < 1: nothing deeper exists

    // and [1] means ONE element: its second is out of bounds
    RDResolveResult r3 = {0};
    rdtest_assert_false(rd_type_resolve_offset(ctx, &t, sizeof(u32), 0, &r3));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_union_is_opaque(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    /* Unions are deliberately unresolvable below their own boundary
     * (design rule: which member "is" at the address is a rendering
     * convention, not an address-resolvable fact). The resolver treats a
     * union like a leaf: never descends into it, never forced past it. */
    // clang-format off
    RDTypeDef* value = rd_typedef_create_union("Value", ctx);
    rdtest_assert_true(rd_typedef_add_member(value, "u32", "as_u32", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(value, "u16", "as_u16", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(value, ctx));

    RDTypeDef* holder = rd_typedef_create_struct("Holder", ctx);
    rdtest_assert_true(rd_typedef_add_member(holder, "u32", "tag", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(holder, "Value", "v", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(holder, ctx));
    // clang-format on

    // the union itself resolves fine at its own boundary...
    RDType uv;
    rdtest_assert_true(rd_type_init(&uv, "Value", 0, RD_TYPE_NONE, ctx));

    RDResolveResult r = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &uv, 0, RD_MAX_DEPTH, &r));
    rdtest_assert_eq(r.depth, 0);

    // ...but never below it, even with an infinite floor: the union member
    // is the honest terminus, with its own name intact. v sits at rel==0
    // directly under Holder.
    // Zero recursions taken, so depth is 0
    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Holder", 0, RD_TYPE_NONE, ctx));

    RDResolveResult rv = {0};
    rdtest_assert_true(
        rd_type_resolve_offset(ctx, &root, sizeof(u32), RD_MAX_DEPTH, &rv));
    rdtest_assert_streq(rv.field.name, "v");
    rdtest_assert_eq(rv.depth, 0);

    // an offset INSIDE the union body is malformed input: clean failure
    RDResolveResult bad = {0};
    rdtest_assert_false(
        rd_type_resolve_offset(ctx, &root, sizeof(u32) + 2, 0, &bad));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_result_is_zeroed(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));
    // clang-format on

    RDType t;
    rdtest_assert_true(rd_type_init(&t, "Point", 0, RD_TYPE_NONE, ctx));

    // public API: the wrapper owns initialization. A caller passing a
    // dirty result struct must get identical answers to a {0} caller
    // (depth is an accumulator, it must never inherit garbage)
    RDResolveResult dirty;
    memset(&dirty, 0xAA, sizeof(dirty));

    rdtest_assert_true(rd_type_resolve_offset(ctx, &t, 0, 0, &dirty));
    rdtest_assert_streq(dirty.field.name, "x");
    rdtest_assert_eq(dirty.depth, 0);
    rdtest_assert(!dirty.item_idx.has_value, "stale item_idx leaked through");

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// ===========================================================================
// The coincidence-chain walk: the renderer's exact access pattern
// (mirrors items.c's _rd_data_chain_row: probe at floor 0 to find the
// chain's start, then probe.depth + k for link k, until depth < requested)
// ===========================================================================

static int test_type_resolve_offset_chain_walk(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    /* Point11 again. The chains this test walks, one head address each:
     *
     *  off 0 (root):  [first, x]      both start at byte 0 with Point11
     *  off 4 (y):     [y]             chain of ONE, at depth 1 (!), the
     *                                 case where row ordinal != depth
     *  off 8 (items): [items, [0], x] field, element, member: one byte
     */
    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point, ctx));

    RDTypeDef* point11 = rd_typedef_create_struct("Point11", ctx);
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "first", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(point11, "Point", "items", 10, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(point11, ctx));
    // clang-format on

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Point11", 0, RD_TYPE_NONE, ctx));

    // --- off 0: probe finds first (d0), link 1 is x (d1), link 2 is gone
    RDResolveResult probe = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &root, 0, 0, &probe));
    rdtest_assert_streq(probe.field.name, "first");
    rdtest_assert_eq(probe.depth, 0);

    RDResolveResult link1 = {0};
    rdtest_assert_true(
        rd_type_resolve_offset(ctx, &root, 0, probe.depth + 1, &link1));
    rdtest_assert_streq(link1.field.name, "x");
    rdtest_assert_eq(link1.depth, probe.depth + 1); // link exists

    RDResolveResult link2 = {0};
    rdtest_assert_true(
        rd_type_resolve_offset(ctx, &root, 0, probe.depth + 2, &link2));
    rdtest_assert(link2.depth < probe.depth + 2, "chain must end after x");

    // --- off 4: the chain is [y] alone, and it lives at depth 1.
    // This asymmetry (first row of the address, depth != 0) is the exact
    // shape that must render as one correctly-indented row, then exhaust
    RDResolveResult py = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &root, 4, 0, &py));
    rdtest_assert_streq(py.field.name, "y");
    rdtest_assert_eq(py.depth, 1); // NOT 0: reached through first@rel!=0

    RDResolveResult py1 = {0};
    rdtest_assert_true(
        rd_type_resolve_offset(ctx, &root, 4, py.depth + 1, &py1));
    rdtest_assert(py1.depth < py.depth + 1, "y is the whole chain");

    // --- off 8: field -> element -> member, three entities on one byte
    usize off_items = rd_typedef_size(point);

    RDResolveResult pi = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &root, off_items, 0, &pi));
    rdtest_assert_streq(pi.field.name, "items");
    rdtest_assert_eq(pi.depth, 0);
    rdtest_assert(!pi.item_idx.has_value, "stopped ABOVE the array");

    RDResolveResult pe = {0};
    rdtest_assert_true(
        rd_type_resolve_offset(ctx, &root, off_items, pi.depth + 1, &pe));
    rdtest_assert(pe.field.name == NULL, "elements are unnamed");
    rdtest_assert_streq(rd_typedef_name(pe.field.type.def), "Point");
    rdtest_assert_eq(pe.item_idx.value, 0);
    rdtest_assert_eq(pe.depth, pi.depth + 1);

    RDResolveResult px = {0};
    rdtest_assert_true(
        rd_type_resolve_offset(ctx, &root, off_items, pi.depth + 2, &px));
    rdtest_assert_streq(px.field.name, "x");
    rdtest_assert_eq(px.depth, pi.depth + 2);

    RDResolveResult pend = {0};
    rdtest_assert_true(
        rd_type_resolve_offset(ctx, &root, off_items, pi.depth + 3, &pend));
    rdtest_assert(pend.depth < pi.depth + 3, "chain ends at [0].x");

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_enum_is_solid(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // the contract names four solid kinds: primitive, pointer, union, enum.
    // The first three have tests; this is the fourth. An enum occupies its
    // base type's bytes but has no interior, cases are values, not fields
    RDTypeDef* color = rd_typedef_create_enum("Color", "u32", ctx);
    rdtest_assert(color, "failed to create enum");
    rdtest_assert_true(rd_typedef_add_enumval(color, "RED", 0, ctx));
    rdtest_assert_true(rd_typedef_add_enumval(color, "GREEN", 1, ctx));
    rdtest_assert_true(rd_typedef_register(color, ctx));

    RDType t;
    rdtest_assert_true(rd_type_init(&t, "Color", 0, RD_TYPE_NONE, ctx));

    RDResolveResult r = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &t, 0, RD_MAX_DEPTH, &r));
    rdtest_assert_streq(rd_typedef_name(r.field.type.def), "Color");
    rdtest_assert_eq(r.depth, 0);

    // an offset inside the enum's bytes is malformed, same as a primitive
    RDResolveResult bad = {0};
    rdtest_assert_false(rd_type_resolve_offset(ctx, &t, 2, 0, &bad));

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_offset_string_is_solid(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // THE one special case: char[n] / char16[n] are string literals -
    // solid for enumeration (never expanded into element rows), while
    // explicit offsets inside them still resolve. Modeled after
    // IMAGE_IMPORT_BY_NAME { u16 Hint; char Name[12]; }
    // clang-format off
    RDTypeDef* ibn = rd_typedef_create_struct("ImportByName", ctx);
    rdtest_assert_true(rd_typedef_add_member(ibn, "u16", "Hint", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(ibn, "char", "Name", 12, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(ibn, ctx));
    // clang-format on

    RDType root;
    rdtest_assert_true(
        rd_type_init(&root, "ImportByName", 0, RD_TYPE_NONE, ctx));

    // forcing past the string member stops AT the string member: the
    // named field is the honest terminus, never an anonymous char [0]
    RDResolveResult r = {0};
    rdtest_assert_true(
        rd_type_resolve_offset(ctx, &root, sizeof(u16), RD_MAX_DEPTH, &r));
    rdtest_assert_streq(r.field.name, "Name");
    rdtest_assert_eq(r.depth, 0);
    rdtest_assert(!r.item_idx.has_value, "never entered the string");

    // ...but containment still resolves: an explicit offset inside the
    // string yields its element (expansion declined, resolution intact)
    RDType str;
    rdtest_assert_true(rd_type_init(&str, "char", 12, RD_TYPE_NONE, ctx));

    RDResolveResult re = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &str, 5, 0, &re));
    rdtest_assert_eq(re.item_idx.value, 5);
    rdtest_assert_eq(re.depth, 0);

    // enumeration past the string root exhausts honestly
    RDResolveResult rx = {0};
    rdtest_assert_true(rd_type_resolve_offset(ctx, &str, 0, 1, &rx));
    rdtest_assert_eq(rx.depth, 0); // 0 < 1: nothing enumerable inside

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// clang-format off
static const RDTest K_TESTS[] = {
    {"primitives", test_primitives},
    {"type_init_unregistered", test_type_init_unregistered},
    {"struct", test_struct},
    {"struct_nested", test_struct_nested},
    {"struct_with_pointer_member", test_struct_with_pointer_member},
    {"array_primitives", test_array_primitives},
    {"array_count_one_size", test_array_count_one_size},
    {"array_of_struct", test_array_of_struct},
    {"array_of_pointers", test_array_of_pointers},
    {"pointer_size_consistency", test_pointer_size_consistency},
    {"typedef_resolve_offset_shallow", test_typedef_resolve_offset_shallow},
    {"typedef_resolve_offset_union_rejected", test_typedef_resolve_offset_union_rejected},
    {"type_resolve_offset_coincidence", test_type_resolve_offset_coincidence},
    {"type_resolve_offset_min_depth", test_type_resolve_offset_min_depth},
    {"type_resolve_offset_deep_nested", test_type_resolve_offset_deep_nested},
    {"type_resolve_offset_array", test_type_resolve_offset_array},
    {"type_resolve_offset_array_of_pointers", test_type_resolve_offset_array_of_pointers},
    {"type_resolve_offset_pointer_terminal", test_type_resolve_offset_pointer_terminal},
    {"type_resolve_offset_primitive_leaf", test_type_resolve_offset_primitive_leaf},
    {"type_resolve_offset_not_found", test_type_resolve_offset_not_found},
    {"type_resolve_offset_exhaustion_scalar", test_type_resolve_offset_exhaustion_scalar},
    {"type_resolve_offset_exhaustion_struct", test_type_resolve_offset_exhaustion_struct},
    {"type_resolve_offset_exhaustion_array", test_type_resolve_offset_exhaustion_array},
    {"type_resolve_offset_array_bounds", test_type_resolve_offset_array_bounds},
    {"type_resolve_offset_count_one_is_array", test_type_resolve_offset_count_one_is_array},
    {"type_resolve_offset_union_is_opaque", test_type_resolve_offset_union_is_opaque},
    {"type_resolve_offset_enum_is_solid", test_type_resolve_offset_enum_is_solid},
    {"type_resolve_offset_string_is_solid", test_type_resolve_offset_string_is_solid},
    {"type_resolve_offset_result_is_zeroed", test_type_resolve_offset_result_is_zeroed},
    {"type_resolve_offset_chain_walk", test_type_resolve_offset_chain_walk},
    {NULL, NULL},
};
// clang-format on

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("types", K_TESTS);
    rdtest_deinit();
    return result;
}
