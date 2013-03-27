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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <memory.h>
#include <errno.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dautil.h"
#include "da_memory.h"

void *memset(void *memblock, int c, size_t n)
{
	static void *(*memsetp)(void *,int,size_t);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	GET_REAL_FUNC_RTLD_NEXT(memset);

	PRE_PROBEBLOCK();

	pret = memsetp(memblock, c, n);

	POST_PROBEBLOCK_BEGIN(LC_MEMORY, VT_PTR, pret, "%p, %d, %u", memblock, c, n);
	POST_PROBEBLOCK_MIDDLE_MEM(n, MEMORY_API_MANAGE, memblock);
	POST_PROBEBLOCK_CALLSTACK();
	POST_PROBEBLOCK_END();

	return pret;
}

int memcmp(const void * ptr1, const void * ptr2, size_t num)
{
	static int(*memcmpp)(const void *,const void *,size_t);
	DECLARE_VARIABLE_STANDARD;

	GET_REAL_FUNC_RTLD_NEXT(memcmp);

	PRE_PROBEBLOCK();

	ret = memcmpp(ptr1, ptr2, num);

	POST_PROBEBLOCK_BEGIN(LC_MEMORY, VT_INT, ret, "%p, %p, %u", ptr1, ptr2, num);
	POST_PROBEBLOCK_MIDDLE_MEM(num, MEMORY_API_MANAGE, ptr1);
	POST_PROBEBLOCK_CALLSTACK();
	POST_PROBEBLOCK_END();

	return ret;
}

void *memcpy(void * destination, const void * source, size_t num )
{
	static void *(*memcpyp)(void *,const void *,size_t);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	GET_REAL_FUNC_RTLD_NEXT(memcpy);

	PRE_PROBEBLOCK();

	pret = memcpyp(destination, source, num);

	POST_PROBEBLOCK_BEGIN(LC_MEMORY, VT_PTR, pret, "%p, %p, %u", destination, source, num);
	POST_PROBEBLOCK_MIDDLE_MEM(num, MEMORY_API_MANAGE, destination);
	POST_PROBEBLOCK_CALLSTACK();
	POST_PROBEBLOCK_END();

	return pret;
}

