/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
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
 * - Samsung RnD Institute Russia
 *
 */

//disable tizen redefines
#define _GL2_MACRO_H_
#define _GL_MACRO_H_

extern "C" {
/*
 * this include to make C native open gl functions
 * probe prototypes
 *
 */

#include "da_gles20_tizen.cpp"

} /* extern C */

#undef eglSwapBuffers
extern "C" EGLBoolean eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
	typedef EGLBoolean (*methodType)(EGLDisplay, EGLSurface);
	BEFORE_EGL(eglSwapBuffers);
	EGLBoolean ret = eglSwapBuffersp(dpy, surface);
	error = eglGetError();
	AFTER_NO_PARAM('d', ret, APITYPE_CONTEXT, "");

	return ret;
}
#define eglSwapBuffers _SglSwapBuffers

EGLBoolean _SglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
	typedef EGLBoolean (*methodType)(EGLDisplay, EGLSurface);
	BEFORE_OSP_UIFW(_SglSwapBuffers);
	EGLBoolean ret = _SglSwapBuffersp(dpy, surface);
	error = eglGetError();
	AFTER_NO_PARAM('d', ret, APITYPE_CONTEXT, "");

	return ret;
}

#undef eglGetProcAddress
extern "C" EGLAPI __eglMustCastToProperFunctionPointerType
eglGetProcAddress(const char* procname)
{
	typedef EGLAPI __eglMustCastToProperFunctionPointerType
		(*methodType)(const char* procname);
	__eglMustCastToProperFunctionPointerType ret = NULL;

	BEFORE_EGL(eglGetProcAddress);

	probeBlockStart();
	INIT_LIB(LIBSELF, RTLD_LAZY | RTLD_GLOBAL);
	ret = (typeof(ret)) dlsym(lib_handle[LIBSELF], procname);
	if (dlerror() != NULL)
		ret = NULL;
	probeBlockEnd();

	if (ret == NULL) {
		// we don't have <procname> function in our lib
		// so we need find original function
		ret = eglGetProcAddressp(procname);
		error = eglGetError();
	}

	AFTER_NO_PARAM('p', ret, APITYPE_CONTEXT, "");
	return ret;

}
#define eglGetProcAddress _eglGetProcAddress

#undef _GL2_MACRO_H_
#undef _GL_MACRO_H_
