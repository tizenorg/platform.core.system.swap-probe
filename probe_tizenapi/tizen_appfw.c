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

#include <app.h>
#include "daprobe.h"
#include "dahelper.h"
#include "probeinfo.h"

app_event_callback_s gAppCallback;

#define AFTER_ORIGINAL_APPFWCYCLE(RTYPE, RVAL, FUNCNAME, INPUTFORMAT, ...)	\
	newerrno = errno;														\
	do {																	\
		if(postBlockBegin(blockresult)) {									\
			INIT_LOG;														\
			APPEND_LOG_BASIC_NAME(LC_LIFECYCLE, #FUNCNAME);					\
			APPEND_LOG_INPUT(INPUTFORMAT, __VA_ARGS__);						\
			APPEND_LOG_RESULT(RTYPE, RVAL);									\
			__appendTypeLog(&log, 1, NULL, VT_INT, 0);						\
			printLog(&log, MSG_LOG);										\
			postBlockEnd();													\
		}																	\
	} while(0);																\
	errno = (newerrno != 0) ? newerrno : olderrno


static bool _dalc_app_create(void* user_data)
{
	bool bret;
	DECLARE_VARIABLE_STANDARD;

	bfiltering = false;
	PRE_PROBEBLOCK();

	bret = gAppCallback.create(user_data);

	AFTER_ORIGINAL_APPFWCYCLE(VT_INT, bret, usercallback_app_create,
			"%p", user_data);

	return bret;
}

static void _dalc_app_terminate(void* user_data)
{
	DECLARE_VARIABLE_STANDARD;

	bfiltering = false;
	PRE_PROBEBLOCK();

	gAppCallback.terminate(user_data);

	AFTER_ORIGINAL_APPFWCYCLE(VT_NULL, NULL, usercallback_app_terminate,
			"%p", user_data);
}

static void _dalc_app_pause(void* user_data)
{
	DECLARE_VARIABLE_STANDARD;

	bfiltering = false;
	PRE_PROBEBLOCK();

	gAppCallback.pause(user_data);

	AFTER_ORIGINAL_APPFWCYCLE(VT_NULL, NULL, usercallback_app_pause,
			"%p", user_data);
}

static void _dalc_app_resume(void* user_data)
{
	DECLARE_VARIABLE_STANDARD;

	bfiltering = false;
	PRE_PROBEBLOCK();

	gAppCallback.resume(user_data);

	AFTER_ORIGINAL_APPFWCYCLE(VT_NULL, NULL, usercallback_app_resume,
			"%p", user_data);
}

static void _dalc_app_service(service_h service, void* user_data)
{
	DECLARE_VARIABLE_STANDARD;

	bfiltering = false;
	PRE_PROBEBLOCK();

	gAppCallback.service(service, user_data);

	AFTER_ORIGINAL_APPFWCYCLE(VT_NULL, NULL, usercallback_app_service,
			"%u, %p", (unsigned int)service, user_data);
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
	int ret;

	GET_REAL_FUNC(app_efl_main, libcapi-appfw-application.so.0);

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

	ret = app_efl_mainp(argc, argv, callback, user_data);

	callback->create = gAppCallback.create;
	callback->terminate = gAppCallback.terminate;
	callback->pause = gAppCallback.pause;
	callback->resume = gAppCallback.resume;
	callback->service = gAppCallback.service;
	callback->device_orientation = gAppCallback.device_orientation;

	return ret;
}

