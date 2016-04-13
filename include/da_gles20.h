/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Sanghyun Lee <sanghyunnim.lee@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
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
 *
 */

#ifndef DA_GLES20_H_
#define DA_GLES20_H_

/*
 * TODO find this headers. need for EGL_Context and other
#include <egl.h>
#include <eglext.h>
#include <gl2.h>
#include <gl2ext.h>
*/

#include <Evas_GL.h>
#include <Evas.h>

#include <errno.h>
#include "daprobe.h"
#include "dahelper.h"

/* TODO add egl header for next */
#define EGLContext void *
extern EGLContext eglGetCurrentContext(void);

#define NO_RETURN_FORMAT "%s"
#define NO_RETURN_VALUE 0
#define APITYPE_INIT 0
#define APITYPE_CONTEXT 1
#define APITYPE_NO_CONTEXT 2

#define FUNC(FUNCNAME) FUNCNAME
#define FUNCSTR(FUNCNAME) #FUNCNAME
#define FUNCID(FUNCNAME) API_ID_##FUNCNAME

#define PACK_GL_ADD_COMMON(GL_api_type, GL_elapsed_time)			\
	do {	/* PACK_GL_ADD_COMMON */					\
		/* TODO restore eglGetCurrentContext */				\
		/*BUF_PTR = pack_int64(BUF_PTR, voidp_to_uint64((void *)eglGetCurrentContext()));*/\
		BUF_PTR = pack_int64(BUF_PTR, voidp_to_uint64((void *)0));\
		BUF_PTR = pack_int32(BUF_PTR, (uint32_t)GL_api_type);	\
		BUF_PTR = pack_int64(BUF_PTR, (uint64_t)GL_elapsed_time); \
	} while (0)

#define PACK_GL_ADD(GL_api_type, GL_elapsed_time, GL_context_value)		\
	do {	/* PACK_GL_ADD */						\
		PACK_GL_ADD_COMMON(GL_api_type, GL_elapsed_time);		\
		BUF_PTR = pack_string(BUF_PTR, GL_context_value);	\
	} while (0)

#define PACK_GL_SHADER(GL_api_type, GL_elapsed_time, GL_shader, GL_shader_size)	\
	do {	/* PACK_GL_SHADER */						\
		PACK_GL_ADD_COMMON(GL_api_type, GL_elapsed_time);		\
		uint32_t min = (sizeof(LOCAL_BUF) - (BUF_PTR - LOCAL_BUF));	\
		if (min > MAX_SHADER_LEN)					\
			min = MAX_SHADER_LEN;					\
		BUF_PTR = pack_string_to_file(BUF_PTR, GL_shader,		\
					      GL_shader_size, min);		\
	} while (0)

#if 0 /* TODO Support old preload */

#define BEFORE_GL_ORIG(FUNCNAME, LIB_NAME)				\
	DECLARE_VARIABLE_STANDARD_NORET;				\
	GLenum error = GL_NO_ERROR;					\
	static methodType FUNCNAME ## p = 0;				\
	int32_t vAPI_ID = API_ID_ ## FUNCNAME;				\
	uint64_t start_nsec = 0;					\
	PRE_PROBEBLOCK();						\
	if(blockresult != 0)						\
		start_nsec = get_current_nsec();			\
	if(!FUNCNAME##p) {						\
		init_probe_gl(#FUNCNAME, (void **)&FUNCNAME##p,		\
			      LIB_NAME, blockresult, vAPI_ID);		\
	}

#else

#define BEFORE_GL_ORIG(FUNCNAME, LIB_NAME)				\
	DECLARE_VARIABLE_STANDARD_NORET;				\
	GLenum error = GL_NO_ERROR;					\
	methodType FUNCNAME ## p = (methodType) orig;			\
	int32_t vAPI_ID = API_ID_ ## FUNCNAME;				\
	uint64_t start_nsec = 0;					\
	PRE_PROBEBLOCK();						\
	if(blockresult != 0)						\
		start_nsec = get_current_nsec();			\

#endif

#define BEFORE_GL2_ORIG(FUNCNAME) /* BEFORE_GL2_ORIG */ BEFORE_GL_ORIG(FUNCNAME, LIBGLES20)

#define BEFORE_GL3_ORIG(FUNCNAME) /* BEFORE_GL3_ORIG */ BEFORE_GL_ORIG(FUNCNAME, LIBGLES30)

#define BEFORE_GL_API(FUNCNAME, LIB_NAME)				\
	DECLARE_VARIABLE_STANDARD_NORET;				\
	GLenum error = GL_NO_ERROR;					\
	int32_t vAPI_ID = API_ID_ ## FUNCNAME;				\
	uint64_t start_nsec = 0;					\
	PRE_PROBEBLOCK();						\
	if(blockresult != 0)						\
		start_nsec = get_current_nsec();			\
	if(!__gl_api->FUNCNAME) {						\
		probe_terminate_with_err("api not initialized",		\
					 #FUNCNAME, LIB_NAME);		\
	}

#define BEFORE_GL2_API(FUNCNAME) BEFORE_GL_API(FUNCNAME, LIBGLES20)

#define BEFORE_GL3_API(FUNCNAME) BEFORE_GL_API(FUNCNAME, LIBGLES30)

#define INIT_LIB_ID_STR(LIB_ID, LIB_STR, KEYS)							\
		if (lib_handle[LIB_ID] == ((void *) 0)) {		\
			lib_handle[LIB_ID] = dlopen(LIB_STR, RTLD_LAZY | RTLD_GLOBAL); \
			if (lib_handle[LIB_ID] == ((void *) 0)) {	\
				char perror_msg[128];			\
				sprintf(perror_msg, "dlopen failed : [%s],%s", \
					__FUNCTION__, LIB_STR);			\
				perror(perror_msg);				\
				exit(0);				\
			}						\
		}

#define INIT_LIB(LIB_ID, KEYS)						\
	INIT_LIB_ID_STR(LIB_ID, lib_string[LIB_ID], KEYS)

#define BEFORE_EGL_NATIVE(FUNCNAME)					\
	DECLARE_VARIABLE_STANDARD_NORET;				\
	GLenum error = EGL_SUCCESS;					\
	static methodType FUNCNAME ## p = 0;				\
	int32_t vAPI_ID = API_ID_ ## FUNCNAME;				\
	uint64_t start_nsec = 0;					\
	PRE_PROBEBLOCK();						\
	if(blockresult != 0)						\
		start_nsec = get_current_nsec();			\
	if(!FUNCNAME##p) 						\
		init_probe_egl(#FUNCNAME, (void **)&FUNCNAME##p, LIBEGL)

#define EGL_GET_ERROR()							\
	if (blockresult != 0) {						\
		error = eglGetError();					\
	}

#define AFTER(RET_TYPE, RET_VAL, APITYPE, CONTEXT_VAL, INPUTFORMAT, ...)	\
	/* AFTER */								\
	POST_PACK_PROBEBLOCK_BEGIN();						\
	PREPARE_LOCAL_BUF();							\
	PACK_COMMON_BEGIN(MSG_PROBE_GL, vAPI_ID, INPUTFORMAT, __VA_ARGS__);	\
	PACK_COMMON_END(RET_TYPE, RET_VAL, error, call_type, caller);		\
	PACK_GL_ADD(APITYPE, get_current_nsec() - start_nsec, CONTEXT_VAL);	\
	FLUSH_LOCAL_BUF();							\
	POST_PACK_PROBEBLOCK_END()

#define AFTER_NO_PARAM(RET_TYPE, RETVAL, APITYPE, CONTEXTVALUE) \
		AFTER(RET_TYPE, RETVAL, APITYPE, CONTEXTVALUE, "", 0)

#define GL_GET_ERROR()							\
	/* GL_GET_ERROR */						\
	if (blockresult != 0) {						\
		is_gl_error_external = 0;				\
		error = glGetError_internal();				\
		is_gl_error_external = 1;				\
	}

#define AFTER_SHADER(RET_TYPE, RET_VAL, APITYPE, CONTEXT_VAL,	       \
		     CONTEXT_SIZE, INPUTFORMAT, ...)	\
	POST_PACK_PROBEBLOCK_BEGIN();						\
	PREPARE_LOCAL_BUF();							\
	PACK_COMMON_BEGIN(MSG_PROBE_GL, vAPI_ID, INPUTFORMAT, __VA_ARGS__);	\
	PACK_COMMON_END(RET_TYPE, RET_VAL, error, call_type, caller);		\
	PACK_GL_SHADER(APITYPE, get_current_nsec() - start_nsec, CONTEXT_VAL, CONTEXT_SIZE);	\
	FLUSH_LOCAL_BUF();							\
	POST_PACK_PROBEBLOCK_END()

#define _ALIAS(NEW_FUNC, FUNCNAME, TYPE, ...)					\
	TYPE NEW_FUNC(__VA_ARGS__) __attribute__ ((				\
						   alias(FUNCSTR(FUNCNAME))	\
						 ))


/* NOTIFY: uncomment to use with LD_PRELOAD */
#if 0

#define PUT_ALIASES(TYPE, FUNCNAME, ...)			    \
	/* alias for C function prototype */					\
	extern _ALIAS(FUNCNAME, REAL_NAME(FUNCNAME), TYPE);			\
	}/* extern C*/								\
	\
	/* alias for C++ function prototype */					\
	extern _ALIAS(FUNCNAME, REAL_NAME(FUNCNAME), TYPE, __VA_ARGS__);	\
	extern "C"								\
	{

#else

#define PUT_ALIASES(TYPE, FUNCNAME, ...)

#endif



#define FUNC_DECLAR(TYPE, FUNCNAME, ...)					\
	PUT_ALIASES(TYPE, FUNCNAME, __VA_ARGS__)			\
	HANDLER_WRAPPERS(graphics_feature, TYPE, FUNCNAME, __VA_ARGS__)

#define FUNC_DECLAR_NOARGS(TYPE, FUNCNAME)					\
	PUT_ALIASES(TYPE, FUNCNAME)							\
	HANDLER_WRAPPERS(graphics_feature, TYPE, FUNCNAME)


#if 0 /* TODO Support old preload */

#define BEFORE_EVAS_GL(FUNCNAME)					\
	DECLARE_VARIABLE_STANDARD_NORET;				\
	GLenum error = GL_NO_ERROR;					\
	static methodType FUNCNAME ## p = 0;				\
	int32_t vAPI_ID = API_ID_ ## FUNCNAME;				\
	uint64_t start_nsec = 0;					\
	PRE_PROBEBLOCK();						\
	if(blockresult != 0)						\
		start_nsec = get_current_nsec();			\
	GET_REAL_FUNCP_RTLD_DEFAULT(FUNCNAME, FUNCNAME##p)

#else

#define BEFORE_EVAS_GL(FUNCNAME)					\
	DECLARE_VARIABLE_STANDARD_NORET;				\
	GLenum error = GL_NO_ERROR;					\
	methodType FUNCNAME ## p = 0;				\
	int32_t vAPI_ID = API_ID_ ## FUNCNAME;				\
	uint64_t start_nsec = 0;					\
	PRE_PROBEBLOCK();						\
	if(blockresult != 0)						\
		start_nsec = get_current_nsec();			\
	FUNCNAME ## p = (void *)orig;

#endif

/* FIXME fixed arguments num. increase if neccessary */
#define PP_RSEQ_N() 10,9,8,7,6,5,4,3,2,1,0
#define PP_ARG_N( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N,...) N
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define GET_ARGS_NUM(...) PP_NARG_(__VA_ARGS__, PP_RSEQ_N())

#define GET_FIRST_2(a, b) a
#define GET_FIRST_4(a, b, ...) a, GET_FIRST_2(__VA_ARGS__)
#define GET_FIRST_6(a, b, ...) a, GET_FIRST_4(__VA_ARGS__)
#define GET_FIRST_8(a, b, ...) a, GET_FIRST_6(__VA_ARGS__)

#define GET_ALL_2(a, b) a b
#define GET_ALL_4(a, b, ...) a b, GET_ALL_2(__VA_ARGS__)
#define GET_ALL_6(a, b, ...) a b, GET_ALL_4(__VA_ARGS__)
#define GET_ALL_8(a, b, ...) a b, GET_ALL_6(__VA_ARGS__)


#define NUM(...) GET_ARGS_NUM(__VA_ARGS__)

#define GET2(a,b) a ## b
#define GET1(a,b) GET2(a,b)
#define GET0(a,b) GET1(a,b)
#define GET_FIRST_(...)  GET0(GET_FIRST_, NUM(__VA_ARGS__))


#define GET_ARGS(type1, ...) GET_FIRST_(type1, __VA_ARGS__)(__VA_ARGS__, type1)
#define GET_TYPES(...) GET_FIRST_(__VA_ARGS__)(__VA_ARGS__)

#define DECLARE_GL_DEFAULT_VOID(TYPE, FUNCNAME, PACK_ARGS, ...)			\
	DECLARE(TYPE, FUNCNAME, __VA_ARGS__)		\
	{									\
		TYPEDEF(void (*methodType)(GET_TYPES(__VA_ARGS__)));		\
		BEFORE(FUNCNAME);						\
		CALL_ORIG(FUNCNAME, GET_ARGS(__VA_ARGS__));			\
		GL_GET_ERROR();							\
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT,		      \
		      "", PACK_ARGS, GET_ARGS(__VA_ARGS__));					\
	}


extern Evas_GL_API *__gl_api;
extern Evas_GL_API *get_gl_api_fake_list(Evas_GL_API *api);
#endif /* DA_GLES20_H_ */

