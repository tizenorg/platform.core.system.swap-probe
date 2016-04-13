#ifndef __EVENT_PROBES_LIST_H__
#define __EVENT_PROBES_LIST_H__

#define PROBES_LIST \
	X(PROBE_NAME(elm_gesture_layer_cb_set), "elm_gesture_layer_cb_set", GT_TARGET_PROBE) \
	X(PROBE_NAME(ecore_event_evas_key_down), "ecore_event_evas_key_down", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(ecore_event_evas_key_up), "ecore_event_evas_key_up", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(ecore_event_evas_mouse_button_down), "ecore_event_evas_mouse_button_down", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(ecore_event_evas_mouse_button_up), "ecore_event_evas_mouse_button_up", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(ecore_event_evas_mouse_move), "ecore_event_evas_mouse_move", GT_ALWAYS_PROBE) \
	X(PROBE_NAME(ecore_wl_init), "ecore_wl_init", GT_ALWAYS_PROBE | GT_INVERTED_PROBE)


/* X-macros replaced with func id's enumeration */
#define X(func_name, orig_name, type)   \
	GET_FUNC_ID(func_name),

enum event_funcs_id_t {
	PROBES_LIST
};
#undef X


enum event_funcs_id_t get_event_feature_id(void *ptr);

#endif /* __EVENT_PROBES_LIST_H__ */
