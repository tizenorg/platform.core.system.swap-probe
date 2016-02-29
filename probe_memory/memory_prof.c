/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) Samsung Electronics, 2016
 *
 * 2016         Vyacheslav Cherkashin <v.cherkashin@samsung.com>
 *
 */


#include <errno.h>
#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>
#include <hash.h>
#include <daprobe.h>
#include <hashtable.h>
#include <real_functions.h>


#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) *__mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member));})

struct stack_data {
	size_t cnt;			/* num entries */
	unsigned long stacktrace[0];	/* stack entries */
};

struct alloc_info {
	struct hlist_node node;		/* hash table node link */
	unsigned long addr;		/* allocated memory address */
	size_t size;			/* requested memory size */
	struct stack_data *stacktrace;	/* stack trace data */
};


enum {
	AI_HASH_BITS = 12,
	AI_TABLE_SIZE = HASH_TABLE_SIZE(AI_HASH_BITS),
};

//static const unsigned int bt_depth = 5;


static struct alloc_info *alloc_info_get(void)
{
	return real_malloc(sizeof(struct alloc_info));
}

static void alloc_info_put(struct alloc_info *info)
{
	real_free(info);
}

//static inline unsigned long *alloc_info_bt(struct alloc_info *info)
//{
//	return info->back_trace;
//}

static inline struct alloc_info *alloc_info_by_node(struct hlist_node *n)
{
	return container_of(n, struct alloc_info, node);
}


// lock/unlock
static pthread_rwlock_t htable_lock = PTHREAD_RWLOCK_INITIALIZER;

static inline void htable_r_lock(void)
{
	pthread_rwlock_rdlock(&htable_lock);
}

static inline void htable_r_unlock(void)
{
	pthread_rwlock_unlock(&htable_lock);
}

static inline void htable_w_lock(void)
{
	pthread_rwlock_wrlock(&htable_lock);
}

static inline void htable_w_unlock(void)
{
	pthread_rwlock_unlock(&htable_lock);
}


// hash table
static DEFINE_HTABLE(alloc_info_htable, AI_TABLE_SIZE);

static struct hlist_head *head_by_addr(unsigned long addr)
{
	unsigned long idx = hash(addr, AI_HASH_BITS);

	return &alloc_info_htable[idx];
}

static struct hlist_node *find_prev(unsigned long addr,
				    struct hlist_head *head)
{
	struct hlist_node *node, *prev = &head->first;
	bool found = false;

	hlist_for_each(node, head) {
		struct alloc_info *info;

		info = alloc_info_by_node(node);
		if (info->addr == addr) {
			found = true;
			break;
		}

		prev = node;
	}

	return found ? prev : NULL;
}



// interface
int mp_add(void *ptr, size_t size)
{
	struct alloc_info *info;
	struct hlist_node *prev;
	unsigned long addr = (unsigned long)ptr;
	struct hlist_head *head = head_by_addr(addr);

	htable_r_lock();
	prev = find_prev(addr, head);
	htable_r_unlock();

	if (prev)
		return -EFAULT;

	info = alloc_info_get();
	if (info == NULL)
		return -ENOMEM;

	// fill parameters
	info->addr = addr;
	info->size = size;

	htable_w_lock();
	hlist_add(head, &info->node);
	htable_w_unlock();

	return 0;
}

int mp_del(void *ptr)
{
	struct alloc_info *info;
	struct hlist_node *prev, *node = NULL;
	unsigned long addr = (unsigned long)ptr;
	struct hlist_head *head = head_by_addr(addr);

	htable_w_lock();
	prev = find_prev(addr, head);
	if (prev) {
		node = prev->next;
		hlist_del(prev, node);
	}
	htable_w_unlock();

	if (node == NULL)
		return -EINVAL;

	info = alloc_info_by_node(node);
	alloc_info_put(info);

	return 0;
}

int mp_resize(void *ptr, size_t size)
{
	int ret = 0;
	struct alloc_info *info;
	struct hlist_node *prev, *node = NULL;
	unsigned long addr = (unsigned long)ptr;
	struct hlist_head *head = head_by_addr(addr);

	htable_r_lock();
	prev = find_prev(addr, head);
	if (prev) {
		node = prev->next;
		info = alloc_info_by_node(node);
		info->size = size;
	} else {
		ret = -EINVAL;
	}
	htable_r_unlock();

	return ret;
}

int mp_export(void (*flog)(const char *fmt, ...))
{
	int i;
	struct hlist_node *node;
	struct alloc_info *info;

	printf("\nEXPORT B:\n");

	hlist_table_for_each(node, i, alloc_info_htable, AI_TABLE_SIZE) {
		info = alloc_info_by_node(node);
		printf("memleak: addr=%lx size=%lu\n", info->addr, info->size);
		flog("memleak: addr=%lx size=%lu\n", info->addr, info->size);
	}

	printf("EXPRT E:\n");

	return 0;
}
