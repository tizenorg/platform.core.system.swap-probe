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

#include <assert.h>		// for assert
#include <stdlib.h>		// for malloc, free
#include <string.h>		// for strlen, strcpy
#include <pthread.h>	// for pthread_mutex_t

#include "dahelper.h"
#include "daerror.h"
#include "dacollection.h"
#include "gesture.h"

// khash table function definition

KHASH_MAP_INIT_VOIDP(allocmap, uint64_t)

KHASH_INIT(gesture, void *, void *, 1, kh_gesture_calc_hash, kh_gesture_cmp)

// hash table variable
__hashInfo _hashinfo =
{
	NULL,						// khash_t(allocmap)*	memHash
	PTHREAD_MUTEX_INITIALIZER,	// pthread_mutex_t memHashMutex
	NULL,						// khash_t(gesture)*	gestHash
	PTHREAD_MUTEX_INITIALIZER	// pthread_mutex_t gestHashMutex
};

// glist typedef and variable
struct _element
{
	int		keylen;
	char*	keystr;
	void*	dataptr;
	struct _element*	next;
	struct _element*	prev;
};

typedef struct _element element_t;

element_t* gsymbol_list = NULL;
pthread_mutex_t glist_mutex = PTHREAD_MUTEX_INITIALIZER;

// *********************************************************************************************
// hash table related implemenation
// *********************************************************************************************

int initialize_hash_table()
{
	MEMORYHASH_LOCK;
	MEMORYHASH = kh_init(allocmap);
	MEMORYHASH_UNLOCK;

	GESTUREHASH_LOCK;
	GESTUREHASH = kh_init(gesture);
	GESTUREHASH_UNLOCK;
	return 0;
}

int finalize_hash_table()
{
	if (MEMORYHASH)
	{
		MEMORYHASH_LOCK;
		kh_destroy(allocmap, MEMORYHASH);
		MEMORYHASH = NULL;
		MEMORYHASH_UNLOCK;
	}

	if (GESTUREHASH) {
		khiter_t k;
		void *val;

		GESTUREHASH_LOCK;
		for(k = kh_begin(GESTUREHASH); k != kh_end(GESTUREHASH); k++) {
			if (kh_exist(GESTUREHASH, k)) {
				val = kh_value(GESTUREHASH, k);
				if (likely(val != NULL))
					free(val);
			}
		}
		kh_destroy(gesture, GESTUREHASH);
		GESTUREHASH = NULL;
		GESTUREHASH_UNLOCK;
	}


	return 0;
}

/***********************************************************
 * memory hash related functions
 ***********************************************************/
// return 0 if succeed
// return 1 if key is already exist in hash table
// return negative value if other error occurred
int add_memory_hash(void* ptr, size_t size, unsigned short type, unsigned short caller)
{
	khiter_t k;
	int rethash, ret = 0;
	size_t memsize;
	uint64_t meminfo;

	if (unlikely(MEMORYHASH == 0))
		return ERR_NOTINITIALIZED;

	if (unlikely(ptr == NULL))
		return ERR_WRONGPARAMETER;

	MEMORYHASH_LOCK;
	k = kh_put(allocmap, MEMORYHASH, ptr, &rethash);
	if (likely(rethash != 0))	// succeed to add in hash table
	{
		kh_value(MEMORYHASH, k) = MAKE_MEMINFO(caller, type, size);
		update_heap_memory_size(true, size);
	}
	else
	{
		// key is already exist in hash
		// update memory info
		meminfo = kh_value(MEMORYHASH, k);
		memsize = GET_MEMSIZE(meminfo);
		if (memsize == size)
		{
			kh_value(MEMORYHASH, k) = MAKE_MEMINFO(caller, type, size);
		}
		ret = 1;
	}
	MEMORYHASH_UNLOCK;

	return ret;
}

// return 0 if succeed
// return 1 if key is not in hash table
// return negative if other error occurred
int del_memory_hash(void* ptr, unsigned short type, unsigned short* caller)
{
	khiter_t k;
	int ret = 0;
	uint32_t size;
	uint64_t meminfo;

	if (unlikely(MEMORYHASH == 0))
		return ERR_NOTINITIALIZED;

	if (unlikely(ptr == NULL))
		return ERR_WRONGPARAMETER;

	MEMORYHASH_LOCK;
	k = kh_get(allocmap, MEMORYHASH, ptr);
	if (likely(k != kh_end(MEMORYHASH)))
	{				// there is entry in hash table
		meminfo = kh_value(MEMORYHASH, k);
		if (unlikely(type != GET_MEMTYPE(meminfo)))
		{
			ret = -1;
		}
		else
		{
			size = GET_MEMSIZE(meminfo);
			if (caller != NULL)
				*caller = GET_MEMCALLER(meminfo);
			update_heap_memory_size(false, size);
			kh_del(allocmap, MEMORYHASH, k);
		}
	}
	else
	{
		ret = 1;	// there is not entry in hash table
	}
	MEMORYHASH_UNLOCK;

	return ret;
}

// *********************************************************************************************
// glist implemenation
// *********************************************************************************************

// all function to call this function have to use mutex_lock
static element_t* find_element(char* key)
{
	element_t* res;
	int keylen;

	if (unlikely(key == NULL))
		return NULL;

	keylen = strlen(key);

//	pthread_mutex_lock(&glist_mutex);	// this is commented because of static function
	res = gsymbol_list;
	while(res != NULL)
	{
		if (keylen == res->keylen && (strcmp(key, res->keystr) == 0))
			break;
		res = res->next;
	}
//	pthread_mutex_unlock(&glist_mutex);	// this is commented because of static function

	return res;
}

int add_to_glist(char* key, void* data)
{
	element_t* elm;
	int ret = 0;

	if (unlikely(key == NULL || data == NULL))
		return 0;

	pthread_mutex_lock(&glist_mutex);
	elm = find_element(key);
	if (elm == NULL)
	{
		elm = (element_t*)real_malloc(sizeof(element_t));
		if (likely(elm != NULL))
		{
			elm->keylen = strlen(key);
			elm->keystr = (char*)real_malloc(elm->keylen + 1);
			if (likely(elm->keystr != NULL))
			{
				memcpy(elm->keystr, key, elm->keylen + 1);
				elm->dataptr = data;
				elm->next = gsymbol_list;
				elm->prev = NULL;
				if (gsymbol_list)
					gsymbol_list->prev = elm;
				gsymbol_list = elm;
				ret = 1;
			}
			else
			{
				free(elm);
			}
		}
	}
	pthread_mutex_unlock(&glist_mutex);

	return ret;
}

int remove_from_glist(char* key)
{
	element_t* elm;
	int ret = 0;

	pthread_mutex_lock(&glist_mutex);
	elm = find_element(key);
	if (elm != NULL)
	{
		if (elm->prev)
			elm->prev->next = elm->next;
		if (elm->next)
			elm->next->prev = elm->prev;
		if (gsymbol_list == elm)
			gsymbol_list = gsymbol_list->next;
	}
	pthread_mutex_unlock(&glist_mutex);

	if (elm != NULL)
	{
		free(elm->dataptr);
		free(elm->keystr);
		free(elm);
		ret = 1;
	}

	return ret;
}

int remove_all_glist()
{
	element_t* elm;

	pthread_mutex_lock(&glist_mutex);
	while(gsymbol_list != NULL)
	{
		elm = gsymbol_list;
		gsymbol_list = elm->next;
		free(elm->dataptr);
		free(elm->keystr);
		free(elm);
	}
	pthread_mutex_unlock(&glist_mutex);

	return 1;
}

void* find_glist(char* key)
{
	element_t* elm;

	pthread_mutex_lock(&glist_mutex);
	elm = find_element(key);
	pthread_mutex_unlock(&glist_mutex);

	if (elm)
		return elm->dataptr;
	else
		return NULL;
}


/***********************************************************
 * gesture hash related functions
 ***********************************************************
 * return 0 if succeed
 * return 1 if key is already exist in hash table
 * return negative value if other error occurred
 */
//uint32_t kh_gesture_calc_hash(struct __elm_gesture_layer_cb_set_data *elm)
uint32_t kh_gesture_calc_hash(void *data)
{
	struct __elm_gesture_layer_cb_set_data *elm = data;

	/* FIXME cast to uint64 */
	uint32_t res = ((uint32_t)elm->obj << 16) +
		       ((uint32_t)elm->idx << 8) +
		       ((uint32_t)elm->cb_type << 0);
	return res;
}

int kh_gesture_cmp(void *data1, void *data2)
{
	int res = 1;
	struct __elm_gesture_layer_cb_set_data *elm1 = data1;
	struct __elm_gesture_layer_cb_set_data *elm2 = data2;

	if (elm1 == NULL || elm2 == NULL) {
		PRINTERR("wrong incoming data");
		goto exit;
	}

	res = (elm1->obj == elm2->obj &&
	       elm1->idx == elm2->idx &&
	       elm1->cb_type == elm2->cb_type);

exit:
	return res;
}

int gesture_update(struct __elm_gesture_layer_cb_set_data *elm1,
		   struct __elm_gesture_layer_cb_set_data *elm2)
{
	int res = 0;
	if (elm1 == NULL || elm2 == NULL) {
		PRINTERR("wrong incoming data");
		res = 1;
		goto exit;
	} else {
		elm1->cb = elm2->cb;
		elm1->data = elm2->data;
	}

exit:
	return res;
}

void *add_gesture_hash(void *ptr)
{
	khiter_t k;
	int rethash;
	void *ret = NULL;
	struct __elm_gesture_layer_cb_set_data *data;

	if (GESTUREHASH == NULL) {
		PRINTERR("gesture hash not initialized");
		goto exit;
	}

	if (ptr == NULL) {
		PRINTERR("wrong incoming data");
		goto exit;
	}

	GESTUREHASH_LOCK;

	data = (struct __elm_gesture_layer_cb_set_data *)ptr;
	k = kh_put(gesture, GESTUREHASH, data, &rethash);
	if (rethash != 0) {
		// succeed to add in hash table
		if (data != NULL) {
			data = (struct __elm_gesture_layer_cb_set_data *)real_malloc(sizeof(*data));
			memcpy(data, ptr, sizeof(*data));

			kh_key(GESTUREHASH, k) = data;
			kh_value(GESTUREHASH, k) = data;
			ret = data;
		} else {
			PRINTERR("Cannot alloc memory.");
			goto exit_unlock;
		}
	} else {
		// key is already exist in hash
		// update memory info
		data = kh_value(GESTUREHASH, k);
		if (gesture_update(data, (struct __elm_gesture_layer_cb_set_data *)ptr) != 0) {
			PRINTERR("Cannot update hash data");
			goto exit_unlock;
		}
		ret = data;
	}

exit_unlock:
	GESTUREHASH_UNLOCK;

exit:
	return ret;
}
