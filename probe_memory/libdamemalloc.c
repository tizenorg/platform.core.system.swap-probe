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
#include "dacollection.h"
#include "dautil.h"
#include "da_memory.h"
#include "binproto.h"

#define EXTRA_MEM_SIZE		20
//#define INTERNALFILTERING		(!isEnableInternalMalloc())
#define INTERNALFILTERING		true
static char extra_mem[EXTRA_MEM_SIZE];
static enum DaOptions _sopt = OPT_ALLOC;

void *malloc(size_t size)
{
	static void*(*mallocp)(size_t size);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	GET_REAL_FUNC_RTLD_NEXT(malloc);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	pret = mallocp(size);

	if(pret != NULL && getTraceState() == 0)
	{
		add_memory_hash(pret, size);
	}

	POST_PACK_PROBEBLOCK_BEGIN();
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY, LC_MEMORY, "x", size);
	PACK_COMMON_END(pret, newerrno, blockresult);
	PACK_MEMORY(size, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();
	
	POST_PACK_PROBEBLOCK_END();

	return pret;
}

void free(void *ptr)
{
	static void (*freep)(void *);
	DECLARE_VARIABLE_STANDARD;

	GET_REAL_FUNC_RTLD_NEXT(free);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	if(ptr != NULL && getTraceState() == 0)
	{
		del_memory_hash(ptr);
	}

	freep(ptr);

	POST_PACK_PROBEBLOCK_BEGIN();
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY, LC_MEMORY, "p", ptr);
	PACK_COMMON_END(0, newerrno, blockresult);
	PACK_MEMORY(0, MEMORY_API_FREE, ptr);
	FLUSH_LOCAL_BUF();
	
	POST_PACK_PROBEBLOCK_END();
}

/*	calloc's helper fucntion
 * 	- dlsym calls calloc - recursion occurs
 * 	- first dlsym calls temp_calloc
 *  */
void *temp_calloc(size_t nelem, size_t elsize)
{
	int i;
	if(nelem * elsize > EXTRA_MEM_SIZE)
	{
		// temp_calloc size error
		abort();
	}
	for(i = 0; i < elsize * nelem; i++)
	{
		extra_mem[i] = 0;
	}
	//memset(extra_mem,0,elsize*nelem);
	return extra_mem;
}

void *calloc(size_t nelem, size_t elsize)
{
	static void *(*callocp)(size_t,size_t);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	if(!callocp) {
		probeBlockStart();
		callocp = temp_calloc;	// make callocp is not null for dlsym
		// dlsym use calloc function
		callocp = dlsym(RTLD_NEXT, "calloc");
		if(callocp == NULL || dlerror() != NULL) {
			perror("calloc dlsym failed");
			exit(0);
		}
		probeBlockEnd();
	}

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	pret = callocp(nelem, elsize);

	if(pret != NULL && getTraceState() == 0)
	{
		add_memory_hash(pret, nelem * elsize);
	}

	POST_PACK_PROBEBLOCK_BEGIN();
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY, LC_MEMORY, "xx", nelem, elsize);
	PACK_COMMON_END(pret, newerrno, blockresult);
	PACK_MEMORY(elsize, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();
	
	POST_PACK_PROBEBLOCK_END();

	return pret;
}

void *realloc(void *memblock, size_t size)
{
	static void *(*reallocp)(void *,size_t);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	GET_REAL_FUNC_RTLD_NEXT(realloc);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	if(memblock != NULL && getTraceState() == 0)
	{
		del_memory_hash(memblock);
	}

	pret = reallocp(memblock, size);

	if(pret != NULL && getTraceState() == 0)
	{
		add_memory_hash(pret, size);
	}

	POST_PACK_PROBEBLOCK_BEGIN();
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY, LC_MEMORY, "px", memblock, size);
	PACK_COMMON_END(pret, newerrno, blockresult);
	PACK_MEMORY(size, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();
	
	POST_PACK_PROBEBLOCK_END();

	return pret;
}

