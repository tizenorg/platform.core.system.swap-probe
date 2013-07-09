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
//#include "dautil.h"
#include "da_event.h"

#include "binproto.h"

bool touch_pressed = false;

#define PACK_HW_EVENT(_EVENTTYPE, _DETAILTYPE, _X, _Y, _KEYCODE, _EXTRA, 		\
	_ARGDATA, _ARGTYPE, _ARGEVENT)												\
	do {																		\
		setProbePoint(&probeInfo);												\
		PREPARE_LOCAL_BUF();													\
		PACK_COMMON_BEGIN(MSG_PROBE_UIEVENT, LC_UIEVENT, "pdp",					\
				  _ARGDATA, _ARGTYPE, _ARGEVENT);								\
		PACK_COMMON_END(0, 0, 0);												\
		PACK_UIEVENT(_EVENTTYPE, _DETAILTYPE, _X, _Y, _KEYCODE, _EXTRA);		\
		FLUSH_LOCAL_BUF();														\
	} while (0)

Eina_Bool ecore_event_evas_key_down(void *data, int type, void *event)
{
	static Eina_Bool (*ecore_event_evas_key_downp)(void *data, int type, void *event);
	probeInfo_t	probeInfo;

	GET_REAL_FUNC(ecore_event_evas_key_down, LIBECORE_INPUT_EVAS);

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		if(event != NULL)
		{
			Ecore_Event_Key* pEv = (Ecore_Event_Key*)event;
			if(strcasestr(pEv->keyname, "volume") == NULL)
			{
				PACK_HW_EVENT(_EVENT_KEY, _KEY_PRESSED, 0, 0, pEv->keyname, 0, 	\
					data, type, event);
			}
		}
		probeBlockEnd();
	}

	return ecore_event_evas_key_downp(data, type, event);
}

Eina_Bool ecore_event_evas_key_up(void *data, int type, void *event)
{
	static Eina_Bool (*ecore_event_evas_key_upp)(void *data, int type, void *event);
	probeInfo_t	probeInfo;

	GET_REAL_FUNC(ecore_event_evas_key_up, LIBECORE_INPUT_EVAS);

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		if(event != NULL)
		{
			Ecore_Event_Key* pEv = (Ecore_Event_Key*)event;
			if(strcasestr(pEv->keyname, "volume") == NULL)
			{
				PACK_HW_EVENT(_EVENT_KEY, _KEY_RELEASED, 0, 0, pEv->keyname, 0, \
					data, type, event);
			}
		}
		probeBlockEnd();
	}

	return ecore_event_evas_key_upp(data, type, event);
}

Eina_Bool ecore_event_evas_mouse_button_down(void *data, int type, void *event)
{
	static Eina_Bool (*ecore_event_evas_mouse_button_downp)(void *data, int type, void *event);
	probeInfo_t	probeInfo;

	GET_REAL_FUNC(ecore_event_evas_mouse_button_down, LIBECORE_INPUT_EVAS);

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		if(event != NULL)
		{
			Ecore_Event_Mouse_Button* pEv = (Ecore_Event_Mouse_Button*)event;
			touch_pressed = true;
			PACK_HW_EVENT(_EVENT_TOUCH, _TOUCH_PRESSED, pEv->root.x, pEv->root.y, "", pEv->multi.device, \
				data, type, event);
		}
		probeBlockEnd();
	}

	return ecore_event_evas_mouse_button_downp(data, type, event);
}

Eina_Bool ecore_event_evas_mouse_button_up(void *data, int type, void *event)
{
	static Eina_Bool (*ecore_event_evas_mouse_button_upp)(void *data, int type, void *event);
	probeInfo_t	probeInfo;

	GET_REAL_FUNC(ecore_event_evas_mouse_button_up, LIBECORE_INPUT_EVAS);

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		if(event != NULL)
		{
			Ecore_Event_Mouse_Button* pEv = (Ecore_Event_Mouse_Button*)event;
			touch_pressed = false;
			PACK_HW_EVENT(_EVENT_TOUCH, _TOUCH_RELEASED, pEv->root.x, pEv->root.y, "", pEv->multi.device, \
				data, type, event);
		}
		probeBlockEnd();
	}

	return ecore_event_evas_mouse_button_upp(data, type, event);
}

Eina_Bool ecore_event_evas_mouse_move(void *data, int type, void *event)
{
	static Eina_Bool (*ecore_event_evas_mouse_movep)(void *data, int type, void *event);
	probeInfo_t	probeInfo;

	GET_REAL_FUNC(ecore_event_evas_mouse_move, LIBECORE_INPUT_EVAS);

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		if(touch_pressed)
		{
			if(event != NULL)
			{
				Ecore_Event_Mouse_Move* pEv = (Ecore_Event_Mouse_Move*)event;
				PACK_HW_EVENT(_EVENT_TOUCH, _TOUCH_MOVED, pEv->root.x, pEv->root.y, "", pEv->multi.device, \
					data, type, event);
			}
		}
		probeBlockEnd();
	}

	return ecore_event_evas_mouse_movep(data, type, event);
}


/*
void evas_event_feed_mouse_down(Evas *e, int b, Evas_Button_Flags flags,
		unsigned int timestamp, const void *data)
{
	static void (*evas_event_feed_mouse_downp)(Evas *e, int b, Evas_Button_Flags flags, unsigned int timestamp, const void *data);

	probeInfo_t	probeInfo;

	GET_REAL_FUNC(evas_event_feed_mouse_down, LIBEVAS);

	PRE_UNCONDITIONAL_BLOCK_BEGIN();
	HW_EVENT_LOG(_EVENT_TOUCH, _TOUCH_PRESSED, )
	PRE_UNCONDITIONAL_BLOCK_END();

	
	DECLARE_VARIABLE_EVENT;
	int x, y;
	
	PRE_PROBEBLOCK_BEGIN_EVENT(evas_event_feed_mouse_down, LIBEVAS, EVENT_TYPE_DOWN);
		evas_pointer_output_xy_get(e, &x, &y);
	PRE_PROBEBLOCK_END_EVENT();	
	
	evas_event_feed_mouse_downp(e, b, flags, timestamp, data);
	
	AFTER_ORIGINAL_EVENT(EVENT_TYPE_DOWN, "%d", b);
}			

void evas_event_feed_mouse_up(Evas *e, int b, Evas_Button_Flags flags,
		unsigned int timestamp, const void *data)
{
	static void (*evas_event_feed_mouse_upp)(Evas *e, int b, Evas_Button_Flags flags, unsigned int timestamp, const void *data);
	
	DECLARE_VARIABLE_EVENT;
	int x, y;
	
	PRE_PROBEBLOCK_BEGIN_EVENT(evas_event_feed_mouse_up, LIBEVAS, EVENT_TYPE_UP);
		evas_pointer_output_xy_get(e, &x, &y);
	PRE_PROBEBLOCK_END_EVENT();	
	
	evas_event_feed_mouse_upp(e, b, flags, timestamp, data);
	
	AFTER_ORIGINAL_EVENT(EVENT_TYPE_UP, "%d", b);
}

void evas_event_feed_mouse_move(Evas *e, int x, int y,
		unsigned int timestamp, const void *data)
{
	static void (*evas_event_feed_mouse_movep)(Evas *e, int x, int y, unsigned int timestamp, const void *data);
	
	BEFORE_ORIGINAL_EVENT(evas_event_feed_mouse_move, LIBEVAS, EVENT_TYPE_MOVE);
	
	evas_event_feed_mouse_movep(e, x, y, timestamp, data);
	
	AFTER_ORIGINAL_EVENT(EVENT_TYPE_MOVE, "%u", timestamp);
}

void evas_event_feed_multi_down(Evas *e, int d, int x, int y,
		double rad, double radx, double rady,
		double pres, double ang, double fx, double fy,
		Evas_Button_Flags flags, unsigned int timestamp, const void *data)
{
	static void (*evas_event_feed_multi_downp)(Evas *e, int d, int x, int y, double rad, double radx, double rady, double pres, double ang, double fx, double fy, Evas_Button_Flags flags, unsigned int timestamp, const void *data);

	BEFORE_ORIGINAL_EVENT(evas_event_feed_multi_down, LIBEVAS, EVENT_TYPE_DOWN);
	
	evas_event_feed_multi_downp(e, d, x, y, rad, radx,
			rady, pres, ang, fx, fy, flags, timestamp, data);
		
	AFTER_ORIGINAL_EVENT(EVENT_TYPE_DOWN, "%d", d);
}

void evas_event_feed_multi_up(Evas *e, int d, int x, int y, double rad, double radx,
		double rady, double pres, double ang, double fx, double fy,
		Evas_Button_Flags flags, unsigned int timestamp, const void *data)
{
	static void (*evas_event_feed_multi_upp)(Evas *e, int d, int x, int y, double rad, double radx, double rady, double pres, double ang, double fx, double fy, Evas_Button_Flags flags, unsigned int timestamp, const void *data);

	BEFORE_ORIGINAL_EVENT(evas_event_feed_multi_up, LIBEVAS, EVENT_TYPE_UP);
	
	evas_event_feed_multi_upp(e, d, x, y, rad, radx, rady,
			pres, ang, fx, fy, flags, timestamp, data);
			
	AFTER_ORIGINAL_EVENT(EVENT_TYPE_UP, "%d", d);
}

void evas_event_feed_multi_move (Evas *e, int d, int x, int y, double rad, double radx,
		double rady, double pres, double ang, double fx, double fy,
		unsigned int timestamp, const void *data)
{
	static void (*evas_event_feed_multi_movep)(Evas *e, int d, int x, int y, double rad, double radx, double rady, double pres, double ang, double fx, double fy, unsigned int timestamp, const void *data);

	BEFORE_ORIGINAL_EVENT(evas_event_feed_multi_move, LIBEVAS, EVENT_TYPE_MOVE);
	
	evas_event_feed_multi_movep(e, d, x, y, rad, radx, rady,
			pres, ang, fx, fy, timestamp, data);
			
	AFTER_ORIGINAL_EVENT(EVENT_TYPE_MOVE, "%d", d);
}
*/
