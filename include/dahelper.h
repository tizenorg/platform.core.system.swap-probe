/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef _DAHELPER_H_
#define _DAHELPER_H_

#include <stdbool.h>
#include <pthread.h>			// for pthread_mutex_t

#include <Evas.h>

#include "daprobe.h"

#ifdef __cplusplus
extern "C"{
#endif

#define MAX_PATH_LENGTH		256
#define MAX_STACK_DEPTH		128
#define TRIM_STACK_DEPTH	2

/*
#define WIN_RENDER_POST 0
#define CONTROLBAR_RENDER_POST 1
#define NAVIFRAME_RENDER_POST 2
#define PAGER_RENDER_POST 3
#define RENDER_POST_COUNT 4

#define SNAPSHOT_WAIT_TIME_MAX 10000
*/

#define TS_NONE						0x0000
#define TS_PROBE					0x0001
#define TS_INIT						0x0002
#define TS_FINIT					0x0004
#define TS_ENTER_PROBE_BLOCK		0x0008
#define TS_SET_PROBE_POINT			0x0010
#define TS_PRINT_LOG				0x0020
#define TS_PRINT_SAMPLE_LOG			0x0040
#define TS_APPEND_TYPE_LOG			0x0080
#define TS_BACKTRACE				0x0100
#define TS_HASHFUNC					0x0200
#define TS_PROFIL_THREAD			0x0400
#define TS_PROFIL_COUNT				0x0800
#define TS_DETECT_TOUCH				0x1000
#define TS_REGIST_SCREEN_CHANGE		0x2000
#define TS_POSSIBLE_CAPTURE			0x4000
#define TRACE_STATE_SET(value)		((gSTrace) |= (value))
#define TRACE_STATE_UNSET(value)	((gSTrace) &= (~value))

#define ENABLE_INTERNAL_MALLOC		0x0001
#define ENABLE_SNAPSHOT				0x0002

#define SCREENSHOT_LOCK()										\
	do {														\
		int old;												\
		pthread_mutex_lock(&(gTraceInfo.screenshot.ssMutex));	\
		old = gTraceInfo.screenshot.state;						\
		gTraceInfo.screenshot.state = -1;						\
		pthread_mutex_unlock(&(gTraceInfo.screenshot.ssMutex));	\
		if(old > 0) {											\
			if(isOptionEnabled(OPT_SNAPSHOT))					\
				captureScreen();								\
		}														\
	} while(0)

#define SCREENSHOT_UNLOCK()										\
	do {														\
		pthread_mutex_lock(&(gTraceInfo.screenshot.ssMutex));	\
		gTraceInfo.screenshot.state = 1;						\
		pthread_mutex_unlock(&(gTraceInfo.screenshot.ssMutex));	\
	} while(0)

#define SCREENSHOT_SET()										\
	do {														\
		int old;												\
		pthread_mutex_lock(&(gTraceInfo.screenshot.ssMutex));	\
		old = gTraceInfo.screenshot.state;						\
		if(gTraceInfo.screenshot.state >= 0)					\
			gTraceInfo.screenshot.state = 1;					\
		pthread_mutex_unlock(&(gTraceInfo.screenshot.ssMutex));	\
		if(old == 2) {											\
			if(isOptionEnabled(OPT_SNAPSHOT))					\
				captureScreen();								\
		}														\
	} while(0)

#define SCREENSHOT_UNSET()										\
	do {														\
		pthread_mutex_lock(&(gTraceInfo.screenshot.ssMutex));	\
		if(gTraceInfo.screenshot.state >= 0)					\
			gTraceInfo.screenshot.state = 0;					\
		pthread_mutex_unlock(&(gTraceInfo.screenshot.ssMutex));	\
	} while(0)

#define SCREENSHOT_DONE()										\
	do {														\
		int old;												\
		pthread_mutex_lock(&(gTraceInfo.screenshot.ssMutex));	\
		old = gTraceInfo.screenshot.state;						\
		if(gTraceInfo.screenshot.state == 1) {					\
			gTraceInfo.screenshot.state = 2;					\
		} else if(gTraceInfo.screenshot.state == 2) {			\
			gTraceInfo.screenshot.state = 0;					\
		} else {												\
		}														\
		pthread_mutex_unlock(&(gTraceInfo.screenshot.ssMutex));	\
		if(old == 1) {											\
			activateCaptureTimer();								\
		} else if(old == 2) {									\
			if(isOptionEnabled(OPT_SNAPSHOT))					\
				captureScreen();								\
		}														\
	} while(0)

#define	NUM_ORIGINAL_LIBRARY	9

typedef enum
{
	LIBC = 0,
	LIBPTHREAD = 1,
	LIBELEMENTARY = 2,
	LIBOSP_UIFW = 3,
	LIBOSP_APPFW = 4,
	LIBOSP_WEB = 5,
	LIBECORE_INPUT_EVAS = 6,
	LIBDAEMON = 7,
	LIBCAPI_APPFW_APPLICATION = 8
} ORIGINAL_LIBRARY;

extern char *lib_string[NUM_ORIGINAL_LIBRARY];
extern void *lib_handle[NUM_ORIGINAL_LIBRARY];

// type definition for global variable
typedef struct
{
	int eventIndex;
	pthread_mutex_t eventMutex;
} __indexInfo;

typedef struct
{
	int daemonSock;
	pthread_mutex_t sockMutex;
} __socketInfo;

typedef struct
{
	char appName[128];
	unsigned int startTime;
} __appInfo;

typedef struct
{
	int state;
	pthread_mutex_t ssMutex;
} __screenshotInfo;

typedef struct
{
	void* map_start;
	void* map_end;
} __mapInfo;

typedef struct
{
	__indexInfo			index;
	__socketInfo		socket;
	__appInfo			app;
	__screenshotInfo	screenshot;
	__mapInfo			exec_map;
	int					stateTouch;
	int					init_complete;
	int					custom_chart_callback_count;
	unsigned long		optionflag;
} __traceInfo;

extern __thread unsigned long	gSTrace;
extern __traceInfo gTraceInfo;

int get_map_address(void* symbol, void** map_start, void** map_end);
char** da_backtrace_symbols (void* const* array, int size);
char** cached_backtrace_symbols (void* const* array, int size);

// profil turned on
int __profil(int mode);

//wchar_t* -> char*
void WcharToChar(char* pstrDest, const wchar_t* pwstrSrc);

// screen capture functions
int initialize_screencapture();
int finalize_screencapture();
int captureScreen();
int activateCaptureTimer();
void _cb_render_post(void* data, Evas* e, void* eventinfo);

// event related functions
int initialize_event();
int finalize_event();
int getOrientation();
void orientationEnabled();
void on_orientation_changed(int angle, bool capi);

int remove_indir(const char* dirname);

// query functions
#define isOptionEnabled(OPT)	((gTraceInfo.optionflag & OPT) != 0)

#ifdef __cplusplus
}
#endif

#endif	// _DAHELPER_H_
