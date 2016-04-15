/*
 * SWAP probe
 *
 * Copyright (C) Samsung Electronics, 2016
 *
 * Contact:
 *
 * Vyacheslav Cherkashin <v.cherkashin@samsung.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#ifndef _ORIENTATION_H_
#define _ORIENTATION_H_


#ifdef X11_SUPPORT

#include <Ecore.h>

Ecore_Event_Handler *register_orientation_event_listener();
void unregister_orientation_event_listener(Ecore_Event_Handler *handler);

#else /* X11_SUPPORT */

#include "daprobe.h"

static inline Ecore_Event_Handler *register_orientation_event_listener()
{
	PRINTERR("X11 isn't support, orientation_event unavailable\n");
	return NULL;
}

static inline
void unregister_orientation_event_listener(Ecore_Event_Handler __unused *handler)
{
	PRINTERR("X11 isn't support, orientation_event unavailable\n");
}

#endif /* X11_SUPPORT */


#endif /* _ORIENTATION_H_ */
