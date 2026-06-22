#include "rdtest_helpers.h"
#include "rdtest.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RDTEST_PROJECT_FILE "project.rdx"

static const char RDTEST_BUFFER[RDTEST_BUFFER_SIZE] = {0};
static const char* g_samples_dir = NULL;
static RDContext* g_context = NULL;

static bool _rdtest_ext_matches(RDContext* ctx, const RDExternal* ext,
                                const RDTestExternal* e) {
    if(ext->kind != e->kind) return false;
    if(ext->address != e->address) return false;

    if(e->name) {
        const char* name = rd_get_name(ctx, ext->address);
        if(!name || strcmp(e->name, name) != 0) return false;
    }

    if(ext->ordinal.has_value != e->ordinal.has_value) return false;

    if(e->ordinal.has_value && ext->ordinal.value != e->ordinal.value)
        return false;

    return true;
}

static void _rdtest_load_plugins() {
    char paths[] = REDASM_PLUGIN_PATHS;
    char* path = strtok(paths, "|");

    while(path) {
        rd_module_load(path);
        path = strtok(NULL, "|");
    }
}

static int _rdtest_check_graph(RDContext* ctx, RDAddress address,
                               u32 expected_hash) {
    const RDFunction* f = rd_find_function(ctx, address);

    if(!f) {
        fprintf(stderr, "  TEST FAIL no function at 0x%08" PRIx64 "\n",
                address);
        return RDTEST_FAIL;
    }

    u32 hash = rd_function_get_hash(f);

    if(hash != expected_hash) {
        fprintf(stderr,
                "  TEST FAIL graph hash mismatch at 0x%08" PRIx64
                ": expected 0x%08x, got 0x%08x\n",
                address, expected_hash, hash);
        return RDTEST_FAIL;
    }

    return RDTEST_PASS;
}

static int _rdtest_run(RDTestSample* sample) {
    rdtest_assert_notnull(sample->ctx);

    if(sample->entry_point.has_value) {
        RDAddress ep;
        rdtest_assert_true(rd_get_entry_point(sample->ctx, &ep));

        if(ep != sample->entry_point.value) {
            fprintf(stderr,
                    "  TEST FAIL address mismatch: expected %" PRIx64
                    ", got %" PRIx64 "\n",
                    sample->entry_point.value, ep);

            return RDTEST_FAIL;
        }

        const RDFunction* f =
            rd_find_function(sample->ctx, sample->entry_point.value);
        rdtest_assert_notnull(f);
        rdtest_assert_eq(rd_function_is_noret(f), sample->entry_point.no_ret);
    }

    rdtest_assert_pass(rdtest_check_names(sample->ctx, sample->names));
    rdtest_assert_pass(rdtest_check_types(sample->ctx, sample->types));
    rdtest_assert_pass(rdtest_check_graphs(sample->ctx, sample->graphs));
    rdtest_assert_pass(rdtest_check_xrefs(sample->ctx, sample->xrefs));
    rdtest_assert_pass(rdtest_check_externals(sample->ctx, sample->externals));

    return RDTEST_PASS;
}

void rdtest_init(int argc, char** argv) {
    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[i], "--samples") && i + 1 < argc)
            g_samples_dir = argv[++i];
    }

    if(!g_samples_dir) g_samples_dir = getenv("REDASM_SAMPLES");

    const char* kb_paths[] = {REDASM_KB_PATH, NULL};
    RDInitParams params = {.kb_paths = kb_paths};
    rd_init(&params);
    _rdtest_load_plugins();
}

void rdtest_deinit(void) {
    if(g_context) rd_destroy(g_context);
    rd_deinit();
}

RDContext* rdtest_context_create(void) {
    RDTestResultSlice slice = rd_test_data(RDTEST_BUFFER, RDTEST_BUFFER_SIZE);
    if(rd_slice_is_empty(slice)) return NULL;

    RDAcceptParams params = {.mode = RD_AM_NEW};
    RDAcceptResult res = rd_accept(rd_slice_at(slice, 0), &params);
    return res.context; // NULL on failure
}

RDContext* rdtest_load_sample(const char* relpath, const char* loaderid,
                              const char* processorid) {
    if(!g_samples_dir) return NULL;

    if(g_context) {
        rd_destroy(g_context);
        g_context = NULL;
    }

    RDTestResultSlice slice =
        rd_test(rd_format("%s/%s", g_samples_dir, relpath));
    if(rd_slice_is_empty(slice)) return NULL;

    RDAcceptParams params = {.mode = RD_AM_NEW};
    RDAcceptResult res = rd_accept(rd_slice_at(slice, 0), &params);
    if(res.status != RD_ACCEPT_OK) return NULL;

    const RDLoaderPlugin* loader = rd_get_loader_plugin(res.context);
    if(!loader || strcmp(loader->id, loaderid) != 0) {
        fprintf(stderr,
                "  TEST FAIL loader mismatch: expected '%s', got '%s'\n",
                loaderid, loader ? loader->id : "(null)");
        rd_destroy(res.context);
        return NULL;
    }

    const RDProcessorPlugin* processor = rd_get_processor_plugin(res.context);
    if(!processor || strcmp(processor->id, processorid) != 0) {
        fprintf(stderr,
                "  TEST FAIL processor mismatch: expected '%s', got '%s'\n",
                processorid, processor ? processor->id : "(null)");
        rd_destroy(res.context);
        return NULL;
    }

    return res.context;
}

int rdtest_check_names(RDContext* ctx, const RDTestName* names) {
    const RDTestName* n = names;

    while(n && n->name) {
        const char* name = rd_get_name(ctx, n->address);

        if(!name || strcmp(n->name, name) != 0) {
            fprintf(stderr,
                    "  TEST FAIL name mismatch at 0x%08" PRIx64
                    ": expected '%s', got '%s'\n",
                    n->address, n->name, name ? name : "(null)");

            return RDTEST_FAIL;
        }

        n++;
    }

    return RDTEST_PASS;
}

int rdtest_check_types(RDContext* ctx, const RDTestType* types) {
    const RDTestType* t = types;

    while(t && !rd_type_is_void(&t->type)) {
        RDType currt;

        if(!rd_get_type(ctx, t->address, &currt)) {
            fprintf(stderr, "  TEST FAIL no type at 0x%08" PRIx64 "\n",
                    t->address);
            return RDTEST_FAIL;
        }

        if(!rd_type_equals(&currt, &t->type)) {
            fprintf(stderr, "  TEST FAIL type mismatch at 0x%08" PRIx64 "\n",
                    t->address);
            return RDTEST_FAIL;
        }

        t++;
    }

    return RDTEST_PASS;
}

int rdtest_check_graphs(RDContext* ctx, const RDTestGraph* graphs) {
    const RDTestGraph* g = graphs;

    while(g && g->hash) {
        if(_rdtest_check_graph(ctx, g->address, g->hash) != RDTEST_PASS)
            return RDTEST_FAIL;
        g++;
    }

    return RDTEST_PASS;
}

int rdtest_check_xrefs(RDContext* ctx, const RDTestXRef* xrefs) {
    const RDTestXRef* r = xrefs;

    while(r && r->ref.type) {
        RDXRefSlice slice = rd_get_xrefs_to(ctx, r->address, r->ref.type);
        bool found = false;

        const RDXRef* xref;
        rd_slice_each(xref, slice) {
            if(xref->address == r->ref.address) {
                found = true;
                break;
            }
        }

        if(!found) {
            fprintf(stderr,
                    "  TEST FAIL xref missing: 0x%08" PRIx64 " -> 0x%08" PRIx64
                    "\n",
                    r->ref.address, r->address);
            return RDTEST_FAIL;
        }

        r++;
    }

    return RDTEST_PASS;
}

int rdtest_check_externals(RDContext* ctx, const RDTestExternal* externals) {
    if(!externals) return RDTEST_PASS;

    RDExternalSlice slice = rd_get_all_externals(ctx, RD_EXT_NONE);
    const RDTestExternal* e = externals;

    while(e && e->kind != RD_EXT_NONE) {
        bool found = false;

        const RDExternal* ext;
        rd_slice_each(ext, slice) {
            if(_rdtest_ext_matches(ctx, ext, e)) {
                found = true;
                break;
            }
        }

        if(!found) {
            fprintf(stderr, "  TEST FAIL external mismatch: 0x%08" PRIx64 "\n",
                    e->address);
            return RDTEST_FAIL;
        }

        e++;
    }

    return RDTEST_PASS;
}

int rdtest_check_sample(RDTestSample* sample) {
    sample->ctx = rdtest_load_sample(sample->rel_path, sample->loader_id,
                                     sample->processor_id);
    g_context = sample->ctx;
    rd_disassemble(sample->ctx);
    rdtest_assert_pass(_rdtest_run(sample));

    // run again in project mode
    bool ok = rd_project_save(sample->ctx, RDTEST_PROJECT_FILE);
    rdtest_assert_true(ok);
    rd_destroy(sample->ctx);
    sample->ctx = NULL;
    g_context = NULL;

    RDAcceptResult res = rd_project_load(RDTEST_PROJECT_FILE, NULL);
    rdtest_assert_eq(res.status, RD_ACCEPT_OK);
    sample->ctx = res.context;
    g_context = res.context;
    remove(RDTEST_PROJECT_FILE);

    rdtest_assert_pass(_rdtest_run(sample));

    return RDTEST_PASS;
}
