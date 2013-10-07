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

#include <Evas.h>

#include <FUi.h>
//#include <FMedia.h>

#include "daprobe.h"
#include "dahelper.h"
#include "dacollection.h"
#include "daerror.h"
//#include "da_ui.h"
#include "tizen_probe.h"

using namespace Tizen::Base;
using namespace Tizen::Ui;
using namespace Tizen::Graphics;
using namespace Tizen::Media;

/*
#define LIBUTILX 	"libutilX.so.1"

typedef void* (*utilx_create_screen_shot_type)(Display* dpy, int width, int height);
typedef void (*utilx_release_screen_shot_type)(void);

static Tizen::Graphics::Bitmap* sysutil_capture_screen()
{
	static utilx_create_screen_shot_type utilx_create_screen_shotp;
	static utilx_release_screen_shot_type utilx_release_screen_shotp;
	result r;
	Bitmap* pBitmap = NULL;

	if(unlikely(utilx_create_screen_shotp == NULL))
	{
		void* lib_handle = dlopen(LIBUTILX, RTLD_LAZY);
		if(lib_handle == NULL) {
			SetLastResult(ERR_DLOPEN);
			return NULL;
		}
		utilx_create_screen_shotp = (utilx_create_screen_shot_type)dlsym(lib_handle, "utilx_create_screen_shot");
		if(utilx_create_screen_shotp == NULL || dlerror() != NULL) {
			SetLastResult(ERR_DLSYM);
			return NULL;
		}
	}

	if(unlikely(utilx_release_screen_shotp == NULL))
	{
		void* lib_handle = dlopen(LIBUTILX, RTLD_LAZY);
		if(lib_handle == NULL) {
			SetLastResult(ERR_DLOPEN);
			return NULL;
		}
		utilx_release_screen_shotp = (utilx_release_screen_shot_type)dlsym(lib_handle, "utilx_release_screen_shot");
		if(utilx_release_screen_shotp == NULL || dlerror() != NULL) {
			SetLastResult(ERR_DLSYM);
			return NULL;
		}
	}

	Display* pDpy = XOpenDisplay(NULL);
	if(pDpy == NULL)
	{
		SetLastResult(ERR_USER - 1);
		return NULL;
	}

	int width = DisplayWidth(pDpy, DefaultScreen(pDpy));
	int height = DisplayHeight(pDpy, DefaultScreen(pDpy));

	void* pDump = utilx_create_screen_shotp(pDpy, width, height);
	if(likely(pDump != NULL))
	{
		ByteBuffer buffer;
		r = buffer.Construct(static_cast<byte*>(pDump), 0, width*height*4, width*height*4);\
		if(likely(r == E_SUCCESS))
		{
			Tizen::Graphics::Dimension dim(width, height);
			Bitmap* pBitmap  = new Bitmap();
			if(likely(pBitmap != NULL))
			{
				r = pBitmap->Construct(buffer, dim, BITMAP_PIXEL_FORMAT_ARGB8888);
				if(unlikely(r != E_SUCCESS))
				{
					SetLastResult(r);
					delete pBitmap;
					pBitmap = NULL;
				}
				else { SetLastResult(E_SUCCESS); }
			}
			else { SetLastResult(ERR_OUTOFMEMORY); }
		}
		else { SetLastResult(r); }
	}
	else { SetLastResult(ERR_USER - 2); }

	utilx_release_screen_shotp();

	return pBitmap;
}

static Bitmap* getBitmapFromBuffer(void* pDump, int width, int height)
{
	result r;
	ByteBuffer buffer;
	Bitmap* pBitmap = NULL;

	r = buffer.Construct(static_cast<byte*>(pDump), 0, width*height*4, width*height*4);
	if(likely(r == E_SUCCESS))
	{
		Tizen::Graphics::Dimension dim(width, height);
		pBitmap  = new Bitmap();
		if(likely(pBitmap != NULL))
		{
			r = pBitmap->Construct(buffer, dim, BITMAP_PIXEL_FORMAT_ARGB8888);
			if(unlikely(r != E_SUCCESS))
			{
				SetLastResult(r);
				delete pBitmap;
				pBitmap = NULL;
			}
			else { SetLastResult(E_SUCCESS); }
		}
		else { SetLastResult(ERR_OUTOFMEMORY); }
	}
	else { SetLastResult(r); }

	return pBitmap;
}

// return 0 if succeed
// return -1 if failed to capture
int captureScreen()
{
	Image img;
	Bitmap* bitmap;
	probeInfo_t	probeInfo; log_t log;
	result r;
	int ret = 0, width, height;
	char* capbuf;

	probeBlockStart();

	capbuf = captureScreenShotX(&width, &height);
	if(capbuf != NULL)
	{
		bitmap = getBitmapFromBuffer((void*)capbuf, width, height);
		if(bitmap != NULL)
		{
			String dstPath;
			setProbePoint(&probeInfo);

			dstPath.Format(MAX_PATH_LENGTH, L"/tmp/da/%d.jpg", probeInfo.eventIndex);
			img.Construct();
			r = img.EncodeToFile(*bitmap, IMG_FORMAT_JPG, dstPath, true);
			if(r == E_SUCCESS)
			{
				INIT_LOG;
				APPEND_LOG_BASIC_NAME(LC_SNAPSHOT, "captureScreen");
				APPEND_LOG_COMMON_NONE(0);
				log.length += sprintf(log.data + log.length, "`,%S", dstPath.GetPointer());
				printLog(&log, MSG_LOG);
			}
			else
			{
				ret = -1;
			}

			delete bitmap;
		}
		else
		{
			char buf[128];
			r = GetLastResult();
			sprintf(buf, "bitmap is null (%p, %d, %s)\n", capbuf, (int)r, GetErrorMessage(r));
			ret = -1;
		}
	}
	else
	{
		ret = -1;
	}

	releaseScreenShotX();
	probeBlockEnd();

	return ret;
}
*/

namespace Tizen { namespace Ui {
/*
class _EcoreEvas
{
public:
	Evas* GetEvas(void) const;
};

class _EcoreEvasMgr
{
	void SetEcoreEvas(const _EcoreEvas& ecoreevas);
};


void _EcoreEvasMgr::SetEcoreEvas(const _EcoreEvas& ecoreevas)
{
	typedef void (_EcoreEvasMgr::*methodType)(const _EcoreEvas& ecoreevas);
	static methodType _ecoreevasmgr_setecoreevasp;

	GET_REAL_FUNC_TIZEN(_ZN5Tizen2Ui13_EcoreEvasMgr12SetEcoreEvasERKNS0_10_EcoreEvasE,
			LIBOSP_UIFW, _ecoreevasmgr_setecoreevasp);

	probeBlockStart();
	evas_event_callback_add(ecoreevas.GetEvas(),
		EVAS_CALLBACK_RENDER_FLUSH_POST, _cb_render_post, NULL);
	probeBlockEnd();

	(this->*_ecoreevasmgr_setecoreevasp)(ecoreevas);
}
*/

result Control::SetShowState(bool state)
{
	typedef result (Control::*methodType)(bool state);
	static methodType control_setshowstatep;
	result ret;

	GET_REAL_FUNC_TIZEN(_ZN5Tizen2Ui7Control12SetShowStateEb,
			LIBOSP_UIFW, control_setshowstatep);

	ret = (this->*control_setshowstatep)(state);

	probeBlockStart();
	{
		char *type, *classname;
		if(find_object_hash((void*)this, &type, &classname) == 1)
		{
			if(strcmp(type, "Panel") == 0 || strcmp(type, "OverlayPanel") == 0 || strcmp(type, "ScrollPanel") == 0)
			{
				SCREENSHOT_SET();
//				SCREENSHOT_DONE();
			}
			else
			{
				// do nothing
			}
		}
		else
		{
			// never happened
		}
	}
	probeBlockEnd();

	return ret;
}

namespace Controls {

result Frame::SetCurrentForm(const Form& form)
{
	typedef result (Frame::*methodType)(const Form& form);
	static methodType frame_setcurrentformp;
	result ret;

	GET_REAL_FUNC_TIZEN(_ZN5Tizen2Ui8Controls5Frame14SetCurrentFormERKNS1_4FormE,
			LIBOSP_UIFW, frame_setcurrentformp);

	ret = (this->*frame_setcurrentformp)(form);

	probeBlockStart();
	SCREENSHOT_SET();
//	SCREENSHOT_DONE();
	probeBlockEnd();

	return ret;
}

result Frame::SetCurrentForm(Form* pForm)
{
	typedef result (Frame::*methodType)(Form* pForm);
	static methodType frame_setcurrentformp;
	result ret;

	GET_REAL_FUNC_TIZEN(_ZN5Tizen2Ui8Controls5Frame14SetCurrentFormEPNS1_4FormE,
			LIBOSP_UIFW, frame_setcurrentformp);

	ret = (this->*frame_setcurrentformp)(pForm);

	probeBlockStart();
	SCREENSHOT_SET();
//	SCREENSHOT_DONE();
	probeBlockEnd();

	return ret;
}


}		// end of namespace Tizen::Ui::Controls

namespace Animations {

result FrameAnimator::SetCurrentForm(const Tizen::Ui::Controls::Form& form)
{
	typedef result (FrameAnimator::*methodType)(const Tizen::Ui::Controls::Form& form);
	static methodType frameanimator_setcurrentformp;
	result ret;

	GET_REAL_FUNC_TIZEN(_ZN5Tizen2Ui10Animations13FrameAnimator14SetCurrentFormERKNS0_8Controls4FormE,
			LIBOSP_UIFW, frameanimator_setcurrentformp);

	ret = (this->*frameanimator_setcurrentformp)(form);

	probeBlockStart();
	SCREENSHOT_SET();
//	SCREENSHOT_DONE();
	probeBlockEnd();

	return ret;
}

result FrameAnimator::SetCurrentForm(Tizen::Ui::Controls::Form* pForm)
{
	typedef result (FrameAnimator::*methodType)(Tizen::Ui::Controls::Form* pForm);
	static methodType frameanimator_setcurrentformp;
	result ret;

	GET_REAL_FUNC_TIZEN(_ZN5Tizen2Ui10Animations13FrameAnimator14SetCurrentFormEPNS0_8Controls4FormE,
			LIBOSP_UIFW, frameanimator_setcurrentformp);

	ret = (this->*frameanimator_setcurrentformp)(pForm);

	probeBlockStart();
	SCREENSHOT_SET();
//	SCREENSHOT_DONE();
	probeBlockEnd();

	return ret;
}


}		// end of namespace Tizen::Ui::Animations

} }		// end of namespace Tizen::Ui


