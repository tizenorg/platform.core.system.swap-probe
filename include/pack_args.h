/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Vitaliy Cherepanov <v.cherepanov@samsung.com>
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
 * - Samsung RnD Institute Russia
 *
 */

#ifndef __PACK_ARGS_H__
#define __PACK_ARGS_H__

#include <stdint.h>

#define PACK_CALL_ARRAY()\
pack_add(A_INT32,   'd', pack_int32),\
pack_add(A_VOIDP64, 'p', pack_p64), \
pack_add(A_VOID,    'v', pack_void), \
pack_add(A_SHADER,  's', pack_shader),\
pack_add(A_FILEPATH,'s', pack_filepath), \
/* A_TAIL is last */ pack_add(A_TAIL,    '0', 0)

#define pack_add(id, ret, func) id
enum handler {
	PACK_CALL_ARRAY()
};
#undef pack_add

typedef void *(*pack_handler_t)(char *to, void *);
extern pack_handler_t pack_handler[];
extern char pack_type_char[];

struct packable_t{
	const enum handler h;
	const void *arg;
};

#endif /* __PACK_ARGS_H__ */
