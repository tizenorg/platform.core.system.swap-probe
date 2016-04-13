#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "capi_capture.h"

#define PROBES_LIST \
	X(PROBE_NAME(evas_object_show), "evas_object_show")


/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
#define X(func_name, orig_name) { & func_name, orig_name },

static struct probe_desc_t ui_probes[] = {
	PROBES_LIST
};

#undef X

/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

static struct feature_desc_t feature = {
	.feature = 0xffffffff,
	.cnt = (PROBES_LIST),
	.probes = ui_probes
};

#undef X

#undef PROBES_LIST

struct feature_desc_t *ui_feature = &feature;
