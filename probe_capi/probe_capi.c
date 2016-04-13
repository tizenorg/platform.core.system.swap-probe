#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "capi_appfw.h"

#define PROBES_LIST \
	X(PROBE_NAME(ui_app_main), "ui_app_main")


/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
#define X(func_name, orig_name) { & func_name, orig_name },

static struct probe_desc_t capi_probes[] = {
	PROBES_LIST
};

#undef X

/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

struct feature_desc_t capi_feature = {
	.feature = 0xffffffff,
	.cnt = (PROBES_LIST),
	.probes = capi_probes
};

#undef X

#undef PROBES_LIST
