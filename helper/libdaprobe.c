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

#include <stdio.h>			// for printf
#include <stdlib.h>			// for getenv
#include <string.h>			// for strstr
#include <stdbool.h>		// for bool
#include <stdint.h>			// fot uint32_t,uint64_t
#include <stdarg.h>			// for va_list, va_arg(__appendTypeLog)
#include <execinfo.h>		// for backtrace, backtrace_symbols
#include <unistd.h>			// for write, alarm function, syscall
#include <pthread.h>		// for pthread_mutex_lock

#include <sys/syscall.h>	// for syscall
#include <sys/time.h>		// for gettimeofday
#include <sys/socket.h>		// for socket, connect
#include <sys/un.h>			// for sockaddr_un

#include "probeinfo.h"
#include "dautil.h"
#include "dahelper.h"
#include "dacollection.h"

#define APP_INSTALL_PATH		"/opt/apps"
#define UDS_NAME				"/tmp/da.socket"

__thread int			gProbeDepth = 0;
__thread unsigned int	gProbeBlockCount = 0;
__thread pid_t			gTid = -1;

int getExecutableMappingAddress();

/******************************************************************************
 * internal functions
   (this means that these functions do not need to set enter/exit flag)
 ******************************************************************************/
 
// create sokcet to daemon and connect
static int createSocket(void)
{
	int clientLen, ret = 0;
	struct sockaddr_un clientAddr;
	char buf[16];

	if((gTraceInfo.socket.daemonSock = socket(AF_UNIX, SOCK_STREAM,0)) != -1)
	{
		bzero(&clientAddr, sizeof(clientAddr));
		clientAddr.sun_family = AF_UNIX;
		sprintf(clientAddr.sun_path, "%s", UDS_NAME);

		clientLen = sizeof(clientAddr);
		if(connect(gTraceInfo.socket.daemonSock, (struct sockaddr *)&clientAddr, clientLen) >= 0)
		{
			PRINTMSG("createSocket connect() success\n");
			sprintf(buf, "%d", getpid());
			printLogStr(buf, MSG_PID);
			sprintf(buf, "%u", gTraceInfo.app.startTime);
			printLogStr(buf, MSG_TIME);
		}
		else
		{
			close(gTraceInfo.socket.daemonSock);
			gTraceInfo.socket.daemonSock = -1;
			ret = -1;
		}
	}
	else
	{
		ret = -1;
	}

	return ret;
}


// parse backtrace string and find out the caller of probed api function
// return 0 if caller is user binary, otherwise return 1
static int determineCaller(char* tracestring)
{
	char *apppath, *substr;

	// determine whether saveptr (caller binary name) is user binary or not
//	apppath = getenv("APP_PATH");
	apppath = NULL;

	if(apppath == NULL)
		substr = strstr(tracestring, APP_INSTALL_PATH);
	else
		substr = strstr(tracestring, apppath);

	if(substr == NULL)	// not user binary
		return 1;
	else				// user binary
		return 0;
}

// return current thread id
static pid_t _gettid()
{
	if(gTid == -1)
		gTid = syscall(__NR_gettid);	// syscall is very expensive
	return gTid;
}

/*****************************************************************************
 * initialize / finalize function
 *****************************************************************************/

void __attribute__((constructor)) _init_probe()
{
	struct timeval cTime;

	TRACE_STATE_SET(TS_INIT);

	initialize_hash_table();

	initialize_screencapture();

	initialize_event();

	getExecutableMappingAddress();

	// get app start time
	gettimeofday(&cTime, NULL);
	gTraceInfo.app.startTime = ((cTime.tv_sec * 10000 + (cTime.tv_usec/100)));

	// create socket for communication with da_daemon
#ifndef PRINT_STDOUT
	if(createSocket() >= 0)
#endif
	if(__atSharedMemory() < 0)
	{
		printLogStr("cannot at shared memory in helper", MSG_ERROR);
	}

	PRINTMSG("dynamic analyzer probe helper so loading...\n");

	gTraceInfo.init_complete = 1;
	TRACE_STATE_UNSET(TS_INIT);
}

void __attribute__((destructor)) _fini_probe()
{
	TRACE_STATE_SET(TS_FINIT);

	gTraceInfo.init_complete = -1;
	PRINTMSG("dynamic analyzer probe helper so unloading...\n");

	__dtSharedMemory();

	remove_all_glist();

	// close socket
#ifndef PRINT_STDOUT
	if(gTraceInfo.socket.daemonSock != -1)
	{
		printLogStr(NULL, MSG_TERMINATE);
		close(gTraceInfo.socket.daemonSock);
		gTraceInfo.socket.daemonSock = -1;
	}
#endif

	finalize_event();

	finalize_screencapture();

	finalize_hash_table();

	TRACE_STATE_UNSET(TS_FINIT);
}


/**************************************************************************
 * Helper APIs
 **************************************************************************/

/************************************************************************
 * manipulate and print log functions
 ************************************************************************/
bool printLog(log_t* log, int msgType)
{
	if(unlikely(gTraceInfo.socket.daemonSock == -1))
		return false;

	if(unlikely(log == NULL))
		return false;

	TRACE_STATE_SET(TS_PRINT_LOG);

#ifndef PRINT_STDOUT
	log->type = msgType;

	pthread_mutex_lock(&(gTraceInfo.socket.sockMutex));
	send(gTraceInfo.socket.daemonSock, log, sizeof(log->type) + sizeof(log->length) + log->length, 0);
	pthread_mutex_unlock(&(gTraceInfo.socket.sockMutex));

#else
	if(log != NULL && log->length > 0 && msgType == MSG_MSG)
		printf("%s\n", log->data);
#endif
	TRACE_STATE_UNSET(TS_PRINT_LOG);

	return true;
}

bool printLogStr(const char* str, int msgType)
{
#ifndef PRINT_STDOUT
	log_t log;
#endif

	if(unlikely(gTraceInfo.socket.daemonSock == -1))
		return false;

	TRACE_STATE_SET(TS_PRINT_LOG);

#ifndef PRINT_STDOUT
	log.type = msgType;

	if(str)
	{
		sprintf(log.data, "%s", str);
		log.length = strlen(str);
	}
	else
	{
		log.length = 0;
	}

	pthread_mutex_lock(&(gTraceInfo.socket.sockMutex));
	send(gTraceInfo.socket.daemonSock, &log, sizeof(log.type) + sizeof(log.length) + log.length, 0);
	pthread_mutex_unlock(&(gTraceInfo.socket.sockMutex));

#else
	if(str != NULL && msgType == MSG_MSG)
		printf("%s\n", str);
#endif
	TRACE_STATE_UNSET(TS_PRINT_LOG);

	return true;
}

// return 0 for successful case
// return non-zero for error
// if token is NULL then use DEFAULT TOKEN "`,"
// if token is not NULL then insert DEFAULT TOKEN before append input
int __appendTypeLog(log_t* log, int nInput, char* token, ...)
{
	static char* default_token = DEFAULT_TOKEN;
	va_list p_arg;
	int i, type;
	char* seperator = default_token;

	if(nInput <= 0 || log == NULL)
		return -1;

	TRACE_STATE_SET(TS_APPEND_TYPE_LOG);

	va_start(p_arg, token);

	if(token != NULL)
		seperator = token;

	for(i = 0; i < nInput; i++)
	{
		type = va_arg(p_arg, int);

		if(likely(log->length > 0))	// append token or default token
		{
			if(unlikely(i == 0))
				log->length += sprintf(log->data + log->length, "%s", default_token);
			else
				log->length += sprintf(log->data + log->length, "%s", seperator);
		}

		switch(type)
		{
		case VT_INT:
			log->length += sprintf(log->data + log->length, "%d", va_arg(p_arg, int));
			break;
		case VT_UINT:
			log->length += sprintf(log->data + log->length, "%u", va_arg(p_arg, unsigned int));
			break;
		case VT_LONG:
			log->length += sprintf(log->data + log->length, "%ld", va_arg(p_arg, long));
			break;
		case VT_ULONG:
			log->length += sprintf(log->data + log->length, "%lu", va_arg(p_arg, unsigned long));
			break;
		case VT_STR:
			log->length += sprintf(log->data + log->length, "%s", va_arg(p_arg, char*));
			break;
		case VT_CHAR:	// 'char' is promoted to 'int' when passed through '...'
			log->length += sprintf(log->data + log->length, "%c", va_arg(p_arg, int));
			break;
		case VT_PTR:
			log->length += sprintf(log->data + log->length, "%p", va_arg(p_arg, void*));
			break;
		case VT_NULL:
			va_arg(p_arg, unsigned int);
			break;
		case VT_OFF_T:
			log->length += sprintf(log->data + log->length, "%ld", va_arg(p_arg, off_t));
			break;
		case VT_SIZE_T:
			log->length += sprintf(log->data + log->length, "%u", va_arg(p_arg, size_t));
			break;
		case VT_SSIZE_T:
			log->length += sprintf(log->data + log->length, "%d", va_arg(p_arg, ssize_t));
			break;
		case VT_SOCKLEN_T:
			log->length += sprintf(log->data + log->length, "%u", va_arg(p_arg, socklen_t));
			break;
		case VT_UINT16_T:	// 'uint16_t' is promoted to 'int' when passed through '...'
			log->length += sprintf(log->data + log->length, "%u", va_arg(p_arg, int));
			break;
		case VT_UINT32_T:
			log->length += sprintf(log->data + log->length, "%u", va_arg(p_arg, uint32_t));
			break;
		case VT_UINT64_T:
			log->length += sprintf(log->data + log->length, "%llu", va_arg(p_arg, uint64_t));
			break;
		case VT_MODE_T:
			log->length += sprintf(log->data + log->length, "%u", va_arg(p_arg, mode_t));
			break;
/*		case VT_DEV_T:
			log->length += sprintf(log->data + log->length, "%lu", va_arg(p_arg, dev_t));
			break;
		case VT_NFDS_T:
			log->length += sprintf(log->data + log->length, "%lu", va_arg(p_arg, nfds_t));
			break;*/
		default:
			va_end(p_arg);
			TRACE_STATE_UNSET(TS_APPEND_TYPE_LOG);
			return -1;
		}
	}

	va_end(p_arg);

	TRACE_STATE_UNSET(TS_APPEND_TYPE_LOG);
	return 0;
}

// get backtrace string
// return stack depth if succeed, otherwise return 0
// parameter 'log' cannot be null
int getBacktraceString(log_t* log, int bufsize)
{
	void* array[MAX_STACK_DEPTH];
	char** strings = NULL;
	size_t i, size;
	int initsize;
	int stringlen;

	if(log == NULL)
		return 0;

	TRACE_STATE_SET(TS_BACKTRACE);

	initsize = log->length;
	log->data[log->length] = '\0';	// is this necessary ?
	size = backtrace(array, MAX_STACK_DEPTH);
	if(likely(size > TRIM_STACK_DEPTH))
	{
		strings = BACKTRACE_SYMBOLS(array + TRIM_STACK_DEPTH, size - TRIM_STACK_DEPTH);

		if(likely(strings != NULL))
		{
			for(i = TRIM_STACK_DEPTH; i < size; i++)
			{
				stringlen = strlen(strings[i - TRIM_STACK_DEPTH]) + 14;
				if(log->length + stringlen >= bufsize + initsize)
					break;

				log->length += sprintf(log->data + log->length, "%010u`,%s`,", (unsigned int)(array[i]), strings[i - TRIM_STACK_DEPTH]);
			}
			log->data[log->length-2] = '\0';
			log->length -= 2;
			free(strings);
		}
		else	// failed to get backtrace symbols
		{
			// just print trace address
			for(i = TRIM_STACK_DEPTH; i < size; i++)
			{
				stringlen = 23;
				if(log->length + stringlen >= bufsize + initsize)
					break;

				log->length += sprintf(log->data + log->length, "%010u`,(unknown)`,", (unsigned int)(array[i]));
			}
			log->data[log->length-2] = '\0';
			log->length -= 2;
		}

		TRACE_STATE_UNSET(TS_BACKTRACE);
		return (int)(size - TRIM_STACK_DEPTH);
	}
	else
	{
		TRACE_STATE_UNSET(TS_BACKTRACE);
		return 0;
	}
}

/*
int printSamplingLog()
{
	log_t log;

	TRACE_STATE_SET(TS_PRINT_SAMPLE_LOG);
	log.length = sprintf(log.data, "%d`,%lu`,%d`,%Ld",
			LC_ALLOCMEM, getCurrentTime(), getpid(), total_alloc_size);

	printLog(&log, MSG_LOG);
	TRACE_STATE_UNSET(TS_PRINT_SAMPLE_LOG);
	return 0;
}
*/

/*************************************************************************
 * probe block control functions
 *************************************************************************/
int preBlockBegin(void* caller, bool bFiltering)
{
	bool user = false;
	void* tarray[1];
	char** strings;

	if(gSTrace != 0 || gProbeDepth != 0)
		return 0;

	if(gTraceInfo.init_complete <= 0)
		return 0;

	TRACE_STATE_SET(TS_ENTER_PROBE_BLOCK);

	if(gTraceInfo.exec_map.map_start != NULL)
	{
		// address comparison
		if(caller >= gTraceInfo.exec_map.map_start &&
				caller <= gTraceInfo.exec_map.map_end)
		{
			user = true;
		}
		else
		{
			// nothing to do
		}
	}
	else
	{
		// backtrace for filtering
		tarray[0] = caller;
		strings = BACKTRACE_SYMBOLS(tarray, 1);
		if(strings != NULL)
		{
			if((determineCaller(strings[0]) == 0))
				user = true;
			free(strings);
		}
		else
		{
			// nothing to do
		}
	}

	if((!user) && bFiltering)
	{
		TRACE_STATE_UNSET(TS_ENTER_PROBE_BLOCK);
		return 0;
	}
	else
	{
		// nothing to do
	}

	gProbeDepth++;

	if(user)
		return 2;	// user call
	else
		return 1;	// internal call
}

int postBlockBegin(int preresult)
{
	if(preresult)
	{
		TRACE_STATE_SET(TS_ENTER_PROBE_BLOCK);
	}

	return preresult;
}

void preBlockEnd()
{
	TRACE_STATE_UNSET(TS_ENTER_PROBE_BLOCK);
}

void postBlockEnd()
{
	gProbeDepth--;
	TRACE_STATE_UNSET(TS_ENTER_PROBE_BLOCK);
}

// for block that have to be run
void probeBlockStart()
{
	gProbeBlockCount++;
	if(gProbeBlockCount == 1)
		TRACE_STATE_SET(TS_PROBE);
}

void probeBlockEnd()
{
	if(gProbeBlockCount == 1)
		TRACE_STATE_UNSET(TS_PROBE);
	gProbeBlockCount--;
}

/*************************************************************************
 * helper info getter functions
 *************************************************************************/
// return current time in 1/10000 sec unit
unsigned long getCurrentTime()
{
	struct timeval cTime;

	gettimeofday(&cTime, NULL);

	return (unsigned long)((cTime.tv_sec * 10000 + (cTime.tv_usec/100)));
}

unsigned int getCurrentEventIndex()
{
	return gTraceInfo.index.eventIndex;
}

unsigned int getCallDepth()
{
	return gProbeDepth;
}

unsigned long getTraceState()
{
	return gSTrace;
}

bool isEnableSnapshot()
{
	if(gTraceInfo.pprobeflag != NULL)
		return (bool)(*gTraceInfo.pprobeflag & ENABLE_SNAPSHOT);
	else
		return false;
}

bool isEnableInternalMalloc()
{
	if(gTraceInfo.pprobeflag != NULL)
		return (bool)(*gTraceInfo.pprobeflag & ENABLE_INTERNAL_MALLOC);
	else
		return false;
}


/******************************************************************
 * screen capture and event related functions
 ******************************************************************/

/*
// TRACE_STATE_SET is not necessary because this function is called in probe block only
int registeScreenChange(int renderID)
{
	if(gTraceInfo.snapshot.isTouchDown == 0)
	{
		return 0;
	}

	if(gTraceInfo.snapshot.snapshotAPICallStartTime == 0){
		gTraceInfo.snapshot.snapshotAPICallStartTime = getCurrentTime();
		gTraceInfo.snapshot.snapshotAPICallCount = 1;
	}
	else
		gTraceInfo.snapshot.snapshotAPICallCount++;

	gTraceInfo.snapshot.renderID = renderID;

	return 1;
}

void detectTouchEvent(int touchID)
{
	gTraceInfo.stateTouch = touchID;

	TRACE_STATE_SET(TS_DETECT_TOUCH);
	if(isEnableSnapshot())
	{
		if(touchID == EVENT_TYPE_UP)
		{
			if(gTraceInfo.snapshot.isTouchDown == 1)
			{
				gTraceInfo.snapshot.snapshotAPICallStartTime = getCurrentTime();
				captureProbe();
			}
			gTraceInfo.snapshot.isTouchDown = 1;
		}
	}
	TRACE_STATE_UNSET(TS_DETECT_TOUCH);
}

int getTouchState()
{
	return gTraceInfo.stateTouch;
}

// TRACE_STATE_SET is not necessary because this function is called in probe block only
int isPossibleCapture()
{
	if(gTraceInfo.snapshot.isTouchDown == 0)
	{
		return 0;
	}
	else
		gTraceInfo.snapshot.snapshotAPICallCount--;

	if(((getCurrentTime() - gTraceInfo.snapshot.snapshotAPICallStartTime) < SNAPSHOT_WAIT_TIME_MAX) && (gTraceInfo.snapshot.snapshotAPICallCount > 0))
	{
		return 0;
	}

	gTraceInfo.snapshot.isTouchDown = 0;
	gTraceInfo.snapshot.snapshotAPICallStartTime = 0;
	gTraceInfo.snapshot.snapshotAPICallCount = 0;
	gTraceInfo.snapshot.renderID = -1;

	return 1;
}
*/

/************************************************************************
 * probe functions
 ************************************************************************/
bool setProbePoint(probeInfo_t* iProbe)
{
	if(unlikely(iProbe == NULL))
	{
		return false;
	}

	TRACE_STATE_SET(TS_SET_PROBE_POINT);

	// atomic operaion(gcc builtin) is more expensive then pthread_mutex
	pthread_mutex_lock(&(gTraceInfo.index.eventMutex));	
	iProbe->eventIndex = gTraceInfo.index.eventIndex++;
	pthread_mutex_unlock(&(gTraceInfo.index.eventMutex));

	iProbe->currentTime = getCurrentTime();
	iProbe->pID = getpid();
	iProbe->tID = _gettid();
	iProbe->callDepth = gProbeDepth;

	TRACE_STATE_UNSET(TS_SET_PROBE_POINT);
	return true;
}

