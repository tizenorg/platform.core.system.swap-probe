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

#ifndef _DACAPTURE_H_
#define _DACAPTURE_H_


#ifdef X11_SUPPORT

#include <Evas.h>

int captureScreen();
int activateCaptureTimer();
void _cb_render_post(void *data, Evas *e, void *eventinfo);

int initialize_screencapture();
int finalize_screencapture();

#else /* X11_SUPPORT */

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

#endif /* X11_SUPPORT */


#endif /* _DACAPTURE_H_ */
