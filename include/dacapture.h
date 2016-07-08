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

#ifndef _DACAPTURE_H_
#define _DACAPTURE_H_


#ifdef TIZEN_FEATURE_WAYLAND

#include <Evas.h>

int captureScreen();
int activateCaptureTimer();
void _cb_render_post(void *data, Evas *e, void *eventinfo);

int initialize_screencapture();
int finalize_screencapture();

#else /* TIZEN_FEATURE_WAYLAND */

#include "daprobe.h"

static inline int captureScreen()
{
	return -1;
}

static inline int activateCaptureTimer()
{
	return -1;
}

static inline void _cb_render_post(void __unused *data, Evas __unused *e,
				   void __unused *eventinfo)
{
}

static inline int initialize_screencapture()
{
	PRINTERR("X11 isn't support, screencapture unavailable\n");
	return -1;
}

static inline int finalize_screencapture()
{
	return -1;
}

#endif /* TIZEN_FEATURE_WAYLAND */


#endif /* _DACAPTURE_H_ */
