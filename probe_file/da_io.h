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

#ifndef __DA_IO_H__
#define __DA_IO_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "daprobe.h"

/********************************************************************
 * RESOURCE PROBE MACRO
 *
 * log format
 * 		SeqNumber`,ApiName`,Time`,Pid`,Tid`,InputParm`,Return`,PCAddr`,Error`,
 *		Size`,FDValue`,FDType`,FDApiType`,\n
 *		callstack_start`,callstack`,callstack_end
 *
 * fd type
 *		FD_TYPE_FILE, FD_TYPE_SOCKET
 *
 * fd api type
 *		FD_API_TYPE_OPEN, FD_API_TYPE_CLOSE, FD_API_TYPE_READ, FD_API_TYPE_WRITE,
 *		FD_API_TYPE_DIRECTORY, FD_API_TYPE_PERMISSION, FD_API_TYPE_OTHER,
 *		FD_API_TYPE_SEND, FD_API_TYPE_RECEIVE, FD_API_TYPE_OPTION, FD_API_TYPE_MANAGE
 *
 ************************************************************************/

// ===================================================================
// macro for file probe
// ===================================================================

#define GET_FD_FROM_FILEP(FILEP)	\
	_fd = (FILEP != NULL) ? fileno(FILEP) : -1

#define DECLARE_VARIABLE_FD							\
		DECLARE_VARIABLE_STANDARD;					\
		char* _filepath = "";						\
		int __attribute((unused)) _fd;				\
		int __attribute((unused)) _fstatret = -1;	\
		struct stat __attribute((unused)) _statbuf

// =================================================================
// POST_PROBEBLOCK_MIDDLE for file
// =================================================================

#define POST_PACK_PROBEBLOCK_MIDDLE_FD(SIZE, FD, APITYPE)			\
	PACK_RESOURCE(SIZE, FD, FD_FILE, APITYPE,						\
		      (_fstatret == 0 ? _statbuf.st_size : 0), _filepath);	\
	FLUSH_LOCAL_BUF()

#define POST_PACK_PROBEBLOCK_MIDDLE_NOFD(SIZE, APITYPE)			\
	PACK_RESOURCE(SIZE, 0, FD_FILE, APITYPE, 0, _filepath);		\
	FLUSH_LOCAL_BUF()

// ==================================================================
// BEFORE_ORIGINAL macro for file
// ==================================================================

#define BEFORE_ORIGINAL_FILE(FUNCNAME, LIBNAME)	\
	DECLARE_VARIABLE_FD;						\
	GET_REAL_FUNC(FUNCNAME, LIBNAME);			\
	PRE_PROBEBLOCK()

#define BEFORE_ORIGINAL_FILE_NOFILTER(FUNCNAME, LIBNAME)	\
	DECLARE_VARIABLE_FD;									\
	GET_REAL_FUNC(FUNCNAME, LIBNAME);						\
	bfiltering = false;										\
	PRE_PROBEBLOCK()

// ==================================================================
// AFTER_ORIGINAL macro for file
// ==================================================================

#define AFTER_PACK_ORIGINAL_FD(RVAL, SIZE, FD, APITYPE, INPUTFORMAT, ...)			\
	POST_PACK_PROBEBLOCK_BEGIN();													\
	PREPARE_LOCAL_BUF();															\
	PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, INPUTFORMAT, __VA_ARGS__);	\
	PACK_COMMON_END(RVAL, newerrno, blockresult);									\
	_fstatret = fstat(FD, &_statbuf);												\
	POST_PACK_PROBEBLOCK_MIDDLE_FD(SIZE, FD, APITYPE);								\
	POST_PACK_PROBEBLOCK_END()

#define AFTER_PACK_ORIGINAL_NOFD(RVAL, SIZE, APITYPE, INPUTFORMAT, ...)					\
	POST_PACK_PROBEBLOCK_BEGIN();														\
	PREPARE_LOCAL_BUF();																\
	PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, INPUTFORMAT, __VA_ARGS__);		\
	PACK_COMMON_END(RVAL, newerrno, blockresult);										\
	POST_PACK_PROBEBLOCK_MIDDLE_NOFD(SIZE, APITYPE);									\
	POST_PACK_PROBEBLOCK_END()

#define AFTER_PACK_ORIGINAL_FILEP(RVAL, SIZE, FILEP, APITYPE, INPUTFORMAT, ...)			\
	POST_PACK_PROBEBLOCK_BEGIN();														\
	GET_FD_FROM_FILEP(FILEP);															\
	if(_fd != -1) { 																	\
		_fstatret = fstat(_fd, &_statbuf);												\
	}																					\
	PREPARE_LOCAL_BUF();																\
	PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE, LC_RESOURCE, INPUTFORMAT, __VA_ARGS__);		\
	PACK_COMMON_END(RVAL, newerrno, blockresult);										\
	POST_PACK_PROBEBLOCK_MIDDLE_FD(SIZE, _fd, APITYPE);									\
	POST_PACK_PROBEBLOCK_END()

#endif	// __DA_IO_H__
