#pragma once

#include <redasm/redasm.h>

#define RDTEST_BUFFER_SIZE 256

typedef struct RDTestGraph {
    RDAddress address;
    u32 hash;
} RDTestGraph;

typedef struct RDTestName {
    RDAddress address;
    const char* name;
} RDTestName;

typedef struct RDTestType {
    RDAddress address;
    RDType type;
} RDTestType;

typedef struct RDTestXRef {
    RDAddress address;
    RDXRef ref;
} RDTestXRef;

typedef struct RDTestExternal {
    RDExternalKind kind;
    RDAddress address;
    const char* module;
    const char* name;

    struct {
        bool has_value;
        u32 value;
    } ordinal;
} RDTestExternal;

typedef struct RDTestSample {
    RDContext* ctx;
    const char* rel_path;
    const char* loader_id;
    const char* processor_id;

    struct {
        RDAddress value;
        bool has_value;
        bool no_ret;
    } entry_point;

    const RDTestName* names;
    const RDTestType* types;
    const RDTestGraph* graphs;
    const RDTestXRef* xrefs;
    const RDTestExternal* externals;
} RDTestSample;

void rdtest_init(int argc, char** argv);
void rdtest_deinit(void);
RDContext* rdtest_context_create(void);
RDContext* rdtest_load_sample(const char* relpath, const char* loaderid,
                              const char* processorid);
int rdtest_check_names(RDContext* ctx, const RDTestName* names);
int rdtest_check_types(RDContext* ctx, const RDTestType* types);
int rdtest_check_graphs(RDContext* ctx, const RDTestGraph* graphs);
int rdtest_check_xrefs(RDContext* ctx, const RDTestXRef* xrefs);
int rdtest_check_externals(RDContext* ctx, const RDTestExternal* xrefs);
int rdtest_check_sample(RDTestSample* sample);
