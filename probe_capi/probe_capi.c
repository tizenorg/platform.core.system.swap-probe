#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "capi_appfw.h"
#include "capi_probes_list.h"

/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
/* Used only for probes from target file */
#define X(func_name, orig_name)         \
        { (ElfW(Addr)) & func_name, orig_name, GT_TARGET_PROBE, NULL },

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
