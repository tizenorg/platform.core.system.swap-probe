#include <stddef.h>
#include "api_names_global.h"
#include "binproto.h"
//#include "libdasync.h"
//#include "libdathread.h"
#include "thread_probes_list.h"



/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X

#define X(func_name, orig_name) \
	void CONCAT(func_name, _always)(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
/* For target binaries probes */
#define X(func_name, orig_name)         \
	{ (ElfW(Addr))& func_name, orig_name, GT_TARGET_PROBE, NULL },

static struct probe_desc_t thread_probes[] = {
	PROBES_LIST
};

#undef X

/* For all binaries probes */
#define X(func_name, orig_name)     \
	{ (ElfW(Addr))& CONCAT(func_name, _always), orig_name,  \
	  GT_ALWAYS_PROBE, NULL },

static struct probe_desc_t thread_always_probes[] = {
	PROBES_LIST
};

#undef X


/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

struct feature_desc_t thread_feature = {
	.feature = FL_THREAD_API_PROBING,
	.cnt = (PROBES_LIST),
	.probes = thread_probes
};

struct feature_desc_t thread_feature_always = {
	.feature = FL_THREAD_API_ALWAYS_PROBING,
	.cnt = (PROBES_LIST),
	.probes = thread_always_probes
};

#undef X

#undef PROBES_LIST
