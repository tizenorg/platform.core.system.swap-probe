#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "gesture_probes.h"
//#include "keytouch.h"
//#include "orientation.h"
#include "daprobe.h"

#include "event_probes_list.h"



/* X-macros replaced with function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name, type)   \
	void func_name(void);

PROBES_LIST

#undef X


/* This function returns function's id by its pointer */
#define X(func_name, orig_name, type)           \
	if (ptr == & func_name)                 \
		return GET_FUNC_ID(func_name);

enum event_funcs_id_t get_event_feature_id(void *ptr)
{
	PROBES_LIST

	PRINTERR("Invalid function pointer %p at <%s>!\n", ptr, __func__);

	return 0;
}

#undef X


/* X-macros replaced with structures defenitions */
/* Used only for probes from target file */
#define X(func_name, orig_name, type)           \
        { (ElfW(Addr))& func_name, orig_name, type, NULL },

static struct probe_desc_t event_probes[] = {
	PROBES_LIST
};

#undef X


/* X-macros replaced with counted additions */
#define X(func_name, orig_name, type) +1

struct feature_desc_t event_feature = {
	.feature = FL_USER_EVENT,
	.cnt = (PROBES_LIST),
	.probes = event_probes
};

#undef X

#undef PROBES_LIST
