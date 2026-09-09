#include "rdtest.h"
#include "rdtest_helpers.h"

typedef struct RDTestTypeShape {
    const char* name;
    usize count;
    RDTypeModifier mod;
    usize size;
} RDTestTypeShape;

static void _mk_point(RDContext* ctx) {
    // clang-format off
    RDTypeDef* point = rd_typedef_create_struct("Point", ctx);
    rd_typedef_add_member(point, "u32", "x", 0, RD_TYPE_NONE, ctx);
    rd_typedef_add_member(point, "u32", "y", 0, RD_TYPE_NONE, ctx);
    rd_typedef_register(point, ctx);
    // clang-format on
}

static void _mk_point11(RDContext* ctx) {
    _mk_point(ctx);

    // clang-format off
    RDTypeDef* point11 = rd_typedef_create_struct("Point11", ctx);
    rd_typedef_add_member(point11, "Point", "first", 0, RD_TYPE_NONE, ctx);
    rd_typedef_add_member(point11, "Point", "items", 10, RD_TYPE_NONE, ctx);
    rd_typedef_register(point11, ctx);
    // clang-format on
}

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

static int test_type_resolve_chain_coincidence(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");
    _mk_point11(ctx);

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Point11", 0, RD_TYPE_NONE, ctx));

    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &root, 0);
    rdtest_assert_eq(s.length, 2);

    rdtest_assert_streq(s.data[0].field.name, "first");
    rdtest_assert_streq(rd_typedef_name(s.data[0].field.type.def), "Point");
    rdtest_assert_eq(s.data[0].depth, 0);
    rdtest_assert_true(s.data[0].at_offset);
    rdtest_assert(!s.data[0].item_idx.has_value, "no array crossed");

    rdtest_assert_streq(s.data[1].field.name, "x");
    rdtest_assert_streq(rd_typedef_name(s.data[1].field.type.def), "u32");
    rdtest_assert_eq(s.data[1].depth, 1);
    rdtest_assert_true(s.data[1].at_offset);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_chain_passthrough(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");
    _mk_point11(ctx);

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Point11", 0, RD_TYPE_NONE, ctx));

    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &root, 4);
    rdtest_assert_eq(s.length, 2);

    rdtest_assert_streq(s.data[0].field.name, "first");
    rdtest_assert_false(s.data[0].at_offset); // begins at 0, not at 4

    rdtest_assert_streq(s.data[1].field.name, "y");
    rdtest_assert_eq(s.data[1].depth, 1);
    rdtest_assert_true(s.data[1].at_offset);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_chain_array_passthrough(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");
    _mk_point11(ctx);

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Point11", 0, RD_TYPE_NONE, ctx));

    // items[2].y == 8 + (2 * 8) + 4 == 28: `items` crossed at a nonzero
    // offset, element 2 crossed at a nonzero offset, y starts there
    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &root, 28);
    rdtest_assert_eq(s.length, 3);

    rdtest_assert_streq(s.data[0].field.name, "items");
    rdtest_assert_false(s.data[0].at_offset);

    rdtest_assert(!s.data[1].field.name, "an element has no member name");
    rdtest_assert_true(s.data[1].item_idx.has_value);
    rdtest_assert_eq(s.data[1].item_idx.value, 2);
    rdtest_assert_false(s.data[1].at_offset);

    rdtest_assert_streq(s.data[2].field.name, "y");
    rdtest_assert_true(s.data[2].at_offset);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_chain_element_head(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");
    _mk_point(ctx);

    RDType arr;
    rdtest_assert_true(rd_type_init(&arr, "Point", 10, RD_TYPE_NONE, ctx));

    // element 3's own edge: the element and its first member both start
    // there, so both are answers
    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &arr, 3 * 8ULL);
    rdtest_assert_eq(s.length, 2);

    rdtest_assert_eq(s.data[0].item_idx.value, 3);
    rdtest_assert_streq(rd_typedef_name(s.data[0].field.type.def), "Point");
    rdtest_assert_eq(s.data[0].field.type.count, 0); // the ELEMENT, not [10]
    rdtest_assert_eq(s.data[0].depth, 0);
    rdtest_assert_true(s.data[0].at_offset);

    rdtest_assert_streq(s.data[1].field.name, "x");
    rdtest_assert_eq(s.data[1].depth, 1);
    rdtest_assert_true(s.data[1].at_offset);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_chain_primitive_leaf(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    RDType t;
    rdtest_assert_true(rd_type_init(&t, "u32", 0, RD_TYPE_NONE, ctx));

    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &t, 0);
    rdtest_assert_eq(s.length, 1);
    rdtest_assert_streq(rd_typedef_name(s.data[0].field.type.def), "u32");
    rdtest_assert_eq(s.data[0].depth, 0);
    rdtest_assert_true(s.data[0].at_offset);
    rdtest_assert(!s.data[0].item_idx.has_value, "no array involved");

    // an offset inside a solid type is malformed input
    RDResolveResultSlice bad = rd_type_resolve_chain(ctx, &t, 2);
    rdtest_assert_eq(bad.length, 0);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_chain_pointer_terminal(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");
    _mk_point(ctx);

    RDType ptr;
    rdtest_assert_true(rd_type_init(&ptr, "Point", 0, RD_TYPE_PTR, ctx));

    // a pointer is opaque: its target is not part of this schema
    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &ptr, 0);
    rdtest_assert_eq(s.length, 1);
    rdtest_assert_eq(s.data[0].field.type.mod, RD_TYPE_PTR);
    rdtest_assert_eq(s.data[0].depth, 0);

    RDResolveResultSlice bad = rd_type_resolve_chain(ctx, &ptr, 1);
    rdtest_assert_eq(bad.length, 0);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_chain_union_is_opaque(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    // clang-format off
    RDTypeDef* u = rd_typedef_create_union("U", ctx);
    rdtest_assert_true(rd_typedef_add_member(u, "u32", "a", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(u, "u16", "b", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(u, ctx));
 
    RDTypeDef* holder = rd_typedef_create_struct("Holder", ctx);
    rdtest_assert_true(rd_typedef_add_member(holder, "u32", "k", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_add_member(holder, "U", "v", 0, RD_TYPE_NONE, ctx));
    rdtest_assert_true(rd_typedef_register(holder, ctx));
    // clang-format on

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Holder", 0, RD_TYPE_NONE, ctx));

    // the union is a solid leaf: the chain stops at `v`, never inside it
    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &root, sizeof(u32));
    rdtest_assert_eq(s.length, 1);
    rdtest_assert_streq(s.data[0].field.name, "v");
    rdtest_assert_eq(s.data[0].depth, 0);

    // an offset inside the union body is malformed input
    RDResolveResultSlice bad =
        rd_type_resolve_chain(ctx, &root, sizeof(u32) + 2);
    rdtest_assert_eq(bad.length, 0);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_chain_string_is_solid(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");

    RDType str;
    rdtest_assert_true(rd_type_init(&str, "char", 8, RD_TYPE_NONE, ctx));

    /*
     * char[n] renders as one literal, so rd_i_type_has_more reports nothing
     * enumerable inside it: the chain is the first element and stops.
     */
    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &str, 0);
    rdtest_assert_eq(s.length, 1);
    rdtest_assert_eq(s.data[0].depth, 0);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// ===========================================================================
// Malformed input: clean failure, never a fabricated answer
// ===========================================================================

static int test_type_resolve_chain_array_bounds(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");
    _mk_point(ctx);

    RDType arr;
    rdtest_assert_true(rd_type_init(&arr, "Point", 10, RD_TYPE_NONE, ctx));

    // element 10 of a [10] array does not exist: never "succeed" with
    // item_idx == 10
    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &arr, 10 * 8ULL);
    rdtest_assert_eq(s.length, 0);

    RDResolveResultSlice s2 = rd_type_resolve_chain(ctx, &arr, 999);
    rdtest_assert_eq(s2.length, 0);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_chain_not_found(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");
    _mk_point(ctx);

    RDType t;
    rdtest_assert_true(rd_type_init(&t, "Point", 0, RD_TYPE_NONE, ctx));

    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &t, 100);
    rdtest_assert_eq(s.length, 0);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

static int test_type_resolve_chain_count_one_is_array(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");
    _mk_point(ctx);

    RDType arr1;
    rdtest_assert_true(rd_type_init(&arr1, "Point", 1, RD_TYPE_NONE, ctx));

    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &arr1, 0);
    rdtest_assert_eq(s.length, 2);
    rdtest_assert_true(s.data[0].item_idx.has_value); // the array WAS crossed
    rdtest_assert_eq(s.data[0].item_idx.value, 0);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// ===========================================================================
// Buffer contract
// ===========================================================================

static int test_type_resolve_chain_is_cleared(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");
    _mk_point11(ctx);

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Point11", 0, RD_TYPE_NONE, ctx));

    // a three-entry chain must not leave residue behind a shorter one
    RDResolveResultSlice deep = rd_type_resolve_chain(ctx, &root, 28);
    rdtest_assert_eq(deep.length, 3);

    RDResolveResultSlice shallow = rd_type_resolve_chain(ctx, &root, 0);
    rdtest_assert_eq(shallow.length, 2);

    // and a failure leaves nothing readable
    RDResolveResultSlice bad = rd_type_resolve_chain(ctx, &root, 9999);
    rdtest_assert_eq(bad.length, 0);

    rd_destroy(ctx);
    return RDTEST_PASS;
}

// ===========================================================================
// The renderer's access pattern: rows come from at_offset entries only
// ===========================================================================

static int test_type_resolve_chain_renderer_filter(void) {
    RDContext* ctx = rdtest_context_create();
    rdtest_assert(ctx, "failed to create test context");
    _mk_point11(ctx);

    RDType root;
    rdtest_assert_true(rd_type_init(&root, "Point11", 0, RD_TYPE_NONE, ctx));

    // off 0: two rows (first, x), both start here
    RDResolveResultSlice s = rd_type_resolve_chain(ctx, &root, 0);
    usize rows = 0;
    for(usize i = 0; i < s.length; i++)
        if(s.data[i].at_offset) rows++;
    rdtest_assert_eq(rows, 2);

    // off 4: one row (y), `first` began elsewhere
    s = rd_type_resolve_chain(ctx, &root, 4);
    rows = 0;
    for(usize i = 0; i < s.length; i++)
        if(s.data[i].at_offset) rows++;
    rdtest_assert_eq(rows, 1);

    // off 28: one row (y), items and element 2 both began elsewhere
    s = rd_type_resolve_chain(ctx, &root, 28);
    rows = 0;
    for(usize i = 0; i < s.length; i++)
        if(s.data[i].at_offset) rows++;
    rdtest_assert_eq(rows, 1);

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
    {"type_resolve_chain_coincidence", test_type_resolve_chain_coincidence},
    {"type_resolve_chain_passthrough", test_type_resolve_chain_passthrough},
    {"type_resolve_chain_array_passthrough", test_type_resolve_chain_array_passthrough},
    {"type_resolve_chain_element_head", test_type_resolve_chain_element_head},
    {"type_resolve_chain_primitive_leaf", test_type_resolve_chain_primitive_leaf},
    {"type_resolve_chain_pointer_terminal", test_type_resolve_chain_pointer_terminal},
    {"type_resolve_chain_union_is_opaque", test_type_resolve_chain_union_is_opaque},
    {"type_resolve_chain_string_is_solid", test_type_resolve_chain_string_is_solid},
    {"type_resolve_chain_array_bounds", test_type_resolve_chain_array_bounds},
    {"type_resolve_chain_not_found", test_type_resolve_chain_not_found},
    {"type_resolve_chain_count_one_is_array", test_type_resolve_chain_count_one_is_array},
    {"type_resolve_chain_is_cleared", test_type_resolve_chain_is_cleared},
    {"type_resolve_chain_renderer_filter", test_type_resolve_chain_renderer_filter},
    {NULL, NULL},
};
// clang-format on

int main(int argc, char** argv) {
    rdtest_init(argc, argv);
    int result = rdtest_run("types", K_TESTS);
    rdtest_deinit();
    return result;
}
