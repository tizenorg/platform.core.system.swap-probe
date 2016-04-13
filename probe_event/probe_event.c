#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "gesture_probes.h"
//#include "keytouch.h"
//#include "orientation.h"

#define PROBES_LIST \
	X(PROBE_NAME(elm_gesture_layer_cb_set), "elm_gesture_layer_cb_set") \
	X(PROBE_NAME(ecore_event_evas_key_down), "ecore_event_evas_key_down") \
	X(PROBE_NAME(ecore_event_evas_key_up), "ecore_event_evas_key_up") \
	X(PROBE_NAME(ecore_event_evas_mouse_button_down), "ecore_event_evas_mouse_button_down") \
	X(PROBE_NAME(ecore_event_evas_mouse_button_up), "ecore_event_evas_mouse_button_up") \
	X(PROBE_NAME(ecore_event_evas_mouse_move), "ecore_event_evas_mouse_move") \
	X(PROBE_NAME(ecore_x_init), "ecore_x_init")


/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
#define X(func_name, orig_name) { & func_name, orig_name },

static struct probe_desc_t event_probes[] = {
	PROBES_LIST
};

#undef X

/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

static struct feature_desc_t feature = {
	.feature = 0xffffffff,
	.cnt = (PROBES_LIST),
	.probes = event_probes
};

#undef X

#undef PROBES_LIST

struct feature_desc_t *event_feature = &feature;
