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

#ifndef __COLLECTIONS_H__
#define __COLLECTIONS_H__

#include <pthread.h>			// for pthread_mutex_t

#include "khash.h"
#include "real_functions.h"
#include "gesture.h"

#ifdef __cplusplus
extern "C"{
#endif

#define MEMORYHASH			_hashinfo.memHash
#define MEMORYHASH_LOCK		pthread_mutex_lock(&(_hashinfo.memHashMutex))
#define MEMORYHASH_UNLOCK	pthread_mutex_unlock(&(_hashinfo.memHashMutex))

#define GESTUREHASH		_hashinfo.gestHash
#define GESTUREHASH_LOCK	pthread_mutex_lock(&(_hashinfo.gestHashMutex))
#define GESTUREHASH_UNLOCK	pthread_mutex_unlock(&(_hashinfo.gestHashMutex))

#define MEMTYPE_ALLOC	0x01
#define MEMTYPE_FREE	0x01
#define MEMTYPE_NEW		0x03
#define MEMTYPE_DELETE	0x03

#define MEM_INTERNAL	0x01
#define MEM_EXTERNAL	0x02

#define MEMMASK_CALLER	0xFFFF000000000000L
#define MEMMASK_TYPE	0x0000FFFF00000000L
#define MEMMASK_SIZE	0x00000000FFFFFFFFL

#define GET_MEMSIZE(_info)		((size_t)(_info & MEMMASK_SIZE))
#define GET_MEMCALLER(_info)	((unsigned short)((_info & MEMMASK_CALLER) >> 48))
#define GET_MEMTYPE(_info)		((unsigned short)((_info & MEMMASK_TYPE) >> 32))

#define MAKE_MEMINFO(caller, type, size)	\
	(((uint64_t)caller << 48) | ((uint64_t)type << 32) | ((uint64_t)size))

typedef struct
{
	char* type;
	char* name;
} _uiobjectinfo;

KHASH_INIT_TYPE_VOIDP(allocmap, uint64_t)
KHASH_INIT_TYPE_VOIDP(gesture, void *)

typedef struct
{
	khash_t(allocmap)*	memHash;
	pthread_mutex_t		memHashMutex;
	khash_t(gesture)*	gestHash;
	pthread_mutex_t		gestHashMutex;
} __hashInfo;

extern __hashInfo _hashinfo;

// APIs for symdata linked list
int add_to_glist(char* key, void* data);
int remove_from_glist(char* key);
int remove_all_glist();
void* find_glist(char* key);

// APIs for hash table
int initialize_hash_table();
int finalize_hash_table();

int add_memory_hash(void* ptr, size_t size, unsigned short type, unsigned short caller);
int del_memory_hash(void* ptr, unsigned short type, unsigned short* caller);

int find_gesture_hash(void *data);
void *add_gesture_hash(void *data);
int del_gesture_hash(void *data);

#ifdef __cplusplus
}
#endif

#endif /* __COLLECTIONS_H_ */
