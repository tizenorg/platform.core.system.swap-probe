#ifndef __GRAPHICS_PROBES_LIST_H__
#define __GRAPHICS_PROBES_LIST_H__

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

#define X(func_name, orig_name)         \
	GET_FUNC_ID(func_name),

enum graphics_funcs_id_t {
	PROBES_LIST
};
#undef X

enum graphics_funcs_id_t get_graphics_feature_id(void *ptr);

#endif /* __GRAPHICS_PROBES_LIST_H__ */
