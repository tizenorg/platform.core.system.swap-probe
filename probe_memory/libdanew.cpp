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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <errno.h>
#include <new>
#include "daprobe.h"
#include "probeinfo.h"
#include "dacollection.h"
#include "da_memory.h"
#include "binproto.h"

//#define INTERNALFILTERING		(!isEnableInternalMalloc())
#define INTERNALFILTERING		true

static enum DaOptions _sopt = OPT_ALLOC;

void *operator new(std::size_t size) throw (std::bad_alloc)
{
	static void*(*newp)(std::size_t size);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	GET_REAL_FUNCP_RTLD_NEXT_CPP(_Znwj,newp);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	pret = newp(size);

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

void *operator new[](std::size_t size) throw (std::bad_alloc)
{
	static void*(*newp)(std::size_t size);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	GET_REAL_FUNCP_RTLD_NEXT_CPP(_Znaj, newp);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	pret = newp(size);

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

void operator delete(void *ptr) throw()
{
	static void (*deletep)(void *ptr);
	DECLARE_VARIABLE_STANDARD;

	GET_REAL_FUNCP_RTLD_NEXT_CPP(_ZdlPv, deletep);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	if(ptr != NULL && getTraceState() == 0)
	{
		del_memory_hash(ptr);
	}

	deletep(ptr);

	POST_PACK_PROBEBLOCK_BEGIN();
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY, LC_MEMORY, "p", ptr);
	PACK_COMMON_END(0, newerrno, blockresult);
	PACK_MEMORY(0, MEMORY_API_FREE, ptr);
	FLUSH_LOCAL_BUF();
	
	POST_PACK_PROBEBLOCK_END();
}

void operator delete[](void *ptr) throw()
{
	static void (*deletep)(void *ptr);
	DECLARE_VARIABLE_STANDARD;

	GET_REAL_FUNCP_RTLD_NEXT_CPP(_ZdaPv, deletep);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	if(ptr != NULL && getTraceState() == 0)
	{
		del_memory_hash(ptr);
	}

	deletep(ptr);

	POST_PACK_PROBEBLOCK_BEGIN();
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY, LC_MEMORY, "p", ptr);
	PACK_COMMON_END(0, newerrno, blockresult);
	PACK_MEMORY(0, MEMORY_API_FREE, ptr);
	FLUSH_LOCAL_BUF();
	
	POST_PACK_PROBEBLOCK_END();
}

void *operator new(std::size_t size, const std::nothrow_t& nothrow) throw()
{
	static void*(*newp)(std::size_t size, const std::nothrow_t& nothrow);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	GET_REAL_FUNCP_RTLD_NEXT_CPP(_ZnwjRKSt9nothrow_t, newp);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	pret = newp(size, nothrow);

	if(pret != NULL && getTraceState() == 0)
	{
		add_memory_hash(pret, size);
	}

	POST_PACK_PROBEBLOCK_BEGIN();
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY, LC_MEMORY, "xp", size, &nothrow);
	PACK_COMMON_END(pret, newerrno, blockresult);
	PACK_MEMORY(size, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();
	
	POST_PACK_PROBEBLOCK_END();

	return pret;
}

void *operator new[](std::size_t size, const std::nothrow_t& nothrow) throw()
{
	static void*(*newp)(std::size_t size, const std::nothrow_t& nothrow);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	GET_REAL_FUNCP_RTLD_NEXT_CPP(_ZnajRKSt9nothrow_t, newp);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	pret = newp(size, nothrow);

	if(pret != NULL && getTraceState() == 0)
	{
		add_memory_hash(pret, size);
	}

	POST_PACK_PROBEBLOCK_BEGIN();
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY, LC_MEMORY, "xp", size, &nothrow);
	PACK_COMMON_END(pret, newerrno, blockresult);
	PACK_MEMORY(size, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();
	
	POST_PACK_PROBEBLOCK_END();

	return pret;
}

void operator delete(void *ptr, const std::nothrow_t& nothrow) throw()
{
	static void (*deletep)(void *ptr, const std::nothrow_t& nothrow);
	DECLARE_VARIABLE_STANDARD;

	GET_REAL_FUNCP_RTLD_NEXT_CPP(_ZdlPvRKSt9nothrow_t, deletep);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	if(ptr != NULL && getTraceState() == 0)
	{
		del_memory_hash(ptr);
	}

	deletep(ptr, nothrow);

	POST_PACK_PROBEBLOCK_BEGIN();
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY, LC_MEMORY, "pp", ptr, &nothrow);
	PACK_COMMON_END(0, newerrno, blockresult);
	PACK_MEMORY(0, MEMORY_API_FREE, ptr);
	FLUSH_LOCAL_BUF();
	
	POST_PACK_PROBEBLOCK_END();
}

void operator delete[](void *ptr, const std::nothrow_t& nothrow) throw()
{
	static void (*deletep)(void *ptr, const std::nothrow_t& nothrow);
	DECLARE_VARIABLE_STANDARD;

	GET_REAL_FUNCP_RTLD_NEXT_CPP(_ZdaPvRKSt9nothrow_t, deletep);

	bfiltering = INTERNALFILTERING;
	PRE_PROBEBLOCK();

	if(ptr != NULL && getTraceState() == 0)
	{
		del_memory_hash(ptr);
	}

	deletep(ptr, nothrow);

	POST_PACK_PROBEBLOCK_BEGIN();
		
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY, LC_MEMORY, "pp", ptr, &nothrow);
	PACK_COMMON_END(0, newerrno, blockresult);
	PACK_MEMORY(0, MEMORY_API_FREE, ptr);
	FLUSH_LOCAL_BUF();
	
	POST_PACK_PROBEBLOCK_END();
}

