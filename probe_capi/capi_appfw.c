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

#include <app.h>
#include <Ecore.h>
#include "daprobe.h"
#include "dahelper.h"
#include "probeinfo.h"
#include "binproto.h"

Ecore_Event_Handler* register_orientation_event_listener();
void unregister_orientation_event_listener(Ecore_Event_Handler* handler);

app_event_callback_s gAppCallback;

#define PACK_ORIGINAL_APPFWCYCLE(API_ID, RVAL, INPUTFORMAT, ...)		\
	newerrno = errno;																		\
	do {																					\
		if(postBlockBegin(blockresult)) {													\
			PREPARE_LOCAL_BUF();															\
			PACK_COMMON_BEGIN(MSG_PROBE_LIFECYCLE, API_ID, INPUTFORMAT, __VA_ARGS__);	\
			PACK_COMMON_END(RVAL, newerrno, blockresult);									\
			FLUSH_LOCAL_BUF();																\
			postBlockEnd();																	\
		}																					\
	} while(0);																				\
	errno = (newerrno != 0) ? newerrno : olderrno

static enum DaOptions _sopt = OPT_ALWAYSON;

static bool _dalc_app_create(void* user_data)
{
	bool bret;
	DECLARE_VARIABLE_STANDARD;

	bfiltering = false;
	PRE_PROBEBLOCK();

	bret = gAppCallback.create(user_data);

	PACK_ORIGINAL_APPFWCYCLE(API_ID__dalc_app_create, bret, "p", user_data);

	return bret;
}

static void _dalc_app_terminate(void* user_data)
{
	DECLARE_VARIABLE_STANDARD;

	bfiltering = false;
	PRE_PROBEBLOCK();

	gAppCallback.terminate(user_data);

	PACK_ORIGINAL_APPFWCYCLE(API_ID__dalc_app_terminate, 0, "p", user_data);
}

static void _dalc_app_pause(void* user_data)
{
	DECLARE_VARIABLE_STANDARD;

	bfiltering = false;
	PRE_PROBEBLOCK();

	gAppCallback.pause(user_data);

	PACK_ORIGINAL_APPFWCYCLE(API_ID__dalc_app_pause, 0, "p", user_data);
}

static void _dalc_app_resume(void* user_data)
{
	DECLARE_VARIABLE_STANDARD;

	bfiltering = false;
	PRE_PROBEBLOCK();

	gAppCallback.resume(user_data);

	PACK_ORIGINAL_APPFWCYCLE(API_ID__dalc_app_resume, 0, "p", user_data);
}

static void _dalc_app_service(service_h service, void* user_data)
{
	DECLARE_VARIABLE_STANDARD;

	bfiltering = false;
	PRE_PROBEBLOCK();

	gAppCallback.service(service, user_data);

	PACK_ORIGINAL_APPFWCYCLE(API_ID__dalc_app_service, 0, "dp", (unsigned int)service, user_data);
}

static void _dalc_app_deviceorientationchanged(app_device_orientation_e orientation, void* user_data)
{
	on_orientation_changed((int)orientation, true);

	if(gAppCallback.device_orientation)
		gAppCallback.device_orientation(orientation, user_data);
}

int app_efl_main(int *argc, char ***argv, app_event_callback_s *callback, void *user_data)
{
	static int (*app_efl_mainp)(int* argc, char*** argv, app_event_callback_s* callback, void* user_data);
	Ecore_Event_Handler* handler;
	int ret;

	GET_REAL_FUNC(app_efl_main, LIBCAPI_APPFW_APPLICATION);

	probeBlockStart();
	handler = register_orientation_event_listener();
	gAppCallback.create = callback->create;
	gAppCallback.terminate = callback->terminate;
	gAppCallback.pause = callback->pause;
	gAppCallback.resume = callback->resume;
	gAppCallback.service = callback->service;
	gAppCallback.device_orientation = callback->device_orientation;

	if(callback->create)
		callback->create = _dalc_app_create;
	if(callback->terminate)
		callback->terminate = _dalc_app_terminate;
	if(callback->pause)
		callback->pause = _dalc_app_pause;
	if(callback->resume)
		callback->resume = _dalc_app_resume;
	if(callback->service)
		callback->service = _dalc_app_service;
	callback->device_orientation = _dalc_app_deviceorientationchanged;
	probeBlockEnd();

	ret = app_efl_mainp(argc, argv, callback, user_data);

	probeBlockStart();
	unregister_orientation_event_listener(handler);
	callback->create = gAppCallback.create;
	callback->terminate = gAppCallback.terminate;
	callback->pause = gAppCallback.pause;
	callback->resume = gAppCallback.resume;
	callback->service = gAppCallback.service;
	callback->device_orientation = gAppCallback.device_orientation;
	probeBlockEnd();

	return ret;
}

