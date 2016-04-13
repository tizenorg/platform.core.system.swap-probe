#ifndef __CAPI_PROBES_LIST_H__
#define __CAPI_PROBES_LIST_H__


#define PROBES_LIST \
	X(PROBE_NAME(ui_app_main), "ui_app_main")


#define X(func_name, orig_name)         \
	GET_FUNC_ID(func_name),
enum capi_funcs_id_t {
	PROBES_LIST
};
#undef X

enum capi_funcs_id_t get_capi_feature_id(void *ptr);

#endif /* __CAPI_PROBES_LIST_H__ */
