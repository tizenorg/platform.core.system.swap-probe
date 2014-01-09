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

#include <egl.h>
#include <eglext.h>
#include <gl2.h>
#include <gl2ext.h>
#include <errno.h>
#include "daprobe.h"
#include "dahelper.h"

#define NO_RETURN_FORMAT "%s"
#define NO_RETURN_VALUE 0
#define APITYPE_INIT 0
#define APITYPE_CONTEXT 1
#define APITYPE_NO_CONTEXT 2

#define PACK_GL_ADD(GL_api_type, GL_elapsed_time, GL_context_value)		\
	do {	/* PACK_GL_ADD */						\
		BUF_PTR = pack_int32(BUF_PTR, (uint32_t)GL_api_type);	\
		BUF_PTR = pack_int64(BUF_PTR, (uint64_t)GL_elapsed_time); \
		BUF_PTR = pack_string(BUF_PTR, GL_context_value);	\
	} while (0)


#define BEFORE(FUNCNAME)						\
	DECLARE_VARIABLE_STANDARD_NORET;				\
	GLenum error = GL_NO_ERROR;					\
	static methodType FUNCNAME ## p = 0;				\
	void* tmpPtr = 0;						\
	int32_t vAPI_ID = API_ID_ ## FUNCNAME;				\
	uint64_t start_nsec = get_current_nsec();			\
	if(!FUNCNAME##p) {						\
		probeBlockStart();					\
		if (lib_handle[LIBGLES20] == ((void *) 0)) {		\
			lib_handle[LIBGLES20] = dlopen(lib_string[LIBGLES20], RTLD_LAZY); \
			if (lib_handle[LIBGLES20] == ((void *) 0)) {    \
				char perror_msg[128];			\
				sprintf(perror_msg, "dlopen failed : %s", \
					lib_string[LIBGLES20]);		\
				perror(perror_msg);			\
				exit(0);				\
			}						\
			probeInfo_t tempProbeInfo;			\
			setProbePoint(&tempProbeInfo);			\
			/* get max value */				\
			char maxValString[64];				\
			GLint maxVal[2];				\
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVal[0]); 		\
			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxVal[1]);	\
			sprintf(maxValString, "%d,%d", maxVal[0], maxVal[1]);		\
			PREPARE_LOCAL_BUF();						\
			PACK_COMMON_BEGIN(MSG_PROBE_GL, vAPI_ID, "", 0);		\
			PACK_COMMON_END('p', 1, 0, 0);					\
			PACK_GL_ADD(APITYPE_INIT, 0, maxValString);			\
			FLUSH_LOCAL_BUF();						\
		}							\
									\
		tmpPtr = dlsym(lib_handle[LIBGLES20], #FUNCNAME);	\
		if (tmpPtr == NULL || dlerror() != NULL) {		\
			perror("dlsym failed : " #FUNCNAME);		\
			exit(0);					\
		}							\
									\
		memcpy(&FUNCNAME##p, &tmpPtr, sizeof(tmpPtr));		\
		probeBlockEnd();					\
	}								\
	PRE_PROBEBLOCK()

#define INIT_LIB_ID_STR(LIB_ID, LIB_STR, KEYS)							\
		if (lib_handle[LIB_ID] == ((void *) 0)) {		\
			lib_handle[LIB_ID] = dlopen(LIB_STR, RTLD_LAZY | RTLD_GLOBAL); \
			if (lib_handle[LIB_ID] == ((void *) 0)) {       \
				char perror_msg[128];			\
				sprintf(perror_msg, "dlopen failed : [%s],%s", \
					__FUNCTION__, LIB_STR);			\
				perror(perror_msg);				\
				exit(0);				\
			}						\
		}

#define INIT_LIB(LIB_ID, KEYS)						\
	INIT_LIB_ID_STR(LIB_ID, lib_string[LIB_ID], KEYS)

#define BEFORE_EGL(FUNCNAME)					\
	DECLARE_VARIABLE_STANDARD_NORET;				\
	GLenum error = GL_NO_ERROR;					\
	static methodType FUNCNAME ## p = 0;				\
	void* tmpPtr = 0;						\
	int32_t vAPI_ID = API_ID_ ## FUNCNAME;				\
	uint64_t start_nsec = get_current_nsec();			\
	if(!FUNCNAME##p) {						\
		probeBlockStart();					\
		INIT_LIB(LIBEGL, RTLD_LAZY | RTLD_GLOBAL);		\
		tmpPtr = dlsym(lib_handle[LIBEGL], #FUNCNAME);		\
		if (tmpPtr == NULL || dlerror() != NULL) {		\
			perror("dlsym failed : " #FUNCNAME);		\
			exit(0);					\
		}							\
		memcpy(&FUNCNAME##p, &tmpPtr, sizeof(tmpPtr));		\
		probeBlockEnd();					\
	}								\
	PRE_PROBEBLOCK()

#define BEFORE_OSP_UIFW(FUNCNAME)					\
	DECLARE_VARIABLE_STANDARD_NORET;			\
	GLenum error = GL_NO_ERROR;				\
	static methodType FUNCNAME ## p = 0;			\
	void* tmpPtr = 0;					\
	int32_t vAPI_ID = API_ID_ ## FUNCNAME;			\
	uint64_t start_nsec = get_current_nsec();				\
	if(!FUNCNAME##p) {						\
		probeBlockStart();					\
		INIT_LIB(LIBOSP_UIFW, RTLD_LAZY);			\
		tmpPtr = dlsym(lib_handle[LIBOSP_UIFW], #FUNCNAME);     \
		if (tmpPtr == NULL || dlerror() != NULL) {		\
			perror("dlsym failed : " #FUNCNAME);		\
			exit(0);					\
		}							\
		memcpy(&FUNCNAME##p, &tmpPtr, sizeof(tmpPtr));		\
		probeBlockEnd();					\
	}								\
	PRE_PROBEBLOCK()

#define AFTER(RET_TYPE, RET_VAL, APITYPE, CONTEXT_VAL, INPUTFORMAT, ...)	\
	POST_PACK_PROBEBLOCK_BEGIN();						\
	PREPARE_LOCAL_BUF();							\
	PACK_COMMON_BEGIN(MSG_PROBE_GL, vAPI_ID, INPUTFORMAT, __VA_ARGS__);	\
	PACK_COMMON_END(RET_TYPE, RET_VAL, error, blockresult);			\
	PACK_GL_ADD(APITYPE, get_current_nsec() - start_nsec, CONTEXT_VAL);	\
	FLUSH_LOCAL_BUF();							\
	POST_PACK_PROBEBLOCK_END()

#define AFTER_NO_PARAM(RET_TYPE, RETVAL, APITYPE, CONTEXTVALUE) \
		AFTER(RET_TYPE, RETVAL, APITYPE, CONTEXTVALUE, "", 0)

#define GL_GET_ERROR()							\
	if (blockresult != 0) {						\
		error = glGetError();					\
	}

#endif /* DA_GLES20_H_ */

