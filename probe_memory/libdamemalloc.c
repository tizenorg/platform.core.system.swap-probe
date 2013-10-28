/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 * Anastasia Lyupa <a.lyupa@samsung.com>
 * Dmitry Bogatov <d.bogatov@samsung.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <memory.h>
#include <errno.h>
#include <malloc.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dacollection.h"
#include "dautil.h"
#include "da_memory.h"
#include "binproto.h"

static enum DaOptions _sopt = OPT_ALLOC;

static void* (*saved_malloc_hook)(size_t, const void*);
static void* malloc_hook(size_t, const void*);
static void* (*saved_realloc_hook)(void*, size_t, const void*);
static void* realloc_hook(void*, size_t, const void*);
static void (*saved_free_hook)(void*, const void*);
static void free_hook(void*, const void*);

static void install_memory_hooks()
{
  __malloc_hook = malloc_hook;
  __realloc_hook = realloc_hook;
  __free_hook = free_hook;
}
static void teardown_memory_hooks()
{
  __malloc_hook = saved_malloc_hook;
  __realloc_hook = saved_realloc_hook;
  __free_hook = saved_free_hook;
}

void memory_initialize_hook(void)
{
  saved_malloc_hook = __malloc_hook;
  saved_realloc_hook = __realloc_hook;
  saved_free_hook = __free_hook;
  install_memory_hooks();
}

static void *malloc_hook(size_t size, const void* caller)
{
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	teardown_memory_hooks();
	PRE_PROBEBLOCK();

	pret = malloc(size);

	if(pret != NULL && gProbeBlockCount == 0)
	{
		add_memory_hash(pret, size, MEMTYPE_ALLOC, blockresult ? MEM_EXTERNAL : MEM_INTERNAL);
	}

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_malloc,
			  "xp", size, caller);
	PACK_COMMON_END(pret, newerrno, blockresult);
	PACK_MEMORY(size, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();

	install_memory_hooks();

	return pret;
}

static void free_hook(void *ptr, const void *caller)
{
	DECLARE_VARIABLE_STANDARD;

	teardown_memory_hooks();

	PRE_PROBEBLOCK();

	if(ptr != NULL && gProbeBlockCount == 0)
	{
		del_memory_hash(ptr, MEMTYPE_FREE, NULL);
	}

	free(ptr);

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_free,
			  "pp", ptr, caller);
	PACK_COMMON_END(0, newerrno, blockresult);
	PACK_MEMORY(0, MEMORY_API_FREE, ptr);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();

	install_memory_hooks();
}

static void* realloc_hook(void *memblock, size_t size, const void* caller)
{
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	teardown_memory_hooks();

	PRE_PROBEBLOCK();

	if(memblock != NULL && gProbeBlockCount == 0)
	{
		del_memory_hash(memblock, MEMTYPE_FREE, NULL);
	}

	pret = realloc(memblock, size);

	if(pret != NULL && gProbeBlockCount == 0)
	{
		add_memory_hash(pret, size, MEMTYPE_ALLOC, blockresult ? MEM_EXTERNAL : MEM_INTERNAL);
	}

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_realloc,
			  "pxp", memblock, size, caller);
	PACK_COMMON_END(pret, newerrno, blockresult);
	PACK_MEMORY(size, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();

	install_memory_hooks();

	return pret;
}

static inline void adhoc_bzero(char *p, size_t size)
{
	unsigned int index;
	for (index = 0; index != size; index++)
		p[index] = '\0';
}

void *calloc(size_t nelem, size_t elsize)
{
	DECLARE_VARIABLE_STANDARD;
	void *pret;
	size_t size = nelem * elsize;
	PRE_PROBEBLOCK();

	pret = (size < elsize) ? NULL : malloc(size);
	if (pret) /* `memset' somewhy deadloops */
		adhoc_bzero(pret, nelem * elsize);

	if(pret != NULL && gProbeBlockCount == 0)
	{
		add_memory_hash(pret, nelem * elsize, MEMTYPE_ALLOC, blockresult ? MEM_EXTERNAL : MEM_INTERNAL);
    }

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_calloc,
			  "xx", nelem, elsize);
	PACK_COMMON_END(pret, newerrno, blockresult);
	PACK_MEMORY(nelem * elsize, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();


	return pret;
}
