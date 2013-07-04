/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 *
 * Woojin Jung <woojin2.jung@samsung.com>
 * Jaewon Lim <jaewon81.lim@samsung.com>
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

#ifndef __DA_THREAD_H__
#define __DA_THREAD_H__

#include "daprobe.h"

#define BEFORE_ORIGINAL_THREAD(FUNCNAME, LIBNAME) 	\
	DECLARE_VARIABLE_STANDARD;						\
	GET_REAL_FUNC(FUNCNAME, LIBNAME);				\
	PRE_PROBEBLOCK()

#define AFTER_PACK_ORIGINAL_THREAD(RVAL, THREADVAL, APITYPE, INPUTFORMAT, ...)			\
	POST_PACK_PROBEBLOCK_BEGIN();				\
	PREPARE_LOCAL_BUF();																\
	PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, INPUTFORMAT, __VA_ARGS__);			\
	PACK_COMMON_END(RVAL, errno, blockresult);											\
	PACK_THREAD(THREADVAL, THREAD_PTHREAD, APITYPE);									\
	FLUSH_LOCAL_BUF();																	\
	POST_PACK_PROBEBLOCK_END()

#endif // __DA_THREAD_H__

