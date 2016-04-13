#ifndef __KEYTOUCH_H__
#define __KEYTOUCH_H__

Eina_Bool PROBE_NAME(ecore_event_evas_key_down)(void *data, int type, void *event);
Eina_Bool PROBE_NAME(ecore_event_evas_key_up)(void *data, int type, void *event);
Eina_Bool PROBE_NAME(ecore_event_evas_mouse_button_down)(void *data, int type, void *event);
Eina_Bool PROBE_NAME(ecore_event_evas_mouse_button_up)(void *data, int type, void *event);
Eina_Bool PROBE_NAME(ecore_event_evas_mouse_move)(void *data, int type, void *event);
void mouse_key_init(void);

#endif /* __KEYTOUCH_H__ */
