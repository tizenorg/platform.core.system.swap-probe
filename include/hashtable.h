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

#ifndef __HASHTABLE__
#define __HASHTABLE__


#include <stddef.h>


#define DEFINE_HTABLE(name, size) \
	struct hlist_head name[] = { [0 ... (size - 1)] = { .first = { .next = NULL } } }


struct hlist_node {
	struct hlist_node *next;
};

struct hlist_head {
	struct hlist_node first;
};


static inline void hlist_head_init(struct hlist_head *h)
{
	h->first.next = NULL;
}

static inline void hlist_node_init(struct hlist_node *n)
{
	n->next = NULL;
}

static inline void hlist_add(struct hlist_head *h, struct hlist_node *n)
{
	n->next = h->first.next;
	h->first.next = n;
}

static inline void hlist_del(struct hlist_node *prev, struct hlist_node *n)
{
	prev->next = n->next;
}

static inline void hlist_del_by_prev(struct hlist_node *prev)
{
	hlist_del(prev, prev->next);
}

#define hlist_entry(ptr, type, member) container_of(ptr, type, member)
#define hlist_for_each(node, head) \
	for (node = (head)->first.next; node; node = node->next)

#define hlist_table_for_each(node, idx, array, size)	\
	for (idx = 0; idx < size; ++idx)		\
		hlist_for_each(node, &array[idx])


#endif /* __HASHTABLE__ */
