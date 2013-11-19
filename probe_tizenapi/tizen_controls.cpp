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
#include "tizen_probe.h"

#include "binproto.h"

using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Animations;

extern IFrameAnimatorEventListener& GetFrameAnimatorEventListener();

bool IsRegisteredFrameAnimatorEventListener = false;

static enum DaOptions _sopt = OPT_UI;

#define BEFORE_ORIGINAL_TIZEN_UI(FUNCNAME, FUNCTIONPOINTER)				\
		DECLARE_VARIABLE_STANDARD_NORET; 					\
		GET_REAL_FUNC_TIZEN(FUNCNAME, LIBOSP_UIFW, FUNCTIONPOINTER)

namespace Tizen { namespace App {

result UiApp::AddFrame(const Tizen::Ui::Controls::Frame& frame)
{
	typedef result (UiApp::*methodType)(const Tizen::Ui::Controls::Frame& frame);
	static methodType uiapp_addframep;
	result ret;

	BEFORE_ORIGINAL_TIZEN_UI(_ZN5Tizen3App5UiApp8AddFrameERKNS_2Ui8Controls5FrameE, uiapp_addframep);
	PRE_PROBEBLOCK();

	ret = (this->*uiapp_addframep)(frame);

	POST_PACK_PROBEBLOCK_BEGIN();
	if(ret == E_SUCCESS)
	{
		frame.IsInTouchMode();

		Control* parent = NULL;
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
				  API_ID_result_UiApp__AddFrame_const_Tizen__Ui__Controls__Frame__frame_,
				  "p", voidp_to_uint64(&frame));
		PACK_COMMON_END('x', ret, 0, 0);
		PACK_UICONTROL(parent);
		PACK_UICONTROL(&frame);
		FLUSH_LOCAL_BUF();
	}
	POST_PACK_PROBEBLOCK_END();

	return ret;
}

result UiApp::RemoveFrame(const Tizen::Ui::Controls::Frame &frame)
{
	typedef result (UiApp::*methodType)(const Tizen::Ui::Controls::Frame& frame);
	static methodType uiapp_removeframep;
	result ret = 0;

	BEFORE_ORIGINAL_TIZEN_UI(_ZN5Tizen3App5UiApp11RemoveFrameERKNS_2Ui8Controls5FrameE, uiapp_removeframep);

	PREPARE_LOCAL_BUF();

	PRE_PROBEBLOCK_BEGIN();
	Control *parent = NULL;

	PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
			  API_ID_result_UiApp__RemoveFrame_const_Tizen__Ui__Controls__Frame__frame_,
			  "p", voidp_to_uint64(&frame));
	PACK_COMMON_END('x', ret, 0, 0);
	PACK_UICONTROL(parent);
	PACK_UICONTROL(&frame);
	PRE_PROBEBLOCK_END();

	frame.IsInTouchMode();

	ret = (this->*uiapp_removeframep)(frame);

	POST_PACK_PROBEBLOCK_BEGIN();
	PACK_RETURN_END('x', ret);
	FLUSH_LOCAL_BUF();
	POST_PACK_PROBEBLOCK_END();


	return ret;
}

} }		// end of namespce Tizen::App


namespace Tizen { namespace Ui {

bool Control::IsInTouchMode(void) const
{
	typedef bool (Control::*methodType)(void) const;
	static methodType control_isintouchmodep;

	GET_REAL_FUNC_TIZEN(_ZNK5Tizen2Ui7Control13IsInTouchModeEv, LIBOSP_UIFW, control_isintouchmodep);

	probeBlockStart();
	add_object_hash_class((void*)(this), typeid(*this).name());
	probeBlockEnd();

	return (this->*control_isintouchmodep)();
}

void Control::SetName(const Tizen::Base::String &name)
{
	typedef void (Control::*methodType)(const Tizen::Base::String &name);
	static methodType control_setnamep;

	BEFORE_ORIGINAL_TIZEN_UI(_ZN5Tizen2Ui7Control7SetNameERKNS_4Base6StringE, control_setnamep);
	PRE_PROBEBLOCK();

	(this->*control_setnamep)(name);

	POST_PACK_PROBEBLOCK_BEGIN();

	IsInTouchMode();
	Control* parent = NULL;

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
			  API_ID_void_Control__SetName_const_Tizen__Base__String__name_,
			  "p", voidp_to_uint64(&name));
	PACK_COMMON_END('v', 0, 0, 0);
	PACK_UICONTROL(this);
	PACK_UICONTROL(parent);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();
}

result Container::AddControl(const Control &control)
{
	typedef result (Container::*methodType)(const Control &control);
	static methodType container_addcontrolp;
	result ret;

	BEFORE_ORIGINAL_TIZEN_UI(_ZN5Tizen2Ui9Container10AddControlERKNS0_7ControlE, container_addcontrolp);
	bfiltering = false;

	PRE_PROBEBLOCK_BEGIN();
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
	PRE_PROBEBLOCK_END();

	ret = (this->*container_addcontrolp)(control);

	POST_PACK_PROBEBLOCK_BEGIN();
	if(ret == E_SUCCESS)
	{
		IsInTouchMode();
		control.IsInTouchMode();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
				  API_ID_result_Container__AddControl_const_Control__control_,
				  "p", voidp_to_uint64(&control));
		PACK_COMMON_END('x', ret, 0, 0);
		PACK_UICONTROL(this);
		PACK_UICONTROL(&control);
		FLUSH_LOCAL_BUF();
	}
	POST_PACK_PROBEBLOCK_END();

	return ret;
}

result Container::AddControl(Control* control)
{
	typedef result (Container::*methodType)(Control* control);
	static methodType container_addcontrolp;
	result ret;

	BEFORE_ORIGINAL_TIZEN_UI(_ZN5Tizen2Ui9Container10AddControlEPNS0_7ControlE, container_addcontrolp);
	bfiltering = false;

	PRE_PROBEBLOCK_BEGIN();
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
	PRE_PROBEBLOCK_END();

	ret = (this->*container_addcontrolp)(control);

	POST_PACK_PROBEBLOCK_BEGIN();
	if(ret == E_SUCCESS)
	{
		IsInTouchMode();
		control->IsInTouchMode();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
				  API_ID_result_Container__AddControl_Control__control_,
				  "p", voidp_to_uint64(control));
		PACK_COMMON_END('x', ret, 0, 0);
		PACK_UICONTROL(this);
		PACK_UICONTROL(control);
		FLUSH_LOCAL_BUF();
	}
	POST_PACK_PROBEBLOCK_END();

	return ret;
}

result Container::RemoveControl(const Control &control)
{
	typedef result (Container::*methodType)(const Control &control);
	static methodType container_removecontrolp;
	result ret = 0;

	BEFORE_ORIGINAL_TIZEN_UI(_ZN5Tizen2Ui9Container13RemoveControlERKNS0_7ControlE, container_removecontrolp);
	bfiltering = false;

	PREPARE_LOCAL_BUF();

	control.IsInTouchMode();

	PRE_PROBEBLOCK_BEGIN();
	PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
			  API_ID_result_Container__RemoveControl_const_Control__control_,
			  "p", voidp_to_uint64(&control));
	PACK_COMMON_END('x', ret, 0, 0);
	PACK_UICONTROL(this);
	PACK_UICONTROL(&control);
	PRE_PROBEBLOCK_END();

	ret = (this->*container_removecontrolp)(control);

	POST_PACK_PROBEBLOCK_BEGIN();
	PACK_RETURN_END('x', ret);
	FLUSH_LOCAL_BUF();
	POST_PACK_PROBEBLOCK_END();

	return ret;
}

result Container::RemoveControl(Control* control)
{
	typedef result (Container::*methodType)(Control* control);
	static methodType container_removecontrolp;
	result ret = 0;

	BEFORE_ORIGINAL_TIZEN_UI(_ZN5Tizen2Ui9Container13RemoveControlEPNS0_7ControlE, container_removecontrolp);
	bfiltering = false;

	PREPARE_LOCAL_BUF();

	PRE_PROBEBLOCK_BEGIN();

	control->IsInTouchMode();

	PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
			  API_ID_result_Container__RemoveControl_Control__control_,
			  "p", voidp_to_uint64(control));
	PACK_COMMON_END('x', ret, 0, 0);
	PACK_UICONTROL(this);
	PACK_UICONTROL(control);

	PRE_PROBEBLOCK_END();

	ret = (this->*container_removecontrolp)(control);

	POST_PACK_PROBEBLOCK_BEGIN();
	PACK_RETURN_END('x', ret);
	FLUSH_LOCAL_BUF();
	POST_PACK_PROBEBLOCK_END();

	return ret;
}

result Container::RemoveControl(int index)
{
	typedef result (Container::*methodType)(int index);
	static methodType container_removecontrolip;
	result ret = 0;
	Control *pcontrol = NULL;

	BEFORE_ORIGINAL_TIZEN_UI(_ZN5Tizen2Ui9Container13RemoveControlEi, container_removecontrolip);
	bfiltering = false;

	PREPARE_LOCAL_BUF();

	PRE_PROBEBLOCK_BEGIN();

	pcontrol = GetControl(index);
	pcontrol->IsInTouchMode();

	PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
			  API_ID_result_Container__RemoveControl_int_index_,
			  "d", index);
	PACK_COMMON_END('x', ret, 0, 0);
	PACK_UICONTROL(this);
	PACK_UICONTROL(pcontrol);
	PRE_PROBEBLOCK_END();

	ret = (this->*container_removecontrolip)(index);

	POST_PACK_PROBEBLOCK_BEGIN();
	PACK_RETURN_END('x', ret);
	FLUSH_LOCAL_BUF();
	POST_PACK_PROBEBLOCK_END();

	return ret;
}

void Container::RemoveAllControls(void)
{
	typedef void (Container::*methodType)(void);
	static methodType container_removeallcontrolp;

	BEFORE_ORIGINAL_TIZEN_UI(_ZN5Tizen2Ui9Container17RemoveAllControlsEv, container_removeallcontrolp);
	bfiltering = false;

	PRE_PROBEBLOCK_BEGIN();

	Control* pcontrol = NULL;

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_UICONTROL,
			  API_ID_void_Container__RemoveAllControls_void_,
			  "", 0);
	PACK_COMMON_END('v', 0, 0, 0);
	PACK_UICONTROL(this);
	PACK_UICONTROL(pcontrol);
	FLUSH_LOCAL_BUF();
	PRE_PROBEBLOCK_END();

	(this->*container_removeallcontrolp)();

	POST_PACK_PROBEBLOCK_BEGIN();
	POST_PACK_PROBEBLOCK_END();

}

} }	// end of namespace Tizen::Ui
