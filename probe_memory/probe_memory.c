#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "libdamemalloc.h"
//#include "libdamemmange.h"

#define PROBES_LIST \
	X(PROBE_NAME(malloc), "malloc") \
	X(PROBE_NAME(free), "free") \
	X(PROBE_NAME(realloc), "realloc") \
	X(PROBE_NAME(calloc), "calloc") \
	X(PROBE_NAME(memset), "memset") \
	X(PROBE_NAME(memcmp), "memcmp") \
	X(PROBE_NAME(memcpy), "memcpy")



/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
#define X(func_name, orig_name) { & func_name, orig_name },

static struct probe_desc_t memory_probes[] = {
	PROBES_LIST
};

#undef X

/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

struct feature_desc_t memory_feature = {
	.feature = FL_MEMORY_ALLOC_PROBING,
	.cnt = (PROBES_LIST),
	.probes = memory_probes
};

struct feature_desc_t memory_feature_always = {
	.feature = FL_MEMORY_ALLOC_ALWAYS_PROBING,
	.cnt = (PROBES_LIST),
	.probes = memory_probes
};

#undef X

#undef PROBES_LIST
