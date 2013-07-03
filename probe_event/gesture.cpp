/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
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
 * 
 */

#include "daprobe.h"
#include "dahelper.h"
#include "probeinfo.h"
#include "osp_probe.h"
#include "dacollection.h"
#include "da_event.h"
#include "gesture.h"
#include "binproto.h"

#define GESTURE_EVENT_LOG(_FUNCNAME, _GESTURETYPE, _X, _Y, _INFO1, _INFO2)		\
	setProbePoint(&probeInfo);													\
	INIT_LOG;																	\
	APPEND_LOG_BASIC_NAME(LC_UIEVENT, _FUNCNAME);								\
	APPEND_LOG_COMMON_NONE(0);													\
	log.length += sprintf(log.data + log.length, "`,%d`,%d`,%d`,%d`,%d`,%d",	\
			_EVENT_GESTURE, _GESTURETYPE, _X, _Y, _INFO1, _INFO2);				\
	printLog(&log, MSG_LOG)

#define PACK_GESTURE_EVENT(_GESTURETYPE, _X, _Y, _INFO1, _INFO2, _ARGDETECTOR)	\
	do {																		\
		char info1_str[16];													\
		PREPARE_LOCAL_BUF();													\
		PACK_COMMON_BEGIN(MSG_PROBE_UIEVENT, LC_UIEVENT, "p", _ARGDETECTOR);	\
		PACK_COMMON_END(0, 0, 0);												\
		sprintf(info1_str, "%d", _INFO1);										\
		PACK_UIEVENT(_EVENT_GESTURE, _GESTURETYPE, _X, _Y, info1_str, _INFO2);	\
		FLUSH_LOCAL_BUF();														\
	} while (0)

GestureEventListener GestureEventListener::rInstance;

GestureEventListener::GestureEventListener()
{

}

GestureEventListener::~GestureEventListener()
{

}

void GestureEventListener::OnCustomGestureCanceled (TouchGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		GESTURE_EVENT_LOG("canceled", _GESTURE_CUSTOM, 0, 0, 0, 0);
		PACK_GESTURE_EVENT(_GESTURE_CUSTOM, 0, 0, 0, 0, &gestureDetector);
		probeBlockEnd();
	}
}

void GestureEventListener::OnCustomGestureChanged (TouchGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		GESTURE_EVENT_LOG("changed", _GESTURE_CUSTOM, 0, 0, 0, 0);
		PACK_GESTURE_EVENT(_GESTURE_CUSTOM, 0, 0, 0, 0, &gestureDetector);
		probeBlockEnd();
	}
}

void GestureEventListener::OnCustomGestureFinished (TouchGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		GESTURE_EVENT_LOG("finished", _GESTURE_CUSTOM, 0, 0, 0, 0);
		PACK_GESTURE_EVENT(_GESTURE_CUSTOM, 0, 0, 0, 0, &gestureDetector);
		probeBlockEnd();
	}
}

void GestureEventListener::OnCustomGestureStarted (TouchGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		GESTURE_EVENT_LOG("started", _GESTURE_CUSTOM, 0, 0, 0, 0);
		PACK_GESTURE_EVENT(_GESTURE_CUSTOM, 0, 0, 0, 0, &gestureDetector);
		probeBlockEnd();
	}
}

void GestureEventListener::OnFlickGestureCanceled (TouchFlickGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int x = 0, y = 0, dur = 0;
			FlickDirection direction = FLICK_DIRECTION_NONE;

			gestureDetector.GetDistance(x, y);
			dur = gestureDetector.GetDuration();
			direction = gestureDetector.GetDirection();
			GESTURE_EVENT_LOG("canceled", _GESTURE_FLICK, x, y, dur, (int)direction);
			PACK_GESTURE_EVENT(_GESTURE_FLICK, x, y, dur, (int)direction, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnFlickGestureDetected (TouchFlickGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int x = 0, y = 0, dur = 0;
			FlickDirection direction = FLICK_DIRECTION_NONE;

			gestureDetector.GetDistance(x, y);
			dur = gestureDetector.GetDuration();
			direction = gestureDetector.GetDirection();
			GESTURE_EVENT_LOG("detected", _GESTURE_FLICK, x, y, dur, (int)direction);
			PACK_GESTURE_EVENT(_GESTURE_FLICK, x, y, dur, (int)direction, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnLongPressGestureCanceled (TouchLongPressGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int moveallow = 0, tcount = 0, dur = 0;

			moveallow = gestureDetector.GetMoveAllowance();
			dur = gestureDetector.GetDuration();
			tcount = gestureDetector.GetTouchCount();
			GESTURE_EVENT_LOG("canceled", _GESTURE_LONGPRESS, moveallow, 0, dur, tcount);
			PACK_GESTURE_EVENT(_GESTURE_LONGPRESS, moveallow, 0, dur, tcount, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnLongPressGestureDetected (TouchLongPressGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int moveallow = 0, tcount = 0, dur = 0;

			moveallow = gestureDetector.GetMoveAllowance();
			dur = gestureDetector.GetDuration();
			tcount = gestureDetector.GetTouchCount();
			GESTURE_EVENT_LOG("detected", _GESTURE_LONGPRESS, moveallow, 0, dur, tcount);
			PACK_GESTURE_EVENT(_GESTURE_LONGPRESS, moveallow, 0, dur, tcount, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnPanningGestureCanceled (TouchPanningGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int tcount = 0;

			tcount = gestureDetector.GetTouchCount();
			GESTURE_EVENT_LOG("canceled", _GESTURE_PANNING, 0, 0, 0, tcount);
			PACK_GESTURE_EVENT(_GESTURE_PANNING, 0, 0, 0, tcount, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnPanningGestureChanged (TouchPanningGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int tcount = 0;

			tcount = gestureDetector.GetTouchCount();
			GESTURE_EVENT_LOG("changed", _GESTURE_PANNING, 0, 0, 0, tcount);
			PACK_GESTURE_EVENT(_GESTURE_PANNING, 0, 0, 0, tcount, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnPanningGestureFinished (TouchPanningGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int tcount = 0;

			tcount = gestureDetector.GetTouchCount();
			GESTURE_EVENT_LOG("finished", _GESTURE_PANNING, 0, 0, 0, tcount);
			PACK_GESTURE_EVENT(_GESTURE_PANNING, 0, 0, 0, tcount, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnPanningGestureStarted (TouchPanningGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int tcount = 0;

			tcount = gestureDetector.GetTouchCount();
			GESTURE_EVENT_LOG("started", _GESTURE_PANNING, 0, 0, 0, tcount);
			PACK_GESTURE_EVENT(_GESTURE_PANNING, 0, 0, 0, tcount, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnPinchGestureCanceled (TouchPinchGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			Tizen::Graphics::Point point;
			int scale = 0;

			point = gestureDetector.GetCenterPoint();
			scale = gestureDetector.GetScale();
			GESTURE_EVENT_LOG("canceled", _GESTURE_PINCH, point.x, point.y, scale, 0);
			PACK_GESTURE_EVENT(_GESTURE_PINCH, point.x, point.y, scale, 0, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnPinchGestureChanged (TouchPinchGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			Tizen::Graphics::Point point;
			int scale = 0;

			point = gestureDetector.GetCenterPoint();
			scale = gestureDetector.GetScale();
			GESTURE_EVENT_LOG("changed", _GESTURE_PINCH, point.x, point.y, scale, 0);
			PACK_GESTURE_EVENT(_GESTURE_PINCH, point.x, point.y, scale, 0, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnPinchGestureFinished (TouchPinchGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			Tizen::Graphics::Point point;
			int scale = 0;

			point = gestureDetector.GetCenterPoint();
			scale = gestureDetector.GetScale();
			GESTURE_EVENT_LOG("finished", _GESTURE_PINCH, point.x, point.y, scale, 0);
			PACK_GESTURE_EVENT(_GESTURE_PINCH, point.x, point.y, scale, 0, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnPinchGestureStarted (TouchPinchGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			Tizen::Graphics::Point point;
			int scale = 0;

			point = gestureDetector.GetCenterPoint();
			scale = gestureDetector.GetScale();
			GESTURE_EVENT_LOG("started", _GESTURE_PINCH, point.x, point.y, scale, 0);
			PACK_GESTURE_EVENT(_GESTURE_PINCH, point.x, point.y, scale, 0, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnRotationGestureCanceled (TouchRotationGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int distance = 0;
			float angle = 0.0f;

			distance = gestureDetector.GetDistance();
			angle = gestureDetector.GetAngle();
			GESTURE_EVENT_LOG("canceled", _GESTURE_ROTATION, 0, 0, distance, static_cast<int>(angle));
			PACK_GESTURE_EVENT(_GESTURE_ROTATION, 0, 0, distance, static_cast<int>(angle), &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnRotationGestureChanged (TouchRotationGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int distance = 0;
			float angle = 0.0f;

			distance = gestureDetector.GetDistance();
			angle = gestureDetector.GetAngle();
			GESTURE_EVENT_LOG("changed", _GESTURE_ROTATION, 0, 0, distance, static_cast<int>(angle));
			PACK_GESTURE_EVENT(_GESTURE_ROTATION, 0, 0, distance, static_cast<int>(angle), &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnRotationGestureFinished (TouchRotationGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int distance = 0;
			float angle = 0.0f;

			distance = gestureDetector.GetDistance();
			angle = gestureDetector.GetAngle();
			GESTURE_EVENT_LOG("finished", _GESTURE_ROTATION, 0, 0, distance, static_cast<int>(angle));
			PACK_GESTURE_EVENT(_GESTURE_ROTATION, 0, 0, distance, static_cast<int>(angle), &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnRotationGestureStarted (TouchRotationGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int distance = 0;
			float angle = 0.0f;

			distance = gestureDetector.GetDistance();
			angle = gestureDetector.GetAngle();
			GESTURE_EVENT_LOG("started", _GESTURE_ROTATION, 0, 0, distance, static_cast<int>(angle));
			PACK_GESTURE_EVENT(_GESTURE_ROTATION, 0, 0, distance, static_cast<int>(angle), &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnTapGestureCanceled (TouchTapGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int move = 0, tapcount = 0, touchcount = 0, interval = 0;

			move = gestureDetector.GetMoveAllowance();
			tapcount = gestureDetector.GetTapCount();
			interval = gestureDetector.GetTapInterval();
			touchcount = gestureDetector.GetTouchCount();
			GESTURE_EVENT_LOG("canceled", _GESTURE_TAP, move, tapcount, interval, touchcount);
			PACK_GESTURE_EVENT(_GESTURE_TAP, move, tapcount, interval, touchcount, &gestureDetector);
		}
		probeBlockEnd();
	}
}

void GestureEventListener::OnTapGestureDetected (TouchTapGestureDetector &gestureDetector)
{
	probeInfo_t	probeInfo; log_t log;

	if(isOptionEnabled(OPT_EVENT))
	{
		probeBlockStart();
		{
			int move = 0, tapcount = 0, touchcount = 0, interval = 0;

			move = gestureDetector.GetMoveAllowance();
			tapcount = gestureDetector.GetTapCount();
			interval = gestureDetector.GetTapInterval();
			touchcount = gestureDetector.GetTouchCount();
			GESTURE_EVENT_LOG("detected", _GESTURE_TAP, move, tapcount, interval, touchcount);
			PACK_GESTURE_EVENT(_GESTURE_TAP, move, tapcount, interval, touchcount, &gestureDetector);
		}
		probeBlockEnd();
	}
}

GestureEventListener& GestureEventListener::GetInstance()
{
	return rInstance;
}


namespace Tizen { namespace Ui {

result TouchFlickGestureDetector::AddFlickGestureEventListener(ITouchFlickGestureEventListener& listener)
{
	typedef result (TouchFlickGestureDetector::*methodType)(ITouchFlickGestureEventListener& listener);
	static methodType addflickgestureeventlistenerp;
	result iret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui25TouchFlickGestureDetector28AddFlickGestureEventListenerERNS0_31ITouchFlickGestureEventListenerE,
			LIBOSP_UIFW, addflickgestureeventlistenerp);

	probeBlockStart();
	GestureEventListener& mylistener = GestureEventListener::GetInstance();
	if(add_detector_hash(static_cast<void*>(this), static_cast<void*>(&mylistener)) <= 0)
	{
		iret = (this->*addflickgestureeventlistenerp)(static_cast<ITouchFlickGestureEventListener&>(mylistener));
		if(likely(iret == E_SUCCESS))
		{
			// do nothing
		}
		else
		{
			del_detector_hash(static_cast<void*>(this));
		}
	}
	probeBlockEnd();

	return (this->*addflickgestureeventlistenerp)(listener);
}

result TouchLongPressGestureDetector::AddLongPressGestureEventListener(ITouchLongPressGestureEventListener& listener)
{
	typedef result (TouchLongPressGestureDetector::*methodType)(ITouchLongPressGestureEventListener& listener);
	static methodType addlongpressgestureeventlistenerp;
	result iret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui29TouchLongPressGestureDetector32AddLongPressGestureEventListenerERNS0_35ITouchLongPressGestureEventListenerE,
			LIBOSP_UIFW, addlongpressgestureeventlistenerp);

	probeBlockStart();
	GestureEventListener& mylistener = GestureEventListener::GetInstance();
	if(add_detector_hash(static_cast<void*>(this), static_cast<void*>(&mylistener)) <= 0)
	{
		iret = (this->*addlongpressgestureeventlistenerp)(static_cast<ITouchLongPressGestureEventListener&>(mylistener));
		if(likely(iret == E_SUCCESS))
		{
			// do nothing
		}
		else
		{
			del_detector_hash(static_cast<void*>(this));
		}
	}
	probeBlockEnd();

	return (this->*addlongpressgestureeventlistenerp)(listener);
}

result TouchPanningGestureDetector::AddPanningGestureEventListener(ITouchPanningGestureEventListener& listener)
{
	typedef result (TouchPanningGestureDetector::*methodType)(ITouchPanningGestureEventListener& listener);
	static methodType addpanninggestureeventlistenerp;
	result iret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui27TouchPanningGestureDetector30AddPanningGestureEventListenerERNS0_33ITouchPanningGestureEventListenerE,
			LIBOSP_UIFW, addpanninggestureeventlistenerp);

	probeBlockStart();
	GestureEventListener& mylistener = GestureEventListener::GetInstance();
	if(add_detector_hash(static_cast<void*>(this), static_cast<void*>(&mylistener)) <= 0)
	{
		iret = (this->*addpanninggestureeventlistenerp)(static_cast<ITouchPanningGestureEventListener&>(mylistener));
		if(likely(iret == E_SUCCESS))
		{
			// do nothing
		}
		else
		{
			del_detector_hash(static_cast<void*>(this));
		}
	}
	probeBlockEnd();

	return (this->*addpanninggestureeventlistenerp)(listener);
}

result TouchPinchGestureDetector::AddPinchGestureEventListener(ITouchPinchGestureEventListener& listener)
{
	typedef result (TouchPinchGestureDetector::*methodType)(ITouchPinchGestureEventListener& listener);
	static methodType addpinchgestureeventlistenerp;
	result iret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui25TouchPinchGestureDetector28AddPinchGestureEventListenerERNS0_31ITouchPinchGestureEventListenerE,
			LIBOSP_UIFW, addpinchgestureeventlistenerp);

	probeBlockStart();
	GestureEventListener& mylistener = GestureEventListener::GetInstance();
	if(add_detector_hash(static_cast<void*>(this), static_cast<void*>(&mylistener)) <= 0)
	{
		iret = (this->*addpinchgestureeventlistenerp)(static_cast<ITouchPinchGestureEventListener&>(mylistener));
		if(likely(iret == E_SUCCESS))
		{
			// do nothing
		}
		else
		{
			del_detector_hash(static_cast<void*>(this));
		}
	}
	probeBlockEnd();

	return (this->*addpinchgestureeventlistenerp)(listener);
}

result TouchRotationGestureDetector::AddRotationGestureEventListener(ITouchRotationGestureEventListener& listener)
{
	typedef result (TouchRotationGestureDetector::*methodType)(ITouchRotationGestureEventListener& listener);
	static methodType addrotationgestureeventlistenerp;
	result iret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui28TouchRotationGestureDetector31AddRotationGestureEventListenerERNS0_34ITouchRotationGestureEventListenerE,
			LIBOSP_UIFW, addrotationgestureeventlistenerp);

	probeBlockStart();
	GestureEventListener& mylistener = GestureEventListener::GetInstance();
	if(add_detector_hash(static_cast<void*>(this), static_cast<void*>(&mylistener)) <= 0)
	{
		iret = (this->*addrotationgestureeventlistenerp)(static_cast<ITouchRotationGestureEventListener&>(mylistener));
		if(likely(iret == E_SUCCESS))
		{
			// do nothing
		}
		else
		{
			del_detector_hash(static_cast<void*>(this));
		}
	}
	probeBlockEnd();

	return (this->*addrotationgestureeventlistenerp)(listener);
}

result TouchTapGestureDetector::AddTapGestureEventListener(ITouchTapGestureEventListener& listener)
{
	typedef result (TouchTapGestureDetector::*methodType)(ITouchTapGestureEventListener& listener);
	static methodType addtapgestureeventlistenerp;
	result iret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui23TouchTapGestureDetector26AddTapGestureEventListenerERNS0_29ITouchTapGestureEventListenerE,
			LIBOSP_UIFW, addtapgestureeventlistenerp);

	probeBlockStart();
	GestureEventListener& mylistener = GestureEventListener::GetInstance();
	if(add_detector_hash(static_cast<void*>(this), static_cast<void*>(&mylistener)) <= 0)
	{
		iret = (this->*addtapgestureeventlistenerp)(static_cast<ITouchTapGestureEventListener&>(mylistener));
		if(likely(iret == E_SUCCESS))
		{
			// do nothing
		}
		else
		{
			del_detector_hash(static_cast<void*>(this));
		}
	}
	probeBlockEnd();

	return (this->*addtapgestureeventlistenerp)(listener);
}

result TouchGestureDetector::AddGestureEventListener(ITouchGestureEventListener& listener)
{
	typedef result (TouchGestureDetector::*methodType)(ITouchGestureEventListener& listener);
	static methodType addgestureeventlistenerp;
	result iret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui20TouchGestureDetector23AddGestureEventListenerERNS0_26ITouchGestureEventListenerE,
			LIBOSP_UIFW, addgestureeventlistenerp);

	probeBlockStart();
	GestureEventListener& mylistener = GestureEventListener::GetInstance();
	if(add_detector_hash(static_cast<void*>(this), static_cast<void*>(&mylistener)) <= 0)
	{
		iret = (this->*addgestureeventlistenerp)(static_cast<ITouchGestureEventListener&>(mylistener));
		if(likely(iret == E_SUCCESS))
		{
			// do nothing
		}
		else
		{
			del_detector_hash(static_cast<void*>(this));
		}
	}
	probeBlockEnd();

	return (this->*addgestureeventlistenerp)(listener);
}


} }	// end of namespace



