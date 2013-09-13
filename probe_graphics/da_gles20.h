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

char contextValue[256];

#define PACK_GL_ADD(GL_api_type, GL_elapsed_time, GL_context_value)		\
	do {							\
		BUF_PTR = pack_int32(BUF_PTR, (uint32_t)GL_api_type);	\
		BUF_PTR = pack_int32(BUF_PTR, (uint32_t)GL_elapsed_time);	\
		BUF_PTR = pack_string(BUF_PTR, GL_context_value);	\
	} while (0)


#define BEFORE(FUNCNAME)	\
	DECLARE_VARIABLE_STANDARD_NORET;	\
	GLenum error = GL_NO_ERROR;	\
	static methodType FUNCNAME ## p = 0;	\
	void* tmpPtr = 0;	\
	int32_t vAPI_ID = API_ID_ ## FUNCNAME; \
	long startTime = getCurrentTime();	\
	PREPARE_LOCAL_BUF(); \
	GET_REAL_FUNCP_STR(#FUNCNAME, LIBGLES20, tmpPtr);	\
	memcpy(& FUNCNAME ## p, &tmpPtr, sizeof(tmpPtr));	\
	PRE_PROBEBLOCK()

#define AFTER(RET_VAL, APITYPE, CONTEXT_VAL, INPUTFORMAT, ...)	\
	POST_PACK_PROBEBLOCK_BEGIN();						\
	PREPARE_LOCAL_BUF();							\
	PACK_COMMON_BEGIN(MSG_PROBE_GL, vAPI_ID, INPUTFORMAT, __VA_ARGS__);\
	PACK_COMMON_END(RET_VAL, error, blockresult);					\
	PACK_GL_ADD(APITYPE, getCurrentTime()-startTime, CONTEXT_VAL);		\
	FLUSH_LOCAL_BUF();							\
	POST_PACK_PROBEBLOCK_END()

#define AFTER_NO_PARAM(RETVAL, APITYPE, CONTEXTVALUE) \
		AFTER(RETVAL, APITYPE, CONTEXTVALUE, "", 0)

#endif /* DA_GLES20_H_ */

