#ifndef __UI_PROBES_LIST_H__
#define __UI_PROBES_LIST_H__

#define PROBES_LIST \
	X(PROBE_NAME(evas_object_show), "evas_object_show")

#define X(func_name, orig_name)         \
	GET_FUNC_ID(func_name),

enum ui_funcs_id_t {
	PROBES_LIST
};
#undef X

enum ui_funcs_id_t get_ui_feature_id(void *ptr);


#endif /* __UI_PROBES_LIST_H__ */
