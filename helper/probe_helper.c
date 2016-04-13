#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "daforkexec_probes.h"

#define PROBES_LIST \
	X(PROBE_NAME(execl), "execl") \
	X(PROBE_NAME(execlp), "execlp") \
	X(PROBE_NAME(execle), "execle") \
	X(PROBE_NAME(execv), "execv") \
	X(PROBE_NAME(execvp), "execvp") \
	X(PROBE_NAME(execve), "execve") \
	X(PROBE_NAME(execvpe), "execvpe") \
	X(PROBE_NAME(fork), "fork")


/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
#define X(func_name, orig_name) { & func_name, orig_name, GT_TARGET_PROBE },

static struct probe_desc_t helper_probes[] = {
	PROBES_LIST
};

#undef X

/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

struct feature_desc_t helper_feature = {
	.feature = 0xffffffff,
	.cnt = (PROBES_LIST),
	.probes = helper_probes
};

#undef X

#undef PROBES_LIST
