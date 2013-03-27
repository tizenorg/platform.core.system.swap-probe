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

#ifndef __DA_SOCKET_H__
#define __DA_SOCKET_H__

#include "daprobe.h"

#define POST_PROBEBLOCK_MIDDLE_SOCK(SIZE, FD, APITYPE, PATH)				\
	log.length += sprintf(log.data + log.length, "`,%u`,%d`,%d`,%d`,0`,%s",	\
			SIZE, FD, FD_SOCKET, APITYPE, PATH)
 
#define POST_PROBEBLOCK_MIDDLE_NOSOCK(SIZE, APITYPE, PATH)				\
	log.length += sprintf(log.data + log.length, "`,%u`,`,%d`,%d`,0`,%s",	\
			SIZE, FD_SOCKET, APITYPE, PATH)

#define POST_PROBEBLOCK_CALLSTACK_SOCKET()	\
	POST_PROBEBLOCK_CALLSTACK()

#define AFTER_ORIGINAL_SOCK_RET(RTYPE, RVAL, SIZE, FD, APITYPE, INPUTFORMAT, ...)	\
	POST_PROBEBLOCK_BEGIN(LC_RESOURCE, RTYPE, RVAL,	INPUTFORMAT, __VA_ARGS__);		\
	POST_PROBEBLOCK_MIDDLE_SOCK(SIZE, FD, APITYPE, "");								\
	POST_PROBEBLOCK_CALLSTACK_SOCKET();												\
	POST_PROBEBLOCK_END()

#define AFTER_ORIGINAL_NOSOCK_RET(RTYPE, RVAL, SIZE, APITYPE, INPUTFORMAT, ...)	\
	POST_PROBEBLOCK_BEGIN(LC_RESOURCE, RTYPE, RVAL,	INPUTFORMAT, __VA_ARGS__);	\
	POST_PROBEBLOCK_MIDDLE_NOSOCK(SIZE, APITYPE, "");							\
	POST_PROBEBLOCK_CALLSTACK_SOCKET();											\
	POST_PROBEBLOCK_END()

#define AFTER_ORIGINAL_SOCK(SIZE, FD, APITYPE, INPUTFORMAT, ...)	\
	AFTER_ORIGINAL_SOCK_RET(VT_INT, ret, SIZE, FD, APITYPE, INPUTFORMAT, __VA_ARGS__)

#define AFTER_ORIGINAL_NOSOCK(SIZE, APITYPE, INPUTFORMAT, ...)	\
	AFTER_ORIGINAL_NOSOCK_RET(VT_INT, ret, SIZE, APITYPE, INPUTFORMAT, __VA_ARGS__)
	
#endif // __DA_SOCKET_H__
