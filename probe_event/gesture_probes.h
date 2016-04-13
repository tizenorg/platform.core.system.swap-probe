#ifndef __GESTURE_PROBES_H__
#define __GESTURE_PROBES_H__

void PROBE_NAME(elm_gesture_layer_cb_set)(Evas_Object *obj, Elm_Gesture_Type idx,
			      Elm_Gesture_State cb_type, Elm_Gesture_Event_Cb cb,
			      void *data);

#endif /* __GESTURE_PROBES_H__ */
