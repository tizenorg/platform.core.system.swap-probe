#include <stddef.h>
#include "api_names_global.h"
#include "binproto.h"
//#include "libdasocket.h"
#include "daprobe.h"

#include "socket_probes_list.h"



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


/* This function returns function's id by its pointer */
#define X(func_name, orig_name)                 \
	if (ptr == & func_name)                 \
		return GET_FUNC_ID(func_name);

enum network_funcs_id_t get_network_feature_id(void *ptr)
{
	PROBES_LIST

	PRINTERR("Invalid function pointer %p at <%s>!\n", ptr, __func__);

	return 0;
}

enum network_funcs_id_t get_network_feature_always_id(void *ptr)
        __attribute__((alias("get_network_feature_id")));

#undef X


/* X-macros replaced by structures defenitions */
/* For target binaries probes */
#define X(func_name, orig_name)         \
	{ (ElfW(Addr))& func_name, orig_name, GT_TARGET_PROBE, NULL },

static struct probe_desc_t network_probes[] = {
	PROBES_LIST
};

#undef X

/* For all binaries probes */
#define X(func_name, orig_name)     \
	{ (ElfW(Addr))& CONCAT(func_name, _always), orig_name,  \
	   GT_ALWAYS_PROBE, NULL },

static struct probe_desc_t network_always_probes[] = {
	PROBES_LIST
};

#undef X


/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

struct feature_desc_t network_feature = {
	.feature = FL_NETWORK_API_PROBING,
	.cnt = (PROBES_LIST),
	.probes = network_probes
};

struct feature_desc_t network_feature_always = {
	.feature = FL_NETWORK_API_ALWAYS_PROBING,
	.cnt = (PROBES_LIST),
	.probes = network_always_probes
};

#undef X

#undef PROBES_LIST
