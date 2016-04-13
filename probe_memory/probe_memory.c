#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "libdamemalloc.h"
//#include "libdamemmange.h"

#define PROBES_LIST_MEMORY \
	X(PROBE_NAME(malloc), "malloc", GT_TARGET_PROBE | GT_INVERTED_PROBE) \
	X(PROBE_NAME(free), "free", GT_TARGET_PROBE | GT_INVERTED_PROBE) \
	X(PROBE_NAME(realloc), "realloc", GT_TARGET_PROBE | GT_INVERTED_PROBE) \
	X(PROBE_NAME(calloc), "calloc", GT_TARGET_PROBE | GT_INVERTED_PROBE) \
	X(PROBE_NAME(memset), "memset", GT_TARGET_PROBE) \
	X(PROBE_NAME(memcmp), "memcmp", GT_TARGET_PROBE) \
	X(PROBE_NAME(memcpy), "memcpy", GT_TARGET_PROBE)

#define PROBES_LIST_MEMORY_ALWAYS \
	X(PROBE_NAME(malloc), "malloc", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(free), "free", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(realloc), "realloc", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(calloc), "calloc", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(memset), "memset", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(memcmp), "memcmp", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(memcpy), "memcpy", GT_ALWAYS_PROBE)



/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
/* Address of functions are the same for always and non-always probes */
/* TODO Create separated handlers for mem that only increments total mem */
#define X(func_name, orig_name, type) \
	void func_name(void);

PROBES_LIST_MEMORY

#undef X


/* X-macros replaced by structures defenitions */
#define X(func_name, orig_name, type) { & func_name, orig_name, type },

static struct probe_desc_t memory_probes[] = {
	PROBES_LIST_MEMORY
};

static struct probe_desc_t memory_always_probes[] = {
	PROBES_LIST_MEMORY_ALWAYS
};

#undef X

/* X-macros replaced by counted additions */
#define X(func_name, orig_name, type) +1

struct feature_desc_t memory_feature = {
	.feature = FL_MEMORY_ALLOC_PROBING,
	.cnt = (PROBES_LIST_MEMORY),
	.probes = memory_probes
};

struct feature_desc_t memory_feature_always = {
	.feature = FL_MEMORY_ALLOC_ALWAYS_PROBING,
	.cnt = (PROBES_LIST_MEMORY_ALWAYS),
	.probes = memory_always_probes
};

#undef X

#undef PROBES_LIST
