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

#ifndef __DA_MEMORY_H__
#define __DA_MEMORY_H__

#include "daprobe.h"

//#define INTERNALFILTERING		(!isEnableInternalMalloc())
#define INTERNAL_MALLOC_FILTERING		true
#define INTERNAL_FREE_FILTERING			true
#define INTERNAL_NEW_FILTERING			true
#define INTERNAL_DELETE_FILTERING		true

/*******************************************************************
 * MEMORY PROBE MACRO
 *
 * log format
 *		SeqNumber`,ApiName`,Time`,Pid`,Tid`,InputParm`,Return`,PCAddr`,Error`,
 *		Size`,MemoryAPIType`,Addr`,\n
 *		callstack_start`,callstack`,callstack_end
 *
 ********************************************************************/

#endif // __DA_MEMORY_H__
