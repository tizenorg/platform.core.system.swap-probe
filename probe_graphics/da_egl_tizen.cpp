/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2014 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Vitaliy Cherepanov <v.cherepanov@samsung.com>
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

#include <egl_macro.h>
#include <egl.h>
#include "da_gles20.h"
#include "daprobe.h"
#include "binproto.h"
#include "common_probe_init.h"

#ifndef BEFORE_EGL
#define BEFORE_EGL	BEFORE_EGL_TIZEN
#endif

static enum DaOptions _sopt = OPT_GLES;
static __thread EGLint egl_error_external = EGL_SUCCESS;

//HANDLER_DEF(EGLint, eglGetError, void)
HANDLER_WRAPPERS(EGLint, eglGetError, void)
{
	typedef EGLint (*methodType)(void);
	BEFORE_EGL(eglGetError);
	EGLint ret = eglGetErrorp();

	if (egl_error_external == EGL_SUCCESS)
		egl_error_external = ret;

	if (blockresult) {
		//external call
		ret = egl_error_external;
		egl_error_external = EGL_SUCCESS;
	}

	AFTER_NO_PARAM('d', ret, APITYPE_CONTEXT, call_type, caller, "");

	return ret;
}

//HANDLED_DEF(EGLDisplay, eglGetDisplay, EGLNativeDisplayType display_id)
HANDLED_WRAPPERS(EGLDisplay, eglGetDisplay, EGLNativeDisplayType, display_id)
{
	typedef EGLDisplay (*methodType)(EGLNativeDisplayType display_id);
	/* probe prepare */
	BEFORE_EGL(eglGetDisplay);
	/* call original function */
	EGLDisplay ret = eglGetDisplayp(display_id);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('p', voidp_to_uint64(ret), APITYPE_CONTEXT, call_type, caller, "", "p",
	      voidp_to_uint64(display_id));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglInitialize, EGLDisplay dpy, EGLint *major,
//	    EGLint *minor)
HANDLER_WRAPPERS(EGLBoolean, eglInitialize, EGLDisplay, dpy, EGLint *, major,
		 EGLint *, minor)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLint *major,
					 EGLint *minor);
	/* probe prepare */
	BEFORE_EGL(eglInitialize);
	/* call original function */
	EGLBoolean ret = eglInitializep(dpy, major, minor);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "", "ppp", voidp_to_uint64(dpy),
	      voidp_to_uint64(major), voidp_to_uint64(minor));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglTerminate, EGLDisplay dpy)
HANDLER_WRAPPERS(EGLBoolean, eglTerminate, EGLDisplay, dpy)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy);
	/* probe prepare */
	BEFORE_EGL(eglTerminate);
	/* call original function */
	EGLBoolean ret = eglTerminatep(dpy);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "", "p", voidp_to_uint64(dpy));
	return ret;
}

//HANDLER_DEF(const char *, eglQueryString, EGLDisplay dpy, EGLint name)
HANDLER_WRAPPERS(const char *, eglQueryString, EGLDisplay, dpy, EGLint, name)
{
	typedef const char *(*methodType)(EGLDisplay dpy, EGLint name);
	/* probe prepare */
	BEFORE_EGL(eglQueryString);
	/* call original function */
	const char *ret = eglQueryStringp(dpy, name);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('s', ret, APITYPE_CONTEXT, call_type, caller, "", "pd", voidp_to_uint64(dpy), name);
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglGetConfigs, EGLDisplay dpy, EGLConfig *configs,
//	    EGLint config_size, EGLint *num_config)
HANDLER_WRAPPERS(EGLBoolean, eglGetConfigs, EGLDisplay, dpy,
		 EGLConfig *, configs, EGLint, config_size,
		 EGLint *, num_config)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLConfig *configs,
					 EGLint config_size,
					 EGLint *num_config);
	/* probe prepare */
	BEFORE_EGL(eglGetConfigs);
	/* call original function */
	EGLBoolean ret = eglGetConfigsp(dpy, configs, config_size, num_config);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "", "ppdp",
	      voidp_to_uint64(dpy), voidp_to_uint64(configs), config_size,
	      voidp_to_uint64(num_config));
	return ret;
}

//HANDLER_DEF(EGLBoolean eglChooseConfig, EGLDisplay dpy,
//	    const EGLint *attrib_list, EGLConfig *configs, EGLint config_size,
//	    EGLint *num_config)
HANDLER_WRAPPERS(EGLBoolean, eglChooseConfig, EGLDisplay, dpy,
		 const EGLint *, attrib_list, EGLConfig *, configs,
		 EGLint, config_size, EGLint *, num_config)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy,
					 const EGLint *attrib_list,
					 EGLConfig *configs, EGLint config_size,
					 EGLint *num_config);
	/* probe prepare */
	BEFORE_EGL(eglChooseConfig);
	/* call original function */
	EGLBoolean ret = eglChooseConfigp(dpy, attrib_list, configs,
					  config_size, num_config);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "pppdp", voidp_to_uint64(dpy), voidp_to_uint64(attrib_list),
	      voidp_to_uint64(configs), config_size,
	      voidp_to_uint64(num_config));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglGetConfigAttrib, EGLDisplay dpy, EGLConfig config,
//	    EGLint attribute, EGLint *value)
HANDLER_WRAPPERS(EGLBoolean, eglGetConfigAttrib, EGLDisplay, dpy,
		 EGLConfig, config, EGLint, attribute, EGLint *, value)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLConfig config,
					 EGLint attribute, EGLint *value);
	/* probe prepare */
	BEFORE_EGL(eglGetConfigAttrib);
	/* call original function */
	EGLBoolean ret = eglGetConfigAttribp(dpy, config, attribute, value);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "ppdp", voidp_to_uint64(dpy), voidp_to_uint64(config), attribute,
	      voidp_to_uint64(value));
	return ret;
}

//HANDLER_DEF(EGLSurface, eglCreateWindowSurface, EGLDisplay dpy, EGLConfig config,
//	    EGLNativeWindowType win, const EGLint *attrib_list)
HANDLER_WRAPPERS(EGLSurface, eglCreateWindowSurface, EGLDisplay, dpy,
		 EGLConfig, config, EGLNativeWindowType, win,
		 const EGLint *, attrib_list)
{
	typedef EGLSurface (*methodType)(EGLDisplay dpy, EGLConfig config,
					 EGLNativeWindowType win,
					 const EGLint *attrib_list);
	/* probe prepare */
	BEFORE_EGL(eglCreateWindowSurface);
	/* call original function */
	EGLSurface ret = eglCreateWindowSurfacep(dpy, config, win, attrib_list);

	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('p', voidp_to_uint64(ret), APITYPE_CONTEXT, call_type, caller, "",
	      "pppp", voidp_to_uint64(dpy), voidp_to_uint64(config),
	      voidp_to_uint64(win), voidp_to_uint64(attrib_list));
	return ret;
}

//HANLDER_DEF(EGLSurface, eglCreatePbufferSurface, EGLDisplay dpy,
//	    EGLConfig config, const EGLint *attrib_list)
HANLDER_WRAPPERS(EGLSurface, eglCreatePbufferSurface, EGLDisplay, dpy,
		 EGLConfig, config, const EGLint *, attrib_list)
{
	typedef EGLSurface (*methodType)(EGLDisplay dpy, EGLConfig config,
					 const EGLint *attrib_list);
	/* probe prepare */
	BEFORE_EGL(eglCreatePbufferSurface);
	/* call original function */
	EGLSurface ret = eglCreatePbufferSurfacep(dpy, config, attrib_list);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('p', voidp_to_uint64(ret), APITYPE_CONTEXT, call_type, caller, "",
	      "ppp", voidp_to_uint64(dpy), voidp_to_uint64(config),
	      voidp_to_uint64(attrib_list));
	return ret;
}

//HANDLER_DEF(EGLSurface, eglCreatePixmapSurface, EGLDisplay dpy, EGLConfig config,
//	    EGLNativePixmapType pixmap, const EGLint *attrib_list)
HANDLER_WRAPPERS(EGLSurface, eglCreatePixmapSurface, EGLDisplay, dpy,
		 EGLConfig, config, EGLNativePixmapType, pixmap,
		 const EGLint *, attrib_list)
{
	typedef EGLSurface (*methodType)(EGLDisplay dpy, EGLConfig config,
					 EGLNativePixmapType pixmap,
					 const EGLint *attrib_list);
	/* probe prepare */
	BEFORE_EGL(eglCreatePixmapSurface);
	/* call original function */
	EGLSurface ret = eglCreatePixmapSurfacep(dpy, config, pixmap,
						 attrib_list);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('p', voidp_to_uint64(ret), APITYPE_CONTEXT, call_type, caller, "",
	      "pppp", voidp_to_uint64(dpy), voidp_to_uint64(config),
	      voidp_to_uint64(pixmap), voidp_to_uint64(attrib_list));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglDestroySurface, EGLDisplay dpy, EGLSurface surface)
HANDLER_WRAPPERS(EGLBoolean, eglDestroySurface, EGLDisplay, dpy,
		 EGLSurface, surface)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLSurface surface);
	/* probe prepare */
	BEFORE_EGL(eglDestroySurface);
	/* call original function */
	EGLBoolean ret = eglDestroySurfacep(dpy, surface);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "pp", voidp_to_uint64(dpy), voidp_to_uint64(surface));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglQuerySurface, EGLDisplay dpy, EGLSurface surface,
//	    EGLint attribute, EGLint *value)
HANDLER_WRAPPERS(EGLBoolean, eglQuerySurface, EGLDisplay, dpy,
		 EGLSurface, surface, EGLint, attribute, EGLint *, value)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLSurface surface,
					 EGLint attribute, EGLint *value);
	/* probe prepare */
	BEFORE_EGL(eglQuerySurface);
	/* call original function */
	EGLBoolean ret = eglQuerySurfacep(dpy, surface, attribute, value);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "ppdp", voidp_to_uint64(dpy), voidp_to_uint64(surface),
	      attribute, voidp_to_uint64(value));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglBindAPI, EGLenum api)
HANDLER_WRAPPERS(EGLBoolean, eglBindAPI, EGLenum, api)
{
	typedef EGLBoolean (*methodType)(EGLenum api);
	/* probe prepare */
	BEFORE_EGL(eglBindAPI);
	/* call original function */
	EGLBoolean ret = eglBindAPIp(api);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "d", api);
	return ret;
}

//HANDLER_DEF(EGLenum, eglQueryAPI, void)
HANDLER_WRAPPERS(EGLenum, eglQueryAPI, void)
{
	typedef EGLenum (*methodType)(void);
	/* probe prepare */
	BEFORE_EGL(eglQueryAPI);
	/* call original function */
	EGLenum ret = eglQueryAPIp();
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER_NO_PARAM('d', ret, APITYPE_CONTEXT, call_type, caller, "");
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglWaitClient, void)
HANDLER_WRAPPERS(EGLBoolean, eglWaitClient, void)
{
	typedef EGLBoolean (*methodType)(void);
	/* probe prepare */
	BEFORE_EGL(eglWaitClient);
	/* call original function */
	EGLBoolean ret = eglWaitClientp();
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER_NO_PARAM('d', ret, APITYPE_CONTEXT, call_type, caller, "");
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglReleaseThread, void)
HANDLER_WRAPPERS(EGLBoolean, eglReleaseThread, void)
{
	typedef EGLBoolean (*methodType)(void);
	/* probe prepare */
	BEFORE_EGL(eglReleaseThread);
	/* call original function */
	EGLBoolean ret = eglReleaseThreadp();
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER_NO_PARAM('d', ret, APITYPE_CONTEXT, call_type, caller, "");
	return ret;
}

//HANDLER_DEF(EGLSurface, eglCreatePbufferFromClientBuffer ,EGLDisplay dpy,
//	    EGLenum buftype, EGLClientBuffer buffer, EGLConfig config,
//	    const EGLint *attrib_list)
HANDLER_WRAPPERS(EGLSurface, eglCreatePbufferFromClientBuffer, EGLDisplay, dpy,
		 EGLenum, buftype, EGLClientBuffer, buffer, EGLConfig, config,
		 const EGLint *, attrib_list)
{
	typedef EGLSurface (*methodType)(EGLDisplay dpy, EGLenum buftype,
					 EGLClientBuffer buffer,
					 EGLConfig config,
					 const EGLint *attrib_list);
	/* probe prepare */
	BEFORE_EGL(eglCreatePbufferFromClientBuffer);
	/* call original function */
	EGLSurface ret = eglCreatePbufferFromClientBufferp(dpy, buftype, buffer,
							   config, attrib_list);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('p', voidp_to_uint64(ret), APITYPE_CONTEXT, call_type, caller, "",
	      "pdppp", voidp_to_uint64(dpy), buftype, voidp_to_uint64(buffer),
	      voidp_to_uint64(config), voidp_to_uint64(attrib_list));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglSurfaceAttrib, EGLDisplay dpy, EGLSurface surface,
//	    EGLint attribute, EGLint value)
HANDLER_WRAPPERS(EGLBoolean, eglSurfaceAttrib, EGLDisplay, dpy,
		 EGLSurface, surface, EGLint, attribute, EGLint, value)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLSurface surface,
					 EGLint attribute, EGLint value);
	/* probe prepare */
	BEFORE_EGL(eglSurfaceAttrib);
	/* call original function */
	EGLBoolean ret = eglSurfaceAttribp(dpy, surface, attribute, value);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "ppdd", voidp_to_uint64(dpy), voidp_to_uint64(surface), attribute,
	      value);
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglBindTexImage, EGLDisplay dpy, EGLSurface surface,
//	    EGLint buffer)
HANDLER_WRAPPERS(EGLBoolean, eglBindTexImage, EGLDisplay, dpy,
		 EGLSurface, surface, EGLint, buffer)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLSurface surface,
					 EGLint buffer);
	/* probe prepare */
	BEFORE_EGL(eglBindTexImage);
	/* call original function */
	EGLBoolean ret = eglBindTexImagep(dpy, surface, buffer);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "ppd", voidp_to_uint64(dpy), voidp_to_uint64(surface), buffer);
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglReleaseTexImage, EGLDisplay dpy, EGLSurface surface,
//	    EGLint buffer)
HANDLER_WRAPPERS(EGLBoolean, eglReleaseTexImage, EGLDisplay, dpy,
		 EGLSurface, surface, EGLint, buffer)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLSurface surface,
					 EGLint buffer);
	/* probe prepare */
	BEFORE_EGL(eglReleaseTexImage);
	/* call original function */
	EGLBoolean ret = eglReleaseTexImagep(dpy, surface, buffer);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "ppd", voidp_to_uint64(dpy), voidp_to_uint64(surface), buffer);
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglSwapInterval, EGLDisplay dpy, EGLint interval)
HANDLER_WRAPPERS(EGLBoolean, eglSwapInterval, EGLDisplay, dpy, EGLint, interval)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLint interval);
	/* probe prepare */
	BEFORE_EGL(eglSwapInterval);
	/* call original function */
	EGLBoolean ret = eglSwapIntervalp(dpy, interval);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "pd", voidp_to_uint64(dpy), interval);
	return ret;
}

//HANDLER_DEF(EGLContext, eglCreateContext, EGLDisplay dpy, EGLConfig config,
//	    EGLContext share_context, const EGLint *attrib_list)
HANDLER_WRAPPERS(EGLContext, eglCreateContext, EGLDisplay, dpy,
		 EGLConfig, config, EGLContext, share_context,
		 const EGLint *, attrib_list)
{
	typedef EGLContext (*methodType)(EGLDisplay dpy, EGLConfig config,
					 EGLContext share_context,
					 const EGLint *attrib_list);
	/* probe prepare */
	BEFORE_EGL(eglCreateContext);
	/* call original function */
	EGLContext ret = eglCreateContextp(dpy, config, share_context,
					   attrib_list);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */

	AFTER('p', voidp_to_uint64(ret), APITYPE_CONTEXT, call_type, caller, "",
	      "pppp", voidp_to_uint64(dpy), voidp_to_uint64(config),
	      voidp_to_uint64(share_context), voidp_to_uint64(attrib_list));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglDestroyContext, EGLDisplay dpy, EGLContext ctx)
HANDLER_WRAPPERS(EGLBoolean, eglDestroyContext, EGLDisplay, dpy,
		 EGLContext, ctx)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLContext ctx);
	/* probe prepare */
	BEFORE_EGL(eglDestroyContext);
	/* call original function */
	EGLBoolean ret = eglDestroyContextp(dpy, ctx);
	/* get error code */
	EGL_GET_ERROR()

	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "pp", voidp_to_uint64(dpy), voidp_to_uint64(ctx));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglMakeCurrent, EGLDisplay dpy, EGLSurface draw,
//	    EGLSurface read, EGLContext ctx)
HANDLER_WRAPPERS(EGLBoolean, eglMakeCurrent, EGLDisplay, dpy, EGLSurface, draw,
		 EGLSurface, read, EGLContext, ctx)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLSurface draw,
					 EGLSurface read, EGLContext ctx);
	/* probe prepare */
	BEFORE_EGL(eglMakeCurrent);
	/* call original function */
	EGLBoolean ret = eglMakeCurrentp(dpy, draw, read, ctx);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "pppp", voidp_to_uint64(dpy), voidp_to_uint64(draw),
	      voidp_to_uint64(read), voidp_to_uint64(ctx));
	return ret;
}

//HANDLER_DEF(EGLContext, eglGetCurrentContext, void)
HANDLER_WRAPPERS(EGLContext, eglGetCurrentContext, void)
{
	typedef EGLContext (*methodType)(void);
	/* probe prepare */
	BEFORE_EGL(eglGetCurrentContext);
	/* call original function */
	EGLContext ret = eglGetCurrentContextp();

	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER_NO_PARAM('p', voidp_to_uint64(ret), APITYPE_CONTEXT, call_type, caller, "");
	return ret;
}

//HANDLER_DEF(EGLSurface, eglGetCurrentSurface, EGLint readdraw)
HANDLER_WRAPPERS(EGLSurface, eglGetCurrentSurface, EGLint, readdraw)
{
	typedef EGLSurface (*methodType)(EGLint readdraw);
	/* probe prepare */
	BEFORE_EGL(eglGetCurrentSurface);
	/* call original function */
	EGLSurface ret = eglGetCurrentSurfacep(readdraw);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('p', voidp_to_uint64(ret), APITYPE_CONTEXT, call_type, caller, "",
	      "d", readdraw);
	return ret;
}

//HANDLER_DEF(EGLDisplay, eglGetCurrentDisplay, void)
HANDLER_WRAPPERS(EGLDisplay, eglGetCurrentDisplay, void)
{
	typedef EGLDisplay (*methodType)(void);
	/* probe prepare */
	BEFORE_EGL(eglGetCurrentDisplay);
	/* call original function */
	EGLDisplay ret = eglGetCurrentDisplayp();
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER_NO_PARAM('p', voidp_to_uint64(ret), APITYPE_CONTEXT, call_type, caller, "");
	return ret;
}

//HANLDER_DEF(EGLBoolean, eglQueryContext, EGLDisplay dpy, EGLContext ctx,
//	    EGLint attribute, EGLint *value)
HANLDER_WRAPPERS(EGLBoolean, eglQueryContext, EGLDisplay, dpy, EGLContext, ctx,
		 EGLint, attribute, EGLint *, value)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLContext ctx,
					 EGLint attribute, EGLint *value);
	/* probe prepare */
	BEFORE_EGL(eglQueryContext);
	/* call original function */
	EGLBoolean ret = eglQueryContextp(dpy, ctx, attribute, value);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "ppdp", voidp_to_uint64(dpy), voidp_to_uint64(ctx),
	      attribute, voidp_to_uint64(value));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglWaitGL, void)
HANDLER_WRAPPERS(EGLBoolean, eglWaitGL, void)
{
	typedef EGLBoolean (*methodType)(void);
	/* probe prepare */
	BEFORE_EGL(eglWaitGL);
	/* call original function */
	EGLBoolean ret = eglWaitGLp();
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER_NO_PARAM('d', ret, APITYPE_CONTEXT, call_type, caller, "");
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglWaitNative, EGLint engine)
HANDLER_WRAPPERS(EGLBoolean, eglWaitNative, EGLint, engine)
{
	typedef EGLBoolean (*methodType)(EGLint engine);
	/* probe prepare */
	BEFORE_EGL(eglWaitNative);
	/* call original function */
	EGLBoolean ret = eglWaitNativep(engine);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "d", engine);
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglSwapBuffers, EGLDisplay dpy, EGLSurface surface)
HANDLER_WRAPPERS(EGLBoolean, eglSwapBuffers, EGLDisplay, dpy,
		 EGLSurface, surface)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLSurface surface);
	BEFORE_EGL(eglSwapBuffers);
	EGLBoolean ret = eglSwapBuffersp(dpy, surface);
	EGL_GET_ERROR();
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "pp", voidp_to_uint64(dpy), voidp_to_uint64(surface));
	return ret;
}

//HANDLER_DEF(EGLBoolean, eglCopyBuffers, EGLDisplay dpy, EGLSurface surface,
//	    EGLNativePixmapType target)
HANDLER_WRAPPERS(EGLBoolean, eglCopyBuffers, EGLDisplay, dpy,
		 EGLSurface, surface, EGLNativePixmapType, target)
{
	typedef EGLBoolean (*methodType)(EGLDisplay dpy, EGLSurface surface,
					 EGLNativePixmapType target);
	/* probe prepare */
	BEFORE_EGL(eglCopyBuffers);
	/* call original function */
	EGLBoolean ret = eglCopyBuffersp(dpy, surface, target);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('d', ret, APITYPE_CONTEXT, call_type, caller, "",
	      "ppp", voidp_to_uint64(dpy), voidp_to_uint64(surface),
	      voidp_to_uint64(target));
	return ret;
}

#define __eglGetProcAddress_t __eglMustCastToProperFunctionPointerType
//HANDLER_DEF(EGLAPI __eglGetProcAddress_t, eglGetProcAddress,
//	    const char *procname)
HANDLER_WRAPPERS(EGLAPI __eglGetProcAddress_t, eglGetProcAddress,
		 const char *, procname)
{
	typedef EGLAPI __eglGetProcAddress_t(*methodType)(const char *procname);
	/* probe prepare */
	BEFORE_EGL(eglGetProcAddress);
	/* call original function */
	__eglGetProcAddress_t ret = eglGetProcAddressp(procname);
	/* get error code */
	EGL_GET_ERROR();
	/* pack and send */
	AFTER('p', voidp_to_uint64((void *)ret), APITYPE_CONTEXT, call_type, caller, "",
	      "s", procname);
	return ret;
}
