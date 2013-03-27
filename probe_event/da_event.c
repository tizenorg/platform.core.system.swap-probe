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

#include <appfw/app.h>
#include <runtime_info.h>

#include <Ecore.h>
#include <Eina.h>
#include <Ecore_X.h>

#include "daprobe.h"
#include "dahelper.h"
#include "da_event.h"

static int external_angle = 0;
static int internal_angle = 0;
static bool orientation_enabled = false;

// ===================================================================
// orientation related functions
// ===================================================================

static int convert_angle(int angle)
{
	int os = _OS_NONE;

	switch(angle)
	{
	case 0:
		os = _OS_PORTRAIT;
		break;
	case 90:
		os = _OS_LANDSCAPE_REVERSE;
		break;
	case 180:
		os = _OS_PORTRAIT_REVERSE;
		break;
	case 270:
		os = _OS_LANDSCAPE;
		break;
	default:
		break;
	}

	return os;
}

void on_orientation_changed(int angle, bool capi)
{
	DECLARE_COMMON_VARIABLE;
	bool autoRotation = true;

	probeBlockStart();

	internal_angle = angle;
	runtime_info_get_value_bool(
			RUNTIME_INFO_KEY_AUTO_ROTATION_ENABLED, &autoRotation);

	if(autoRotation)	// rotation is not locked
	{
		external_angle = internal_angle;
		setProbePoint(&probeInfo);

		INIT_LOG;
		APPEND_LOG_BASIC_NAME(LC_UIEVENT, "OrientationChanged");
		APPEND_LOG_COMMON_NONE(0);
		log.length += sprintf(log.data + log.length, "`,%d`,`,`,`,%d`,",
				_EVENT_ORIENTATION, convert_angle(external_angle));
		printLog(&log, MSG_LOG);

//		if(orientation_enabled)
//		{
			SCREENSHOT_SET();
//			if(!capi)
//			{
//				SCREENSHOT_DONE();
//			}
//		}
	}
	else { }	// do nothing

	probeBlockEnd();
}

void orientationEnabled()
{
	orientation_enabled = true;
}

int getOrientation()
{
	return external_angle;
}

// ====================================================================
// initialize and finalize event
// ====================================================================

int initialize_event()
{
	external_angle = internal_angle = app_get_device_orientation();
	return 0;
}

int finalize_event()
{
	return 0;
}


