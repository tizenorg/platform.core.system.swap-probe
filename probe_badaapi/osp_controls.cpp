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

#include <typeinfo>
#include <FApp.h>
#include <FUi.h>

#include "daprobe.h"
#include "dacollection.h"
#include "dahelper.h"
#include "osp_probe.h"

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Animations;

#define APPEND_LOG_CONTROL_OSP(CONTROL)														\
	do {																					\
		if(unlikely(CONTROL == NULL)) {														\
			log.length += sprintf(log.data + log.length, "`,`,`,");							\
		} else {																			\
			char *type = NULL, *name = NULL;												\
			if(find_object_hash((void*)(CONTROL), &type, &name) == 1) {						\
				log.length += sprintf(log.data + log.length, "`,%s`,%s", type, name);		\
			} else {																		\
				log.length += sprintf(log.data + log.length, "`,`,");						\
			}																				\
			log.length += sprintf(log.data + log.length, "`,%p",							\
					(void*)(CONTROL));														\
		}																					\
	} while(0)

extern IFrameAnimatorEventListener& GetFrameAnimatorEventListener();

bool IsRegisteredFrameAnimatorEventListener = false;

namespace Tizen { namespace App {

result UiApp::AddFrame(const Tizen::Ui::Controls::Frame& frame)
{
	typedef result (UiApp::*methodType)(const Tizen::Ui::Controls::Frame& frame);
	static methodType uiapp_addframep;
	DECLARE_COMMON_VARIABLE;
	result ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen3App5UiApp8AddFrameERKNS_2Ui8Controls5FrameE, libosp-uifw.so, uiapp_addframep);

	ret = (this->*uiapp_addframep)(frame);

	probeBlockStart();
	if(ret == E_SUCCESS)
	{
		Control* parent = NULL;
		setProbePoint(&probeInfo);
		INIT_LOG;
		APPEND_LOG_BASIC_NAME(LC_UICREATE, "AddFrame");
		APPEND_LOG_COMMON_NONE(CALLER_ADDRESS);

		frame.IsInTouchMode();

		APPEND_LOG_CONTROL_OSP(parent);
		APPEND_LOG_CONTROL_OSP(&frame);
		printLog(&log, MSG_LOG);
	}
	probeBlockEnd();

	return ret;
}

result UiApp::RemoveFrame(const Tizen::Ui::Controls::Frame &frame)
{
	typedef result (UiApp::*methodType)(const Tizen::Ui::Controls::Frame& frame);
	static methodType uiapp_removeframep;
	DECLARE_COMMON_VARIABLE;
	result ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen3App5UiApp11RemoveFrameERKNS_2Ui8Controls5FrameE, libosp-uifw.so, uiapp_removeframep);

	probeBlockStart();
	Control* parent = NULL;
	setProbePoint(&probeInfo);
	INIT_LOG;
	APPEND_LOG_BASIC_NAME(LC_UICREATE, "RemoveFrame");
	APPEND_LOG_COMMON_NONE(CALLER_ADDRESS);

	frame.IsInTouchMode();

	APPEND_LOG_CONTROL_OSP(parent);
	APPEND_LOG_CONTROL_OSP(&frame);
	probeBlockEnd();

	ret = (this->*uiapp_removeframep)(frame);

	probeBlockStart();
	if(ret == E_SUCCESS)
	{
		printLog(&log, MSG_LOG);
	}
	probeBlockEnd();

	return ret;
}

} }		// end of namespce Tizen::App


namespace Tizen { namespace Ui {

bool Control::IsInTouchMode(void) const
{
	typedef bool (Control::*methodType)(void) const;
	static methodType control_isintouchmodep;

	GET_REAL_FUNC_OSP(_ZNK5Tizen2Ui7Control13IsInTouchModeEv, libosp-uifw.so, control_isintouchmodep);

	probeBlockStart();
	add_object_hash_class((void*)(this), typeid(*this).name());
	probeBlockEnd();

	return (this->*control_isintouchmodep)();
}

void Control::SetName(const Tizen::Base::String &name)
{
	typedef void (Control::*methodType)(const Tizen::Base::String &name);
	static methodType control_setnamep;
	DECLARE_COMMON_VARIABLE;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui7Control7SetNameERKNS_4Base6StringE, libosp-uifw.so, control_setnamep);

	(this->*control_setnamep)(name);

	probeBlockStart();
	Control* parent = NULL;
	setProbePoint(&probeInfo);
	INIT_LOG;
	APPEND_LOG_BASIC_NAME(LC_UICREATE, "SetName");
	APPEND_LOG_COMMON_NONE(CALLER_ADDRESS);

	IsInTouchMode();

	APPEND_LOG_CONTROL_OSP(this);
	APPEND_LOG_CONTROL_OSP(parent);

	printLog(&log, MSG_LOG);
	probeBlockEnd();
}

result Container::AddControl(const Control &control)
{
	typedef result (Container::*methodType)(const Control &control);
	static methodType container_addcontrolp;
	DECLARE_COMMON_VARIABLE;
	result ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9Container10AddControlERKNS0_7ControlE, libosp-uifw.so, container_addcontrolp);

	probeBlockStart();
	if(unlikely(IsRegisteredFrameAnimatorEventListener == false))
	{
		char *type = NULL, *classname = NULL;
		if(likely(find_object_hash((void*)this, &type, &classname) == 1))	// there is entry in object hash
		{
			if(strcmp(type, "Frame") == 0)
			{
				FrameAnimator* fa = ((Frame*)this)->GetFrameAnimator();
				if(fa != NULL)
				{
					fa->AddFrameAnimatorEventListener(GetFrameAnimatorEventListener());
					IsRegisteredFrameAnimatorEventListener = true;
				}
				else	// frame is not yet constructed
				{
				}
			}
		}
	}
	probeBlockEnd();

	ret = (this->*container_addcontrolp)(control);

	probeBlockStart();
	if(ret == E_SUCCESS)
	{
		setProbePoint(&probeInfo);
		INIT_LOG;
		APPEND_LOG_BASIC_NAME(LC_UICREATE, "AddControl");
		APPEND_LOG_COMMON_NONE(CALLER_ADDRESS);

		IsInTouchMode();
		control.IsInTouchMode();

		APPEND_LOG_CONTROL_OSP(this);
		APPEND_LOG_CONTROL_OSP(&control);

		printLog(&log, MSG_LOG);
	}
	probeBlockEnd();

	return ret;
}

result Container::RemoveControl(const Control &control)
{
	typedef result (Container::*methodType)(const Control &control);
	static methodType container_removecontrolp;
	DECLARE_COMMON_VARIABLE;
	result ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9Container13RemoveControlERKNS0_7ControlE, libosp-uifw.so, container_removecontrolp);

	probeBlockStart();
	setProbePoint(&probeInfo);
	INIT_LOG;
	APPEND_LOG_BASIC_NAME(LC_UICREATE, "RemoveControl");
	APPEND_LOG_COMMON_NONE(CALLER_ADDRESS);

	control.IsInTouchMode();

	APPEND_LOG_CONTROL_OSP(this);
	APPEND_LOG_CONTROL_OSP(&control);
	probeBlockEnd();

	ret = (this->*container_removecontrolp)(control);

	probeBlockStart();
	if(ret == E_SUCCESS)
	{
		printLog(&log, MSG_LOG);
	}
	probeBlockEnd();

	return ret;
}

result Container::RemoveControl(int index)
{
	typedef result (Container::*methodType)(int index);
	static methodType container_removecontrolip;
	DECLARE_COMMON_VARIABLE;
	result ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9Container13RemoveControlEi, libosp-uifw.so, container_removecontrolip);

	probeBlockStart();
	Control* pcontrol;
	setProbePoint(&probeInfo);
	INIT_LOG;
	APPEND_LOG_BASIC_NAME(LC_UICREATE, "RemoveControl");
	APPEND_LOG_COMMON_NONE(CALLER_ADDRESS);

	pcontrol = GetControl(index);
	pcontrol->IsInTouchMode();

	APPEND_LOG_CONTROL_OSP(this);
	APPEND_LOG_CONTROL_OSP(pcontrol);
	probeBlockEnd();

	ret = (this->*container_removecontrolip)(index);

	probeBlockStart();
	if(ret == E_SUCCESS)
	{
		printLog(&log, MSG_LOG);
	}
	probeBlockEnd();

	return ret;
}

void Container::RemoveAllControls(void)
{
	typedef void (Container::*methodType)(void);
	static methodType container_removeallcontrolp;
	DECLARE_COMMON_VARIABLE;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9Container17RemoveAllControlsEv, libosp-uifw.so, container_removeallcontrolp);

	probeBlockStart();
	Control* pcontrol = NULL;
	setProbePoint(&probeInfo);
	INIT_LOG;
	APPEND_LOG_BASIC_NAME(LC_UICREATE, "RemoveAllControl");
	APPEND_LOG_COMMON_NONE(CALLER_ADDRESS);

	APPEND_LOG_CONTROL_OSP(this);
	APPEND_LOG_CONTROL_OSP(pcontrol);
	printLog(&log, MSG_LOG);
	probeBlockEnd();

	(this->*container_removeallcontrolp)();
}

} }	// end of namespace Tizen::Ui
