/*
 *  DA probe
 *
 * Copyright (File::*c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
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
#include <FApp.h>
#include <FBase.h>

#include "daprobe.h"
#include "dahelper.h"
#include "probeinfo.h"
#include "osp_probe.h"

#include "binproto.h"

using namespace Tizen::Base;
using namespace Tizen::Base::Collection;

#define LIFECYCLE_PROBE_BLOCK(FUNCNAME)					\
	setProbePoint(&probeInfo);							\
	INIT_LOG;											\
	APPEND_LOG_BASIC_NAME(LC_LIFECYCLE, FUNCNAME);		\
	APPEND_LOG_COMMON_NONE(CALLER_ADDRESS);				\
	printLog(&log, MSG_LOG)

namespace Tizen { namespace App
{

//class _IAppImpl
//{
//	virtual void OnDeviceOrientationChanged(app_device_orientation_e orientation) = 0;
//};

class _AppImpl
{
	static void OnTerminate(void* user_data);
	static void OnDeviceOrientationChanged(app_device_orientation_e orientation, void* user_data);
};

class _UiAppImpl 
//	: public Tizen::Base::Object
//	, public _IAppImpl
{
	void OnBackground(void);
	void OnForeground(void);
//	virtual void OnDeviceOrientationChanged(app_device_orientation_e orientation);
};

class _AppInfo
{
	static void SetAppState(AppState appstate);
};

result UiApp::Execute(UiAppInstanceFactory pUiAppFactory,
		const IList* pArguments)
{
	typedef result (*methodType)(UiAppInstanceFactory pFactory, const IList* pArgs);
	static methodType uiapp_executep;
	DECLARE_COMMON_VARIABLE;
	result ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen3App5UiApp7ExecuteEPFPS1_vEPKNS_4Base10Collection5IListE,
		LIBOSP_UIFW, uiapp_executep);

	probeBlockStart();
	if(gTraceInfo.exec_map.map_start == NULL)
	{
		get_map_address(CALLER_ADDRESS, &(gTraceInfo.exec_map.map_start),
						&(gTraceInfo.exec_map.map_end));
	}
	LIFECYCLE_PROBE_BLOCK("INITIALIZING");
	probeBlockEnd();

	ret = uiapp_executep(pUiAppFactory, pArguments);

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_LIFECYCLE, LC_LIFECYCLE,
			  "pp", pUiAppFactory, pArguments);
	PACK_COMMON_END(ret, uiapp_executep, 0);
	FLUSH_LOCAL_BUF();

	return ret;
}

void _AppImpl::OnTerminate(void* user_data)
{
	typedef void (*methodType)(void*);
	static methodType appimpl_onterminatep;
	DECLARE_COMMON_VARIABLE;
	
	GET_REAL_FUNC_OSP(_ZN5Tizen3App8_AppImpl11OnTerminateEPv, LIBOSP_APPFW, appimpl_onterminatep);

	probeBlockStart();
	LIFECYCLE_PROBE_BLOCK("TERMINATING");
	probeBlockEnd();

	appimpl_onterminatep(user_data);
}

void _AppImpl::OnDeviceOrientationChanged(app_device_orientation_e orientation, void* user_data)
{
	typedef void (*methodType)(app_device_orientation_e, void*);
	static methodType appimpl_ondeviceorientationchangedp;
	
	GET_REAL_FUNC_OSP(_ZN5Tizen3App8_AppImpl26OnDeviceOrientationChangedE24app_device_orientation_ePv,
			LIBOSP_APPFW, appimpl_ondeviceorientationchangedp);

	probeBlockStart();
	on_orientation_changed((int)orientation, false);
	probeBlockEnd();

	appimpl_ondeviceorientationchangedp(orientation, user_data);
}

void _AppInfo::SetAppState(AppState appState)
{
	typedef void (*methodType)(AppState appstate);
	static methodType appinfo_setappstatep;
	DECLARE_COMMON_VARIABLE;
	
	GET_REAL_FUNC_OSP(_ZN5Tizen3App8_AppInfo11SetAppStateENS0_8AppStateE, LIBOSP_APPFW, appinfo_setappstatep);

	probeBlockStart();
	if(appState == RUNNING)
	{
		LIFECYCLE_PROBE_BLOCK("RUNNING");
	}
	probeBlockEnd();

	appinfo_setappstatep(appState);
}

void _UiAppImpl::OnBackground(void)
{
	typedef void (_UiAppImpl::*methodType)(void);
	static methodType uiappimpl_onbackgroundp;
	DECLARE_COMMON_VARIABLE;
	
	GET_REAL_FUNC_OSP(_ZN5Tizen3App10_UiAppImpl12OnBackgroundEv, LIBOSP_UIFW, uiappimpl_onbackgroundp);

	probeBlockStart();
	SCREENSHOT_LOCK();
	LIFECYCLE_PROBE_BLOCK("PAUSING");
	probeBlockEnd();

	(this->*uiappimpl_onbackgroundp)();
}

void _UiAppImpl::OnForeground(void)
{
	typedef void (_UiAppImpl::*methodType)(void);
	static methodType uiappimpl_onforegroundp;
	DECLARE_COMMON_VARIABLE;
	
	GET_REAL_FUNC_OSP(_ZN5Tizen3App10_UiAppImpl12OnForegroundEv, LIBOSP_UIFW, uiappimpl_onforegroundp);

	probeBlockStart();
	LIFECYCLE_PROBE_BLOCK("RUNNING");
	SCREENSHOT_UNLOCK();
//	SCREENSHOT_DONE();
	probeBlockEnd();

	(this->*uiappimpl_onforegroundp)();
}

/*
void _UiAppImpl::OnDeviceOrientationChanged(app_device_orientation_e orientation)
{
	typedef void (*methodType)(_UiAppImpl* th, app_device_orientation_e orientation);
	static methodType uiappimpl_ondeviceorientationchangedp;
	
	GET_REAL_FUNC_OSP(_ZThn4_N5Tizen3App10_UiAppImpl26OnDeviceOrientationChangedE24app_device_orientation_e,
			LIBOSP_UIFW, uiappimpl_ondeviceorientationchangedp);

	probeBlockStart();
	on_orientation_changed((int)orientation, false);
	probeBlockEnd();

	uiappimpl_ondeviceorientationchangedp(static_cast<_UiAppImpl*>(dynamic_cast<_IAppImpl*>(this)), orientation);
}
*/
} }	// end of namespace

