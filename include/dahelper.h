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

#ifndef _DAHELPER_H_
#define _DAHELPER_H_

#include <stdbool.h>
#include <pthread.h>			// for pthread_mutex_t
#include <sys/queue.h>			// for LIST_ENTRY, LIST_HEAD etc.;

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
		if(gTraceInfo.screenshot.state == 1) 					\
			gTraceInfo.screenshot.state = 2;					\
		pthread_mutex_unlock(&(gTraceInfo.screenshot.ssMutex));	\
		if(old == 1) {											\
			activateCaptureTimer();								\
		}														\
	} while(0)

#define SCREENSHOT_TIMEOUT()									\
	do {														\
		int old;												\
		pthread_mutex_lock(&(gTraceInfo.screenshot.ssMutex));	\
		old = gTraceInfo.screenshot.state;						\
		if(gTraceInfo.screenshot.state == 2) 					\
			gTraceInfo.screenshot.state = 1;					\
		pthread_mutex_unlock(&(gTraceInfo.screenshot.ssMutex));	\
		if(old == 2) {											\
			if(isOptionEnabled(OPT_SNAPSHOT))					\
				captureScreen();								\
		}														\
	} while(0)

#define	NUM_ORIGINAL_LIBRARY	10

typedef enum
{
	LIB_NO = -1,
	LIBC = 0,
	LIBPTHREAD = 1,
	LIBELEMENTARY = 2,
	LIBECORE_INPUT_EVAS = 3,
	LIBDAEMON = 4,
	LIBCAPI_APPFW_APPLICATION = 5,
	LIBGLES20 = 6,
	LIBGLES30 = 6,
	LIBEGL = 7,
	LIBSELF = 8,
	LIBEVAS = 9
} ORIGINAL_LIBRARY;

extern const char *lib_string[NUM_ORIGINAL_LIBRARY];
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
} __appInfo;

typedef struct
{
	int state;
	pthread_mutex_t ssMutex;
} __screenshotInfo;

struct bin_info_t {
	SLIST_ENTRY(bin_info_t) list;
	char *path;
};

struct bins_info_t {
	pthread_mutex_t bins_mutex;
	SLIST_HEAD(head, bin_info_t) bins_list;
};


typedef struct
{
	__indexInfo			index;
	__socketInfo		socket;
	__appInfo			app;
	__screenshotInfo	screenshot;
	int					stateTouch;
	int					init_complete;
	int					custom_chart_callback_count;
	uint64_t		optionflag;
	struct bins_info_t bins_info;
} __traceInfo;

extern __traceInfo gTraceInfo;

/* pid/tid values */
pid_t _getpid();
pid_t _gettid();
extern void reset_pid_tid();

//wchar_t* -> char*
char *absolutize_filepath(const char *fname, char *buf, size_t bufsiz);

/* returns the real absolute file path (resolves symlinks) */
char *real_abs_path(int fd, char *buffer, size_t bufsiz);

// screen capture functions
int captureScreen();
int activateCaptureTimer();
void _cb_render_post(void* data, Evas* e, void* eventinfo);

// event related functions
int initialize_event();
int finalize_event();
int getOrientation();
void on_orientation_changed(int angle, bool capi);

// query functions
#define isOptionEnabled(OPT)	((gTraceInfo.optionflag & OPT) != 0)

/* Binaries list functions */
int add_binary(char *path);
bool check_binary(const char *path);
int remove_binary(char *path);
void cleanup_binaries(void);

#ifdef __cplusplus
}
#endif

#endif	// _DAHELPER_H_
