#include <stddef.h>
#include "api_names_global.h"
#include "binproto.h"
//#include "libdamemalloc.h"
//#include "libdamemmange.h"
#include "daprobe.h"

#include "memory_probes_list.h"



/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
/* Address of functions are the same for always and non-always probes */
/* TODO Create separated handlers for mem that only increments total mem */
#define X(func_name, orig_name, type) \
	void func_name(void);

PROBES_LIST_MEMORY
PROBES_LIST_MEMORY_ALWAYS

#undef X


/* This function returns function's id by its pointer */
#define X(func_name, orig_name, type)           \
	if (ptr == & func_name)                 \
		return GET_FUNC_ID(func_name);

enum memory_funcs_id_t get_memory_feature_id(void *ptr)
{
	PROBES_LIST

	PRINTERR("Invalid function pointer %p at <%s>!\n", ptr, __func__);

	return 0;
}

enum memory_funcs_id_t get_memory_feature_always_id(void *ptr)
        __attribute__((alias("get_memory_feature_id")));

#undef X


/* X-macros replaced by structures defenitions */
/* For target binaries probes */
#define X(func_name, orig_name, type)   \
	{ (ElfW(Addr))& func_name, orig_name, type, NULL },

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
