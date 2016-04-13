#ifndef __EVENT_PROBES_LIST_H__
#define __EVENT_PROBES_LIST_H__

#define PROBES_LIST \
	X(PROBE_NAME(elm_gesture_layer_cb_set), "elm_gesture_layer_cb_set") \
	X(PROBE_NAME(ecore_event_evas_key_down), "ecore_event_evas_key_down") \
	X(PROBE_NAME(ecore_event_evas_key_up), "ecore_event_evas_key_up") \
	X(PROBE_NAME(ecore_event_evas_mouse_button_down), "ecore_event_evas_mouse_button_down") \
	X(PROBE_NAME(ecore_event_evas_mouse_button_up), "ecore_event_evas_mouse_button_up") \
	X(PROBE_NAME(ecore_event_evas_mouse_move), "ecore_event_evas_mouse_move") \
	X(PROBE_NAME(ecore_wl_init), "ecore_wl_init")


/* X-macros replaced with func id's enumeration */
#define X(func_name, orig_name)         \
	GET_FUNC_ID(func_name),

enum event_funcs_id_t {
	PROBES_LIST
};
#undef X


enum event_funcs_id_t get_event_feature_id(void *ptr);

#endif /* __EVENT_PROBES_LIST_H__ */
