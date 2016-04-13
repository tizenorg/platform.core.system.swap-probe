#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "da_evas_gl.h"

#define PROBES_LIST \
	X(PROBE_NAME(evas_gl_free), "evas_gl_free") \
	X(PROBE_NAME(evas_gl_config_free), "evas_gl_config_free") \
	X(PROBE_NAME(evas_gl_surface_destroy), "evas_gl_surface_destroy") \
	X(PROBE_NAME(evas_gl_context_destroy), "evas_gl_context_destroy") \
	X(PROBE_NAME(evas_gl_new), "eval_gl_new") \
	X(PROBE_NAME(evas_gl_config_new), "evas_gl_config_new") \
	X(PROBE_NAME(evas_gl_surface_create), "evas_gl_surface_create") \
	X(PROBE_NAME(evas_gl_context_create), "evas_gl_context_create") \
	X(PROBE_NAME(evas_gl_make_current), "evas_gl_make_current") \
	X(PROBE_NAME(evas_gl_string_query), "evas_gl_string_query") \
	X(PROBE_NAME(evas_gl_proc_address_get), "evas_gl_proc_address_get") \
	X(PROBE_NAME(evas_gl_native_surface_get), "evas_gl_native_surface_get") \
	X(PROBE_NAME(evas_gl_api_get), "evas_gl_api_get") \
	X(PROBE_NAME(elm_glview_gl_api_get), "elm_glview_gl_api_get") \
	X(PROBE_NAME(evas_gl_context_api_get), "evas_gl_context_api_get")



/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
/* For target binaries probes */
#define X(func_name, orig_name) { & func_name, orig_name, GT_TARGET_PROBE },

static struct probe_desc_t graphics_probes[] = {
	PROBES_LIST
};

#undef X

/* For all binaries probes */
#define X(func_name, orig_name) { & func_name, orig_name, GT_ALWAYS_PROBE },

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
