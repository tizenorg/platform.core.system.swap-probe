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

#ifdef __cplusplus
extern "C"{
#endif

#define SYMBOLHASH			_hashinfo.symHash
#define SYMBOLHASH_LOCK		pthread_mutex_lock(&(_hashinfo.symHashMutex))
#define SYMBOLHASH_UNLOCK	pthread_mutex_unlock(&(_hashinfo.symHashMutex))

#define MEMORYHASH			_hashinfo.memHash
#define MEMORYHASH_LOCK		pthread_mutex_lock(&(_hashinfo.memHashMutex))
#define MEMORYHASH_UNLOCK	pthread_mutex_unlock(&(_hashinfo.memHashMutex))

#define OBJECTHASH			_hashinfo.objHash
#define OBJECTHASH_LOCK		pthread_mutex_lock(&(_hashinfo.objHashMutex))
#define OBJECTHASH_UNLOCK	pthread_mutex_unlock(&(_hashinfo.objHashMutex))

#define DETECTORHASH		_hashinfo.dttHash
#define DETECTORHASH_LOCK	pthread_mutex_lock(&(_hashinfo.dttHashMutex))
#define DETECTORHASH_UNLOCK	pthread_mutex_unlock(&(_hashinfo.dttHashMutex))

typedef struct
{
	char* type;
	char* name;
} _objectinfo;

// khash table function definition
KHASH_MAP_INIT_INT(symbol, char*)

KHASH_MAP_INIT_INT(allocmap, size_t)

KHASH_MAP_INIT_INT(object, _objectinfo*)

KHASH_MAP_INIT_INT(detector, void*)

typedef struct
{
	khash_t(symbol)*	symHash;
	pthread_mutex_t		symHashMutex;
	khash_t(allocmap)*	memHash;
	pthread_mutex_t		memHashMutex;
	khash_t(object)*	objHash;
	pthread_mutex_t		objHashMutex;
	khash_t(detector)*	dttHash;
	pthread_mutex_t		dttHashMutex;
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

int find_symbol_hash(void* ptr, char** psymbol);
int add_symbol_hash(void* ptr, const char* str, int strlen);
int add_memory_hash(void* ptr, size_t size);
int del_memory_hash(void* ptr);

int find_object_hash(void* ptr, char** type,  char** classname);
int add_object_hash_class(void* ptr, const char* classname);
int add_object_hash_type(void* ptr, const char* type);
int del_object_hash(void* ptr);

int add_detector_hash(void* ptr, void* listener);
int del_detector_hash(void* ptr);

#ifdef __cplusplus
}
#endif

#endif /* __COLLECTIONS_H_ */
