/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Woojin Jung <woojin2.jung@samsung.com>
 * Jaewon Lim <jaewon81.lim@samsung.com>
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
#include "memory_probes_list.h"
#include "probe_memory.h"


//HANDLER_DEF_THROW(void *, new, (std::bad_alloc), std::size_t size)
HANDLER_WRAPPERS_THROW(memory_feature, void *, new, (std::bad_alloc), std::size_t, size)
{
	void*(*newp)(std::size_t size);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	newp = (void*(*)(std::size_t size))orig;

	PRE_PROBEBLOCK();

	pret = newp(size);

	if(pret != NULL)
		add_memory_hash(pret, size, MEMTYPE_NEW, (unsigned short)call_type);

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_void__operator_new_std__size_t_size__throw__std__bad_alloc_,
			  "x", (uint64_t)(size));
	PACK_COMMON_END('p', pret, newerrno, call_type, caller);
	PACK_MEMORY(size, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();

	return pret;
}

//HANDLER_DEF_THROW(void *, new_array, (std::bad_alloc), std::size_t size)
HANDLER_WRAPPERS_THROW(memory_feature, void *, new_array, (std::bad_alloc), std::size_t, size)
{
	void*(*newp)(std::size_t size);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	newp = (void*(*)(std::size_t size))orig;

	PRE_PROBEBLOCK();

	pret = newp(size);

	if(pret != NULL)
		add_memory_hash(pret, size, MEMTYPE_NEW, call_type);

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_void__operator_new___std__size_t_size__throw__std__bad_alloc_,
			  "x", (uint64_t)(size));
	PACK_COMMON_END('p', pret, newerrno, call_type, caller);
	PACK_MEMORY(size, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();

	return pret;
}

//HANDLER_DEF_THROW(void, delete, (), void *ptr)
HANDLER_WRAPPERS_THROW(memory_feature, void, delete, (), void *, ptr)
{
	void (*deletep)(void *ptr);
	unsigned short ct;
	DECLARE_VARIABLE_STANDARD;

	deletep = (void (*)(void *))orig;

	PRE_PROBEBLOCK();

	if(ptr != NULL)
	{
		/* TODO call_type rewrited as it was previously. Is it needed? */
		ret = del_memory_hash(ptr, MEMTYPE_DELETE, &ct);
		if(blockresult == 0 && ret == 0 && ct == EXTERNAL_CALL)
		{
			inc_current_event_index();
			blockresult = 2;
		}
	}

	deletep(ptr);

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_void_operator_delete_void__ptr__throw__,
			  "p", voidp_to_uint64(ptr));
	PACK_COMMON_END('v', 0, newerrno, call_type, caller);
	PACK_MEMORY(0, MEMORY_API_FREE, ptr);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();
}

//HANDLER_DEF_THROW(void, delete_array, (), void *ptr)
HANDLER_WRAPPERS_THROW(memory_feature, void, delete_array, (), void *, ptr)
{
	void (*deletep)(void *ptr);
	unsigned short ct;
	DECLARE_VARIABLE_STANDARD;

	deletep = (void (*)(void *))orig;

	PRE_PROBEBLOCK();

	if(ptr != NULL)
	{
		/* TODO call_type rewrited as it was previously. Is it needed? */
		ret = del_memory_hash(ptr, MEMTYPE_DELETE, &ct);
		if(blockresult == 0 && ret == 0 && call_type == EXTERNAL_CALL)
		{
			inc_current_event_index();
			blockresult = 2;
		}
	}

	deletep(ptr);

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_void_operator_delete___void__ptr__throw__,
			  "p", voidp_to_uint64(ptr));
	PACK_COMMON_END('v', 0, newerrno, call_type, caller);
	PACK_MEMORY(0, MEMORY_API_FREE, ptr);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();
}

//HANDLER_DEF_THROW(void *, new_nothrow, (), std::size_t size,
//		  const std::nothrow_t& nothrow)
HANDLER_WRAPPERS_THROW(memory_feature, void *, new_nothrow, (), std::size_t, size,
		       const std::nothrow_t&, nothrow)
{
	void*(*newp)(std::size_t size, const std::nothrow_t& nothrow);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	newp = (void*(*)(std::size_t, const std::nothrow_t& ))orig;

	PRE_PROBEBLOCK();

	pret = newp(size, nothrow);

	if(pret != NULL)
		add_memory_hash(pret, size, MEMTYPE_NEW, (unsigned short)call_type);

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_void__operator_new_std__size_t_size__const_std__nothrow_t__nothrow__throw__,
			  "xp", (uint64_t)(size), voidp_to_uint64(&nothrow));
	PACK_COMMON_END('p', pret, newerrno, call_type, caller);
	PACK_MEMORY(size, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();

	return pret;
}

//HANDLER_DEF_THROW(void *, new_array_nothrow, (), std::size_t size,
//		  const std::nothrow_t& nothrow)
HANDLER_WRAPPERS_THROW(memory_feature, void *, new_array_nothrow, (), std::size_t, size,
		       const std::nothrow_t&, nothrow)
{
	void*(*newp)(std::size_t size, const std::nothrow_t& nothrow);
	DECLARE_VARIABLE_STANDARD;
	void *pret;

	newp = (void*(*)(std::size_t, const std::nothrow_t&))orig;

	PRE_PROBEBLOCK();

	pret = newp(size, nothrow);

	if(pret != NULL)
		add_memory_hash(pret, size, MEMTYPE_NEW, call_type);

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_void__operator_new___std__size_t_size__const_std__nothrow_t__nothrow__throw__,
			  "xp", (uint64_t)(size), voidp_to_uint64(&nothrow));
	PACK_COMMON_END('p', pret, newerrno, call_type, caller);
	PACK_MEMORY(size, MEMORY_API_ALLOC, pret);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();

	return pret;
}

//HANDLER_DEF_THROW(void, delete_nothrow, (), void *ptr,
//		  const std::nothrow_t& nothrow)
HANDLER_WRAPPERS_THROW(memory_feature, void, delete_nothrow, (), void *, ptr,
		       const std::nothrow_t&, nothrow)
{
	void (*deletep)(void *ptr, const std::nothrow_t& nothrow);
	unsigned short ct;
	DECLARE_VARIABLE_STANDARD;

	deletep = (void (*)(void *, const std::nothrow_t&))orig;

	PRE_PROBEBLOCK();

	if(ptr != NULL)
	{
		/* TODO call_type rewrited as it was previously. Is it needed? */
		ret = del_memory_hash(ptr, MEMTYPE_DELETE, &ct);
		if(blockresult == 0 && ret == 0 && call_type == EXTERNAL_CALL)
		{
			inc_current_event_index();
			blockresult = 2;
		}
	}

	deletep(ptr, nothrow);

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_void_operator_delete_void__ptr__const_std__nothrow_t__nothrow__throw__,
			  "pp", voidp_to_uint64(ptr), voidp_to_uint64(&nothrow));
	PACK_COMMON_END('v', 0, newerrno, call_type, caller);
	PACK_MEMORY(0, MEMORY_API_FREE, ptr);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();
}

//HANDLER_DEF_THROW(void, delete_array_nothrow, (), void *ptr,
//		  const std::nothrow_t& nothrow)
HANDLER_WRAPPERS_THROW(memory_feature, void, delete_array_nothrow, (), void *, ptr,
		       const std::nothrow_t&, nothrow)
{
	void (*deletep)(void *ptr, const std::nothrow_t& nothrow);
	unsigned short ct;
	DECLARE_VARIABLE_STANDARD;

	deletep = (void (*)(void *, const std::nothrow_t&))orig;

	PRE_PROBEBLOCK();

	if(ptr != NULL)
	{
		/* TODO call_type rewrited as it was previously. Is it needed? */
		ret = del_memory_hash(ptr, MEMTYPE_DELETE, &ct);
		if(blockresult == 0 && ret == 0 && call_type == EXTERNAL_CALL)
		{
			inc_current_event_index();
			blockresult = 2;
		}
	}

	deletep(ptr, nothrow);

	POST_PACK_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_MEMORY,
			  API_ID_void_operator_delete___void__ptr__const_std__nothrow_t__nothrow__throw__,
			  "pp", voidp_to_uint64(ptr), voidp_to_uint64(&nothrow));
	PACK_COMMON_END('v', 0, newerrno, call_type, caller);
	PACK_MEMORY(0, MEMORY_API_FREE, ptr);
	FLUSH_LOCAL_BUF();

	POST_PACK_PROBEBLOCK_END();
}
