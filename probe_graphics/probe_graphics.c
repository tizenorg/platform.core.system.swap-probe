#include <stddef.h>
#include "api_names_global.h"
#include "binproto.h"
//#include "da_evas_gl.h"
#include "graphics_probes_list.h"


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

static struct probe_desc_t graphics_probes[] = {
	PROBES_LIST
};

#undef X

/* For all binaries probes */
#define X(func_name, orig_name)     \
	{ (ElfW(Addr)) & CONCAT(func_name, _always), orig_name,      \
	  GT_ALWAYS_PROBE, NULL },

static struct probe_desc_t graphics_always_probes[] = {
	PROBES_LIST
};

#undef X


/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

struct feature_desc_t graphics_feature = {
	.feature = FL_OPENGL_API_PROBING,
	.cnt = (PROBES_LIST),
	.probes = graphics_probes
};

struct feature_desc_t graphics_feature_always = {
	.feature = FL_OPENGL_API_ALWAYS_PROBING,
	.cnt = (PROBES_LIST),
	.probes = graphics_always_probes
};

#undef X

#undef PROBES_LIST
