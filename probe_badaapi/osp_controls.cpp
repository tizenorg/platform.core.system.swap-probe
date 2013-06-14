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

#include <typeinfo>
#include <FApp.h>
#include <FUi.h>

#include "daprobe.h"
#include "dacollection.h"
#include "dahelper.h"
#include "osp_probe.h"

#include "binproto.h"

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Animations;

extern IFrameAnimatorEventListener& GetFrameAnimatorEventListener();

bool IsRegisteredFrameAnimatorEventListener = false;

namespace Tizen { namespace App {

result UiApp::AddFrame(const Tizen::Ui::Controls::Frame& frame)
{
	typedef result (UiApp::*methodType)(const Tizen::Ui::Controls::Frame& frame);
	static methodType uiapp_addframep;
	probeInfo_t	probeInfo;
	result ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen3App5UiApp8AddFrameERKNS_2Ui8Controls5FrameE, LIBOSP_UIFW, uiapp_addframep);

	ret = (this->*uiapp_addframep)(frame);

	probeBlockStart();
	if(ret == E_SUCCESS)
	{
		frame.IsInTouchMode();

		if(isOptionEnabled(OPT_UI))
		{
			Control* parent = NULL;
			setProbePoint(&probeInfo);

			PREPARE_LOCAL_BUF();
			PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
					  API_ID_result_UiApp__AddFrame_const_Tizen__Ui__Controls__Frame__frame_,
					  "p", &frame);
			PACK_COMMON_END(ret, 0, 0);
			PACK_UICONTROL(parent);
			PACK_UICONTROL(&frame);
			FLUSH_LOCAL_BUF();
		}
	}
	probeBlockEnd();

	return ret;
}

result UiApp::RemoveFrame(const Tizen::Ui::Controls::Frame &frame)
{
	typedef result (UiApp::*methodType)(const Tizen::Ui::Controls::Frame& frame);
	static methodType uiapp_removeframep;
	probeInfo_t	probeInfo;
	result ret;
	bool bOption;

	GET_REAL_FUNC_OSP(_ZN5Tizen3App5UiApp11RemoveFrameERKNS_2Ui8Controls5FrameE, LIBOSP_UIFW, uiapp_removeframep);

	probeBlockStart();
	
	PREPARE_LOCAL_BUF();
	
	frame.IsInTouchMode();

	if((bOption = isOptionEnabled(OPT_UI)))
	{
		Control* parent = NULL;
		setProbePoint(&probeInfo);
		
		PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
				  API_ID_result_UiApp__RemoveFrame_const_Tizen__Ui__Controls__Frame__frame_,
				  "p", &frame);
		PACK_COMMON_END(0, 0, 0);
		PACK_UICONTROL(parent);
		PACK_UICONTROL(&frame);
	}
	probeBlockEnd();

	ret = (this->*uiapp_removeframep)(frame);

	probeBlockStart();
	
	if(bOption)
	{
		PACK_RETURN_END(ret);
	}
	
	FLUSH_LOCAL_BUF();
		
	probeBlockEnd();
	
	return ret;
}

} }		// end of namespce Tizen::App


namespace Tizen { namespace Ui {

bool Control::IsInTouchMode(void) const
{
	typedef bool (Control::*methodType)(void) const;
	static methodType control_isintouchmodep;

	GET_REAL_FUNC_OSP(_ZNK5Tizen2Ui7Control13IsInTouchModeEv, LIBOSP_UIFW, control_isintouchmodep);

	probeBlockStart();
	add_object_hash_class((void*)(this), typeid(*this).name());
	probeBlockEnd();

	return (this->*control_isintouchmodep)();
}

void Control::SetName(const Tizen::Base::String &name)
{
	typedef void (Control::*methodType)(const Tizen::Base::String &name);
	static methodType control_setnamep;
	probeInfo_t	probeInfo;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui7Control7SetNameERKNS_4Base6StringE, LIBOSP_UIFW, control_setnamep);

	(this->*control_setnamep)(name);

	probeBlockStart();
	IsInTouchMode();

	if(isOptionEnabled(OPT_UI))
	{
		Control* parent = NULL;
		setProbePoint(&probeInfo);
		
		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
				  API_ID_void_Control__SetName_const_Tizen__Base__String__name_,
				  "p", &name);
		PACK_COMMON_END(0, 0, 0);
		PACK_UICONTROL(this);
		PACK_UICONTROL(parent);
		FLUSH_LOCAL_BUF();
	}
	probeBlockEnd();
}

result Container::AddControl(const Control &control)
{
	typedef result (Container::*methodType)(const Control &control);
	static methodType container_addcontrolp;
	probeInfo_t	probeInfo;
	result ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9Container10AddControlERKNS0_7ControlE, LIBOSP_UIFW, container_addcontrolp);

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
		IsInTouchMode();
		control.IsInTouchMode();

		if(isOptionEnabled(OPT_UI))
		{
			setProbePoint(&probeInfo);
			
			PREPARE_LOCAL_BUF();
			PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
					  API_ID_result_Container__AddControl_const_Control__control_,
					  "p", &control);
			PACK_COMMON_END(ret, 0, 0);
			PACK_UICONTROL(this);
			PACK_UICONTROL(&control);
			FLUSH_LOCAL_BUF();
		}
	}
	probeBlockEnd();

	return ret;
}

result Container::AddControl(Control* control)
{
	typedef result (Container::*methodType)(Control* control);
	static methodType container_addcontrolp;
	probeInfo_t	probeInfo;
	result ret;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9Container10AddControlEPNS0_7ControlE, LIBOSP_UIFW, container_addcontrolp);

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
		IsInTouchMode();
		control->IsInTouchMode();

		if(isOptionEnabled(OPT_UI))
		{
			setProbePoint(&probeInfo);

			PREPARE_LOCAL_BUF();
			PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
					  API_ID_result_Container__AddControl_Control__control_,
					  "p", control);
			PACK_COMMON_END(ret, 0, 0);
			PACK_UICONTROL(this);
			PACK_UICONTROL(control);
			FLUSH_LOCAL_BUF();
		}
	}
	probeBlockEnd();

	return ret;
}

result Container::RemoveControl(const Control &control)
{
	typedef result (Container::*methodType)(const Control &control);
	static methodType container_removecontrolp;
	probeInfo_t	probeInfo;
	result ret;
	bool bOption;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9Container13RemoveControlERKNS0_7ControlE, LIBOSP_UIFW, container_removecontrolp);

	probeBlockStart();

	PREPARE_LOCAL_BUF();

	control.IsInTouchMode();

	if((bOption = isOptionEnabled(OPT_UI)))
	{
		setProbePoint(&probeInfo);
		
		PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
				  API_ID_result_Container__RemoveControl_const_Control__control_,
				  "p", &control);
		PACK_COMMON_END(0, 0, 0);
		PACK_UICONTROL(this);
		PACK_UICONTROL(&control);
	}
	probeBlockEnd();

	ret = (this->*container_removecontrolp)(control);

	probeBlockStart();

	if(bOption)
	{
		PACK_RETURN_END(ret);
	}
	
	FLUSH_LOCAL_BUF();	

	probeBlockEnd();
			
	return ret;
}

result Container::RemoveControl(Control* control)
{
	typedef result (Container::*methodType)(Control* control);
	static methodType container_removecontrolp;
	probeInfo_t	probeInfo;
	result ret;
	bool bOption;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9Container13RemoveControlEPNS0_7ControlE, LIBOSP_UIFW, container_removecontrolp);

	probeBlockStart();

	PREPARE_LOCAL_BUF();

	control->IsInTouchMode();

	if((bOption = isOptionEnabled(OPT_UI)))
	{
		setProbePoint(&probeInfo);

		PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
				  API_ID_result_Container__RemoveControl_Control__control_,
				  "p", control);
		PACK_COMMON_END(0, 0, 0);
		PACK_UICONTROL(this);
		PACK_UICONTROL(control);
	}
	probeBlockEnd();

	ret = (this->*container_removecontrolp)(control);

	probeBlockStart();

	if(bOption)
	{
		PACK_RETURN_END(ret);
	}
	
	FLUSH_LOCAL_BUF();	

	probeBlockEnd();

	return ret;
}

result Container::RemoveControl(int index)
{
	typedef result (Container::*methodType)(int index);
	static methodType container_removecontrolip;
	probeInfo_t	probeInfo;
	result ret;
	bool bOption;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9Container13RemoveControlEi, LIBOSP_UIFW, container_removecontrolip);

	probeBlockStart();

	PREPARE_LOCAL_BUF();

	Control* pcontrol = GetControl(index);
	pcontrol->IsInTouchMode();

	if((bOption = isOptionEnabled(OPT_UI)))
	{
		setProbePoint(&probeInfo);
		
		PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
				  API_ID_result_Container__RemoveControl_int_index_,
				  "d", index);
		PACK_COMMON_END(0, 0, 0);
		PACK_UICONTROL(this);
		PACK_UICONTROL(pcontrol);
	}
	probeBlockEnd();

	ret = (this->*container_removecontrolip)(index);

	probeBlockStart();

	if(bOption)
	{
		PACK_RETURN_END(ret);
	}

	FLUSH_LOCAL_BUF();

	probeBlockEnd();
	
	return ret;
}

void Container::RemoveAllControls(void)
{
	typedef void (Container::*methodType)(void);
	static methodType container_removeallcontrolp;
	probeInfo_t	probeInfo;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9Container17RemoveAllControlsEv, LIBOSP_UIFW, container_removeallcontrolp);

	probeBlockStart();
	if(isOptionEnabled(OPT_UI))
	{
		Control* pcontrol = NULL;
		setProbePoint(&probeInfo);
		
		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
				  API_ID_void_Container__RemoveAllControls_void_,
				  "", 0);
		PACK_COMMON_END(0, 0, 0);
		PACK_UICONTROL(this);
		PACK_UICONTROL(pcontrol);
		FLUSH_LOCAL_BUF();
	}
	probeBlockEnd();

	(this->*container_removeallcontrolp)();
}

} }	// end of namespace Tizen::Ui
