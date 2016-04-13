/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 * Anastasia Lyupa <a.lyupa@samsung.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Contributors:
 * - S-Core Co., Ltd
 * - Samsung RnD Institute Russia
 *
 */

//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//#include <fcntl.h>
//#include <dlfcn.h>
//#include <unistd.h>
//#include <errno.h>
#include <Eina.h>
#include <Ecore_Input.h>
#include <Evas.h>
//#include <linux/input.h>

#include "daprobe.h"
#include "dahelper.h"
#include "probeinfo.h"
#include "da_event.h"

#include "binproto.h"
#include "real_functions.h"
#include "event_probes_list.h"
#include "probe_event.h"


bool touch_pressed = false;
static Ecore_Event_Handler *ecore_event_evas_handlers[5];

#define PACK_HW_EVENT(API_ID, _EVENTTYPE, _DETAILTYPE, _X, _Y, _KEYCODE, _EXTRA, \
	_ARGDATA, _ARGTYPE, _ARGEVENT)												\
	do {																		\
		inc_current_event_index();												\
		PREPARE_LOCAL_BUF();													\
		PACK_COMMON_BEGIN(MSG_PROBE_UIEVENT, API_ID, "pdp",					\
				  voidp_to_uint64(_ARGDATA), _ARGTYPE,	\
				  voidp_to_uint64(_ARGEVENT));		\
		/* TODO Check if address is really unused here */					   \
		PACK_COMMON_END('c', 0, 0, 0, (uint64_t)0xffffffff);							\
		PACK_UIEVENT(_EVENTTYPE, _DETAILTYPE, _X, _Y, _KEYCODE, _EXTRA);		\
		FLUSH_LOCAL_BUF();														\
	} while (0)

Eina_Bool PROBE_NAME(ecore_event_evas_key_down)(void *data, int type, void *event)
{
	/* TODO Support old preload */
//	static Eina_Bool (*ecore_event_evas_key_downp)(void *data, int type, void *event);
//	Eina_Bool (*ecore_event_evas_key_downp)(void *data, int type, void *event);

//	GET_REAL_FUNC(ecore_event_evas_key_down, LIBECORE_INPUT_EVAS);

//	ecore_event_evas_key_downp = (void *)GET_ORIG_FUNC(event_feature,
//						     ecore_event_evas_key_down);

	if(isOptionEnabled(OPT_EVENT))
	{
		if(event != NULL)
		{
			Ecore_Event_Key* pEv = (Ecore_Event_Key*)event;
			if(strcasestr(pEv->keyname, "volume") == NULL)
			{
				PACK_HW_EVENT(API_ID_ecore_event_evas_key_down,
					      _EVENT_KEY, _KEY_PRESSED, 0, 0, pEv->keyname, 0, \
					      data, type, event);
			}
		}
	}

//	return ecore_event_evas_key_downp(data, type, event);
	return ECORE_CALLBACK_PASS_ON;
}

Eina_Bool PROBE_NAME(ecore_event_evas_key_up)(void *data, int type, void *event)
{
	/* TODO Support old preload */
//	static Eina_Bool (*ecore_event_evas_key_upp)(void *data, int type, void *event);
//	Eina_Bool (*ecore_event_evas_key_upp)(void *data, int type, void *event);

//	GET_REAL_FUNC(ecore_event_evas_key_up, LIBECORE_INPUT_EVAS);

//	ecore_event_evas_key_upp = (void *)GET_ORIG_FUNC(event_feature,
//						       ecore_event_evas_key_up);

	if(isOptionEnabled(OPT_EVENT))
	{
		if(event != NULL)
		{
			Ecore_Event_Key* pEv = (Ecore_Event_Key*)event;
			if(strcasestr(pEv->keyname, "volume") == NULL)
			{
				PACK_HW_EVENT(API_ID_ecore_event_evas_key_up,
					      _EVENT_KEY, _KEY_RELEASED, 0, 0, pEv->keyname, 0, \
					      data, type, event);
			}
		}
	}

//	return ecore_event_evas_key_upp(data, type, event);
	return ECORE_CALLBACK_PASS_ON;
}

Eina_Bool PROBE_NAME(ecore_event_evas_mouse_button_down)(void *data, int type, void *event)
{
	/* TODO Support old preload */
//	static Eina_Bool (*ecore_event_evas_mouse_button_downp)(void *data, int type, void *event);
//	Eina_Bool (*ecore_event_evas_mouse_button_downp)(void *data, int type, void *event);

//	GET_REAL_FUNC(ecore_event_evas_mouse_button_down, LIBECORE_INPUT_EVAS);
//	ecore_event_evas_mouse_button_downp = (void *)GET_ORIG_FUNC(
//					    event_feature,
//					    ecore_event_evas_mouse_button_down);

	if(isOptionEnabled(OPT_EVENT))
	{
		if(event != NULL)
		{
			Ecore_Event_Mouse_Button* pEv = (Ecore_Event_Mouse_Button*)event;
			touch_pressed = true;
			PACK_HW_EVENT(API_ID_ecore_event_evas_mouse_button_down,
				      _EVENT_TOUCH, _TOUCH_PRESSED, pEv->root.x, pEv->root.y, "", pEv->multi.device, \
				      data, type, event);
		}
	}

//	return ecore_event_evas_mouse_button_downp(data, type, event);
	return ECORE_CALLBACK_PASS_ON;
}

Eina_Bool PROBE_NAME(ecore_event_evas_mouse_button_up)(void *data, int type, void *event)
{
	/* TODO Support old preload */
//	static Eina_Bool (*ecore_event_evas_mouse_button_upp)(void *data, int type, void *event);
//	Eina_Bool (*ecore_event_evas_mouse_button_upp)(void *data, int type, void *event);

//	GET_REAL_FUNC(ecore_event_evas_mouse_button_up, LIBECORE_INPUT_EVAS);

//	ecore_event_evas_mouse_button_upp = (void *)GET_ORIG_FUNC(event_feature,
//					      ecore_event_evas_mouse_button_up);

	if(isOptionEnabled(OPT_EVENT))
	{
		if(event != NULL)
		{
			Ecore_Event_Mouse_Button* pEv = (Ecore_Event_Mouse_Button*)event;
			touch_pressed = false;
			PACK_HW_EVENT(API_ID_ecore_event_evas_mouse_button_up,
				      _EVENT_TOUCH, _TOUCH_RELEASED, pEv->root.x, pEv->root.y, "", pEv->multi.device, \
				      data, type, event);
		}
	}

//	return ecore_event_evas_mouse_button_upp(data, type, event);
	return ECORE_CALLBACK_PASS_ON;
}

Eina_Bool PROBE_NAME(ecore_event_evas_mouse_move)(void *data, int type, void *event)
{
	/* TODO Support old preload */
//	static Eina_Bool (*ecore_event_evas_mouse_movep)(void *data, int type, void *event);
//	Eina_Bool (*ecore_event_evas_mouse_movep)(void *data, int type, void *event);

//	GET_REAL_FUNC(ecore_event_evas_mouse_move, LIBECORE_INPUT_EVAS);

//	ecore_event_evas_mouse_movep = (void *)GET_ORIG_FUNC(event_feature,
//						   ecore_event_evas_mouse_move);

	if(isOptionEnabled(OPT_EVENT))
	{
		if(touch_pressed)
		{
			if(event != NULL)
			{
				Ecore_Event_Mouse_Move* pEv = (Ecore_Event_Mouse_Move*)event;
				PACK_HW_EVENT(API_ID_ecore_event_evas_mouse_move,
					      _EVENT_TOUCH, _TOUCH_MOVED, pEv->root.x, pEv->root.y, "", pEv->multi.device, \
					      data, type, event);
			}
		}
	}

//	return ecore_event_evas_mouse_movep(data, type, event);
	return ECORE_CALLBACK_PASS_ON;
}

void mouse_key_init(void)
{
	ecore_event_evas_handlers[0] = ecore_event_handler_add(
					ECORE_EVENT_KEY_DOWN,
					PROBE_NAME(ecore_event_evas_key_down),
					NULL);
	ecore_event_evas_handlers[1] = ecore_event_handler_add(
					ECORE_EVENT_KEY_UP,
					PROBE_NAME(ecore_event_evas_key_up),
					NULL);
	ecore_event_evas_handlers[2] = ecore_event_handler_add(
					ECORE_EVENT_MOUSE_BUTTON_DOWN,
					PROBE_NAME(ecore_event_evas_mouse_button_down),
					NULL);
	ecore_event_evas_handlers[3] = ecore_event_handler_add(
					ECORE_EVENT_MOUSE_BUTTON_UP,
					PROBE_NAME(ecore_event_evas_mouse_button_up),
					NULL);
	ecore_event_evas_handlers[4] = ecore_event_handler_add(
					ECORE_EVENT_MOUSE_MOVE,
					PROBE_NAME(ecore_event_evas_mouse_move),
					NULL);
}
