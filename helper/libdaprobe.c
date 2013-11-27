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
 * Nikita Kalyazin <n.kalyazin@samsung.com>
 * Anastasia Lyupa <a.lyupa@samsung.com>
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
 * - Samsung RnD Institute Russia
 *
 */

#include <stdio.h>			// for sprintf
#include <stdlib.h>			// for getenv
#include <string.h>			// for strstr
#include <stdbool.h>		// for bool
#include <stdint.h>			// fot uint32_t,uint64_t
#include <stdarg.h>			// for va_list, va_arg(__appendTypeLog)
#include <execinfo.h>		// for backtrace, backtrace_symbols
#include <unistd.h>			// for write, alarm function, syscall
#include <pthread.h>		// for pthread_mutex_lock
#include <signal.h>
#include <stdint.h>

#include <sys/syscall.h>	// for syscall
#include <sys/time.h>		// for gettimeofday
#include <sys/socket.h>		// for socket, connect
#include <sys/un.h>			// for sockaddr_un
#include <sys/timerfd.h>	// for timerfd

#include <app.h>
#include "probeinfo.h"
#include "dautil.h"
#include "dahelper.h"
#include "dacollection.h"
#include "da_sync.h"
#include "daprobe.h"

#include "binproto.h"

#define APP_INSTALL_PATH		"/opt/apps"
#define TISEN_APP_POSTFIX			".exe"
#define UDS_NAME				"/tmp/da.socket"
#define TIMERFD_INTERVAL		100000000		// 0.1 sec

__thread int		gProbeBlockCount = 0;
__thread int		gProbeDepth = 0;
pid_t gPid = -1;
__thread pid_t		gTid = -1;

int			g_timerfd = 0;
long		g_total_alloc_size = 0;
pthread_t	g_recvthread_id;

int log_fd = 0;

int getExecutableMappingAddress();

bool printLog(log_t* log, int msgType);
/******************************************************************************
 * internal functions
   (this means that these functions do not need to set enter/exit flag)
 ******************************************************************************/

// runtime configure the probe option
static void _configure(char* configstr)
{
	char buf[64];
	gTraceInfo.optionflag = atoll(configstr);

	sprintf(buf, "configure in probe : %s, %llx\n", configstr, gTraceInfo.optionflag);
	PRINTMSG(buf);
}

// create socket to daemon and connect
static int createSocket(void)
{
	ssize_t recvlen;
	int clientLen, ret = 0;
	struct sockaddr_un clientAddr;
	log_t log;

	gTraceInfo.socket.daemonSock =
	  socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (gTraceInfo.socket.daemonSock != -1)	{
		memset(&clientAddr, '\0', sizeof(clientAddr));
		clientAddr.sun_family = AF_UNIX;
		sprintf(clientAddr.sun_path, "%s", UDS_NAME);

		clientLen = sizeof(clientAddr);
		if (connect(gTraceInfo.socket.daemonSock,
			    (struct sockaddr *)&clientAddr, clientLen) >= 0)
		{
			char buf[64];
			/* recv initial configuration value */
			recvlen = recv(gTraceInfo.socket.daemonSock, &log,
				       sizeof(log.type) + sizeof(log.length),
				       MSG_WAITALL);

			if (recvlen > 0) {/* recv succeed */
				if (log.length > 0) {
					if(log.length >= DA_LOG_MAX)
						log.length = DA_LOG_MAX - 1;
					recvlen = recv(gTraceInfo.socket.daemonSock, log.data,
						       log.length, MSG_WAITALL);
				} else {
					log.length = 0;
				}

				log.data[log.length] = '\0';

				if(log.type == MSG_CONFIG)
				{
					_configure(log.data);
				}
				else
				{
					// unexpected case
				}
			} else if (recvlen < 0) {
				sprintf(buf, "recv failed in socket creation with error(%d)\n", recvlen);
			} else {
				/* closed by other peer */
			}

			sprintf(buf, "%d|%llu", getpid(),
				gTraceInfo.app.startTime);
			printLogStr(buf, MSG_PID);
			PRINTMSG("createSocket connect() success\n");
		} else {
			close(gTraceInfo.socket.daemonSock);
			gTraceInfo.socket.daemonSock = -1;
			ret = -1;
		}
	} else {
		ret = -1;
	}

	return ret;
}


// parse backtrace string and find out the caller of probed api function
// return 0 if caller is user binary, otherwise return 1
static int determineCaller(char* tracestring)
{
	char *substr;

	// determine whether saveptr (caller binary name) is user binary or not
	substr = strstr(tracestring, APP_INSTALL_PATH);

	if(substr == NULL)	// not user binary
	{
		return 1;
	}
	else				// user binary
	{
#ifdef TISENAPP
		substr = strstr(tracestring, TISEN_APP_POSTFIX);
		if(substr == NULL)
			return 1;
#endif
		return 0;
	}
}

// return current process id
static pid_t _getpid()
{
	if (gPid == -1)
		gPid = getpid();
	return gPid;
}

// return current thread id
static pid_t _gettid()
{
	if(gTid == -1)
		gTid = syscall(__NR_gettid);	// syscall is very expensive
	return gTid;
}

static void *recvThread(void __unused * data)
{
	fd_set readfds, workfds;
	int maxfd = 0, rc;
	uint64_t xtime;
	ssize_t recvlen;
	log_t log;
	sigset_t profsigmask;

	if(gTraceInfo.socket.daemonSock == -1)
		return NULL;

	probeBlockStart();

	sigemptyset(&profsigmask);
	sigaddset(&profsigmask, SIGPROF);
	pthread_sigmask(SIG_BLOCK, &profsigmask, NULL);

	FD_ZERO(&readfds);
	if(g_timerfd > 0)
	{
		maxfd = g_timerfd;
		FD_SET(g_timerfd, &readfds);
	}
	if(maxfd < gTraceInfo.socket.daemonSock)
		maxfd = gTraceInfo.socket.daemonSock;
	FD_SET(gTraceInfo.socket.daemonSock, &readfds);

	while(1)
	{
		workfds = readfds;
		rc = select(maxfd + 1, &workfds, NULL, NULL, NULL);
		if(rc < 0)
		{
			continue;
		}

		if(g_timerfd > 0 && FD_ISSET(g_timerfd, &workfds))
		{
			recvlen = read(g_timerfd, &xtime, sizeof(xtime));
			if(recvlen > 0)
			{
				log.length = sprintf(log.data, "%ld", g_total_alloc_size);
				printLog(&log, MSG_ALLOC);
			}
			else
			{
				// read failed
			}
			continue;
		}
		else if(FD_ISSET(gTraceInfo.socket.daemonSock, &workfds))
		{
			recvlen = recv(gTraceInfo.socket.daemonSock, &log,
					sizeof(log.type) + sizeof(log.length), MSG_WAITALL);

			if(recvlen > 0)	// recv succeed
			{
				if(log.length > 0)
				{
					if(log.length >= DA_LOG_MAX)
						log.length = DA_LOG_MAX - 1;
					recvlen = recv(gTraceInfo.socket.daemonSock, log.data,
						log.length, MSG_WAITALL);
				}
				else
				{
					log.length = 0;
				}

				log.data[log.length] = '\0';

				if (log.type == MSG_CAPTURE_SCREEN) {
					captureScreen();
				} else if (log.type == MSG_CONFIG) {
					_configure(log.data);
				}
				else if(log.type == MSG_STOP)
				{
					application_exit();
				}
				else
				{
					char buf[64];
					sprintf(buf, "recv unknown message(%d)\n", log.type);
					continue;
				}
			}
			else if(recvlen == 0)	// closed by other peer
			{
				close(gTraceInfo.socket.daemonSock);
				gTraceInfo.socket.daemonSock = -1;
				break;
			}
			else	// recv error
			{
				char buf[64];
				sprintf(buf, "recv failed in recv thread with error(%d)\n", recvlen);
				continue;
			}
		}
		else	// unknown case
		{
			continue;
		}
	}

	probeBlockEnd();
	return NULL;
}

/*****************************************************************************
 * initialize / finalize function
 *****************************************************************************/
static int init_timerfd(void)
{
	const struct timespec interval = {
		.tv_sec = 0,
		.tv_nsec = TIMERFD_INTERVAL
	};
	const struct itimerspec ctime = {
		.it_interval = interval,
		.it_value = interval
	};

	int timer = timerfd_create(CLOCK_REALTIME, TFD_CLOEXEC);

	if (timer == -1) {
		PRINTMSG("failed to create timerdf\n");
		return -1;
	}

	if (timerfd_settime(timer, 0, &ctime, NULL) != 0) {
		PRINTMSG("failed to set timerfd\n");
		close(timer);
		return -1;
	}

	return timer;
}

static uint64_t get_app_start_time(void)
{
	enum {nsecs_in_sec = 1000 * 1000};
	struct timeval time;

	gettimeofday(&time, NULL);
	return nsecs_in_sec * (uint64_t) time.tv_sec + time.tv_usec;
}

static int create_recv_thread()
{
	int err = pthread_create(&g_recvthread_id, NULL, recvThread, NULL);

	if (err)
		PRINTMSG("failed to crate recv thread\n");

	return err;
}

void __attribute__((constructor)) _init_probe()
{
	probeBlockStart();

	initialize_hash_table();

	initialize_screencapture();

	initialize_event();

	getExecutableMappingAddress();

	gTraceInfo.app.startTime = get_app_start_time();

	// create socket for communication with da_daemon
	if (createSocket() == 0) {
		g_timerfd = init_timerfd();
		create_recv_thread();
		update_heap_memory_size(true, 0);
	}


	PRINTMSG("dynamic analyzer probe helper so loading...\n");

	gTraceInfo.init_complete = 1;
	probeBlockEnd();
}

void __attribute__((destructor)) _fini_probe()
{
	int i;
	probeBlockStart();

	gTraceInfo.init_complete = -1;
	PRINTMSG("dynamic analyzer probe helper so unloading...\n");

	remove_all_glist();

	// close timerfd
	if(g_timerfd > 0)
		close(g_timerfd);

	// close socket
	if(gTraceInfo.socket.daemonSock != -1)
	{
		printLogStr(NULL, MSG_TERMINATE);
		close(gTraceInfo.socket.daemonSock);
		gTraceInfo.socket.daemonSock = -1;
	}

	finalize_event();

	finalize_screencapture();

	finalize_hash_table();

	for(i = 0; i < NUM_ORIGINAL_LIBRARY; i++)
	{
		if(lib_handle[i] != NULL)
		{
			dlclose(lib_handle[i]);
		}
	}

	probeBlockEnd();
}


/**************************************************************************
 * Helper APIs
 **************************************************************************/

/************************************************************************
 * manipulate and print log functions
 ************************************************************************/
bool printLog(log_t *log, int msgType)
{
	int res;
	if(unlikely(gTraceInfo.socket.daemonSock == -1))
		return false;

	if(unlikely(log == NULL))
		return false;

	probeBlockStart();
	log->type = msgType;
	real_pthread_mutex_lock(&(gTraceInfo.socket.sockMutex));
	res = send(gTraceInfo.socket.daemonSock, log, sizeof(log->type) + sizeof(log->length) + log->length, 0);
	real_pthread_mutex_unlock(&(gTraceInfo.socket.sockMutex));
	probeBlockEnd();

	return true;
}

bool printLogStr(const char* str, int msgType)
{
	int res;
	log_t log;

	if(unlikely(gTraceInfo.socket.daemonSock == -1))
		return false;

	probeBlockStart();

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

	real_pthread_mutex_lock(&(gTraceInfo.socket.sockMutex));
	res = send(gTraceInfo.socket.daemonSock, &log, sizeof(log.type) + sizeof(log.length) + log.length, MSG_DONTWAIT);
	real_pthread_mutex_unlock(&(gTraceInfo.socket.sockMutex));

	probeBlockEnd();

	return true;
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

	probeBlockStart();

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

		probeBlockEnd();
		return (int)(size - TRIM_STACK_DEPTH);
	}
	else
	{
		probeBlockEnd();
		return 0;
	}
}

/*************************************************************************
 * probe block control functions
 *************************************************************************/
static inline bool isNoFiltOptionEnabled(enum DaOptions option)
{
	return
		((option == OPT_ALLOC)
		 && isOptionEnabled(OPT_ALLOC_ALWAYS))
		|| ((option == OPT_FILE)
		    && isOptionEnabled(OPT_FILE_ALWAYS))
		|| ((option == OPT_THREAD)
		    && isOptionEnabled(OPT_THREAD_ALWAYS))
		|| ((option == OPT_UI)
		    && isOptionEnabled(OPT_UI_ALWAYS))
		|| ((option == OPT_NETWORK)
		    && isOptionEnabled(OPT_NETWORK_ALWAYS))
		|| ((option == OPT_GLES)
		    && isOptionEnabled(OPT_GLES_ALWAYS));
}

int preBlockBegin(void* caller, bool bFiltering, enum DaOptions option)
{
	bool user = false;
	void* tarray[1];
	char** strings;
	bool opt_nofilt;

	if(gProbeBlockCount != 0 || gProbeDepth != 0)
		return 0;

	if(gTraceInfo.init_complete <= 0)
		return 0;

	opt_nofilt = isNoFiltOptionEnabled(option);

	/* Actually we are considering 3 conditions here:
	    - regular option is enabled
	    - non-filtering (always) option is enabled
	    - per-probe filtering
	*/
	if (!isOptionEnabled(option) && !opt_nofilt)
		return 0;
	else if (opt_nofilt)
		bFiltering = false;

	probeBlockStart();

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

	if(user)
	{
		probingStart();
		return 2;	// user call
	}
	else
	{
		if(bFiltering)
		{
			probeBlockEnd();
			return 0;	// not probing
		}
		else
		{
			probingStart();
			return 1;	// internal call
		}
	}
}

int postBlockBegin(int preresult)
{
	if(preresult)
	{
		probeBlockStart();
	}

	return preresult;
}

void preBlockEnd()
{
	probeBlockEnd();
}

void postBlockEnd()
{
	probingEnd();
	probeBlockEnd();
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

uint64_t get_current_nsec(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (uint64_t)tv.tv_sec * 1000 * 1000 * 1000 + tv.tv_usec * 1000;
}

/************************************************************************
 * probe functions
 ************************************************************************/
bool setProbePoint(probeInfo_t* iProbe)
{
	if(unlikely(iProbe == NULL))
	{
		return false;
	}

	probeBlockStart();

	// atomic operaion(gcc builtin) is more expensive then pthread_mutex
	real_pthread_mutex_lock(&(gTraceInfo.index.eventMutex));
	iProbe->eventIndex = gTraceInfo.index.eventIndex++;
	real_pthread_mutex_unlock(&(gTraceInfo.index.eventMutex));

	iProbe->currentTime = getCurrentTime();
	iProbe->pID = _getpid();
	iProbe->tID = _gettid();
	iProbe->callDepth = gProbeDepth;

	probeBlockEnd();
	return true;
}

// update heap memory size through socket
// return 0 if size is updated through socket
// return 1 if size is updated into global variable
int update_heap_memory_size(bool isAdd, size_t size)
{
	long tmp;
	if(isAdd)
	{
		tmp = __sync_add_and_fetch(&g_total_alloc_size, (long)size);
	}
	else
	{
		tmp = __sync_sub_and_fetch(&g_total_alloc_size, (long)size);
	}

	return 0;
}
