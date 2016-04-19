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
 * Vitaliy Cherepanov <v.cherepanov@samsung.com>
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

#include "probeinfo.h"
#include "dahelper.h"
#include "dacollection.h"
#include "da_sync.h"
#include "daprobe.h"

#include "binproto.h"
#include "daforkexec.h"
#include "dastdout.h"
#include "common_probe_init.h"
#include "real_functions.h"

#define UDS_NAME				"/tmp/da.socket"
#define TIMERFD_INTERVAL		100000000		// 0.1 sec

pid_t gPid = -1;
__thread pid_t		gTid = -1;

int			g_timerfd = 0;
long		g_total_alloc_size = 0;
pthread_t	g_recvthread_id;

int log_fd = 0;

bool printLog(log_t* log, int msgType);

/******************************************************************************
 * internal functions
   (this means that these functions do not need to set enter/exit flag)
 ******************************************************************************/

// runtime configure the probe option
static void _configure(char* configstr)
{
	gTraceInfo.optionflag = atoll(configstr);

	if isOptionEnabled(OPT_SNAPSHOT)
		SCREENSHOT_SET();
	else
		SCREENSHOT_UNSET();
	PRINTMSG("configure in probe : %s, %llx\n", configstr, gTraceInfo.optionflag);
}

void application_exit()
{
	pid_t gpid;
	size_t readed = 0;
	FILE *f = NULL;
	char buf[MAX_PATH_LENGTH];
	const char manager_name[] = "da_manager";

	/* TODO think of another way for correct app termination */

	gpid = getpgrp();
	/* check for parent */
	snprintf(buf, sizeof(buf), "/proc/%d/cmdline", gpid);
	f = fopen(buf, "r");
	if (f != NULL) {
		readed = fread(buf, strlen(manager_name), 1, f);
		fclose(f);
		if ((readed != 0) && (!memcmp(buf, manager_name, strlen(manager_name)))) {
			/* Luke, I am your father
			 * da_manager is our parent
			 * looks like we are common applicaton
			 */
			PRINTMSG("App termination: EXIT(0)");
			exit(0);
		}
	}

	/* da_manager is not our father
	 * we are native app or already launched.
	 * Will be troubles up there if we are common already launched app!!!
	 * Kill yourself!!
	 */
	PRINTMSG("App termination: kill all process group");
	killpg(gpid, SIGKILL);
}

// create socket to daemon and connect
#define MSG_CONFIG_RECV 0x01
static int create_socket(void)
{
	char strerr_buf[MAX_PATH_LENGTH];
	ssize_t recvlen;
	int client_len, ret = 0;
	struct sockaddr_un client_addr;
	log_t log;

	gTraceInfo.socket.daemonSock =
	  socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (gTraceInfo.socket.daemonSock != -1)	{
		memset(&client_addr, '\0', sizeof(client_addr));
		client_addr.sun_family = AF_UNIX;
		snprintf(client_addr.sun_path, sizeof(UDS_NAME), "%s", UDS_NAME);

		client_len = sizeof(client_addr);
		if (connect(gTraceInfo.socket.daemonSock,
			    (struct sockaddr *)&client_addr, client_len) >= 0)
		{
			char buf[64];
			int recved = 0;

			/* send pid and ppid to manager */
			snprintf(buf, sizeof(buf), "%d %d", getpid(), getppid());
			print_log_str(APP_MSG_PID, buf);

			/* we need recv this messages right now! */
			while ((recved & MSG_CONFIG_RECV) == 0)
			{
				PRINTMSG("wait incoming message %d\n",
					 gTraceInfo.socket.daemonSock);
				/* recv header */
				recvlen = recv(gTraceInfo.socket.daemonSock, &log,
					       sizeof(log.type) + sizeof(log.length),
					       MSG_WAITALL);

				fprintf(stderr, "recv %d\n", recvlen);
				if (recvlen > 0) {/* recv succeed */
					char *data_buf = NULL;

					data_buf = real_malloc(log.length);

					if (data_buf == NULL) {
						PRINTERR("cannot allocate buf to recv msg");
						break;
					}

					recvlen = recv(gTraceInfo.socket.daemonSock, data_buf,
						       log.length, MSG_WAITALL);

					if (recvlen != log.length) {
						PRINTERR("Can not get data from daemon sock\n");
						goto free_data_buf;
					}

					if (log.type == APP_MSG_CONFIG) {
						PRINTMSG("APP_MSG_CONFIG");
						_configure(data_buf);
						recved |= MSG_CONFIG_RECV;
					} else {
						// unexpected case
						PRINTERR("unknown message! %d", log.type);
					}

free_data_buf:
					if (data_buf != NULL)
						free(data_buf);

				} else if (recvlen < 0) {
					close(gTraceInfo.socket.daemonSock);
					gTraceInfo.socket.daemonSock = -1;
					PRINTERR("recv failed with error(%d)\n",
						 recvlen);
					ret = -1;
					application_exit();
					break;
				} else {
					/* closed by other peer */
					close(gTraceInfo.socket.daemonSock);
					gTraceInfo.socket.daemonSock = -1;
					PRINTERR("closed by other peer\n");
					ret = -1;
					application_exit();
					break;
				}

			}

			PRINTMSG("create_socket connect() success\n");
		} else {
			close(gTraceInfo.socket.daemonSock);
			gTraceInfo.socket.daemonSock = -1;
			strerror_r(errno, strerr_buf, sizeof(strerr_buf));
			PRINTERR("cannot connect to da_manager. err <%s>\n",
				 strerr_buf);
			ret = -1;
		}
	} else {
		strerror_r(errno, strerr_buf, sizeof(strerr_buf));
		PRINTERR("cannot create socket. err <%s>\n", strerr_buf);
		ret = -1;
	}

	PRINTMSG("socket create done with result = %d", ret);
	return ret;
}


void reset_pid_tid()
{
	gPid = -1;
	gTid = -1;
}

// return current process id
pid_t _getpid()
{
	if (gPid == -1)
		gPid = getpid();
	return gPid;
}

// return current thread id
pid_t _gettid()
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
	char *data_buf = NULL;
	sigset_t profsigmask;

	if(gTraceInfo.socket.daemonSock == -1)
		return NULL;

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
				log.length = snprintf(log.data, sizeof(log.data), "%ld", g_total_alloc_size) + 1;
				printLog(&log, APP_MSG_ALLOC);
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

				if(log.length > 0) {
					data_buf = real_malloc(log.length);
					if (data_buf == NULL) {
						PRINTERR("cannot allocate buf to recv msg");
						break;
					}
					recvlen = recv(gTraceInfo.socket.daemonSock, data_buf,
						log.length, MSG_WAITALL);
					if (recvlen != log.length) {
						PRINTERR("Can not recv data from\n");
						goto free_data_buf;
					}
				}

				if (log.type == APP_MSG_CAPTURE_SCREEN) {
					captureScreen();
				} else if (log.type == APP_MSG_CONFIG) {
					_configure(data_buf);
				} else if ((log.type == APP_MSG_STOP) ||
					   (log.type == APP_MSG_STOP_WITHOUT_KILL)) {
					PRINTMSG("APP_MSG_STOP (%d)", log.type);
					if (data_buf) {
						free(data_buf);
						data_buf = NULL;
					}
					if (log.type != APP_MSG_STOP_WITHOUT_KILL)
						application_exit();
					break;
				} else if(log.type == APP_MSG_GET_UI_HIERARCHY) {
					PRINTMSG("APP_MSG_GET_UI_HIERARCHY");
					// do nothing
					continue;
				} else if(log.type == APP_MSG_GET_UI_SCREENSHOT) {
					PRINTMSG("APP_MSG_GET_UI_SCREENSHOT");
					// do nothing
					continue;
				} else if(log.type == APP_MSG_GET_UI_HIERARCHY_CANCEL) {
					PRINTMSG("APP_MSG_GET_UI_HIERARCHY_CANCEL");
					// do nothing
					continue;
				} else {
					PRINTERR("recv unknown message. id = (%d)", log.type);
				}

free_data_buf:
				if (data_buf) {
					free(data_buf);
					data_buf = NULL;
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
				PRINTERR("recv failed in recv thread with error(%d)", recvlen);
				continue;
			}
		}
		else	// unknown case
		{
			continue;
		}
	}

	/* release buffer */
	if (data_buf)
		free(data_buf);

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

static int create_recv_thread()
{
	int err = pthread_create(&g_recvthread_id, NULL, recvThread, NULL);

	if (err)
		PRINTMSG("failed to crate recv thread\n");

	return err;
}

void _init_(void)
{
	/* redirect stderr and stdout.*/
	/* if there is no std - print call will crash app */
	__redirect_std();

	init_exec_fork();
	initialize_hash_table();

	// create socket for communication with da_daemon
	if (create_socket() == 0) {
		g_timerfd = init_timerfd();
		create_recv_thread();
		update_heap_memory_size(true, 0);
	}


	PRINTMSG("dynamic analyzer probe helper so loading... pid[%d]\n",
		 getpid());

	gTraceInfo.init_complete = 1;
}

void __attribute__((constructor)) _init_probe()
{

	if (_init_real_functions()) {
		PRINTERR("cannot init real functions\n");
		exit(1);
	}
	rtdl_next_set_once(real_malloc, "malloc");

	/* init library */
	_init_();

	/* init gl functions */
	__init_gl_functions__();

	PRINTMSG("<-lib construnctor");
}

void _uninit_(void)
{
	int i;

	gTraceInfo.init_complete = -1;
	PRINTMSG("dynamic analyzer probe helper so unloading... pid[%d]\n",
		 getpid());

	remove_all_glist();

	// close timerfd
	if(g_timerfd > 0)
		close(g_timerfd);

	// close socket
	if(gTraceInfo.socket.daemonSock != -1)
	{
		print_log_str(APP_MSG_TERMINATE, NULL);
		close(gTraceInfo.socket.daemonSock);
		gTraceInfo.socket.daemonSock = -1;
	}

	finalize_event();

	finalize_hash_table();

	for(i = 0; i < NUM_ORIGINAL_LIBRARY; i++)
	{
		if(lib_handle[i] != NULL)
		{
			dlclose(lib_handle[i]);
		}
	}
}

void __attribute__((destructor)) _fini_probe()
{
	PRINTMSG("->lib destructor. pid[%d]\n", getpid());
	_uninit_();
}


/**************************************************************************
 * Helper APIs
 **************************************************************************/

/************************************************************************
 * manipulate and print log functions
 ************************************************************************/
const char *msg_code_to_srt(enum AppMessageType type)
{
	switch (type) {
		case APP_MSG_MSG:
			return "[INF]";
		case APP_MSG_ERROR:
			return "[ERR]";
		case APP_MSG_WARNING:
			return "[WRN]";
		default:
			return "[???]";
	}
}

bool printLog(log_t *log, int msgType)
{
	ssize_t res, len;
	if(unlikely(gTraceInfo.socket.daemonSock == -1))
		return false;

	if(unlikely(log == NULL))
		return false;

	log->type = msgType;
	len = sizeof(log->type) + sizeof(log->length) + log->length;

	real_pthread_mutex_lock(&(gTraceInfo.socket.sockMutex));
	res = send(gTraceInfo.socket.daemonSock, log, len, MSG_NOSIGNAL);
	real_pthread_mutex_unlock(&(gTraceInfo.socket.sockMutex));

	return (res == len);
}

/*
 * Debug function for ld preloaded library
 * msgType - message type (APP_MSG_TERMINATE, APP_MSG_PID)
 * str     - string send to host. may be NULL
 * return  - true on success. false on fail.
 */

bool print_log_str(int msgType, char *str)
{
	log_t log;
	ssize_t res, len;

	/* Check connection status */
	if(unlikely(gTraceInfo.socket.daemonSock == -1))
		return false;

	/* fill message */
	log.type = msgType;
	if (str != NULL)
		log.length = snprintf(log.data, sizeof(log.data), str);
	else
		log.length = 0;

	/* calculate message length */
	len = sizeof(log.type) + sizeof(log.length) + log.length;

	/* lock socket and send */
	real_pthread_mutex_lock(&(gTraceInfo.socket.sockMutex));
	res = send(gTraceInfo.socket.daemonSock, &log, len, MSG_NOSIGNAL);
	real_pthread_mutex_unlock(&(gTraceInfo.socket.sockMutex));

	return (res == len);
}

/*
 * Additional debug function for ld preloaded library can be used in defines
 * msgType   - message type (APP_MSG_TERMINATE, APP_MSG_PID)
 * func_name - call function name
 * line      - call line number
 * ...       - format and arguments like for sprintf function
 * return    - true on success. false on fail.
 */
bool print_log_fmt(int msgType, const char *func_name, int line, ...)
{
	ssize_t res = 0, len = 0;
	char *fmt, *p;
	int n;
	log_t log;
	va_list ap;

	/* Check connection status */

	/* fill message */
	log.type = msgType;

	p = log.data;
	n = snprintf(p, sizeof(log.data), "[%05d:%05d]%s:%d)", _getpid(), _gettid(),  func_name, line);
	p += n;

	/* extract params */
	va_start(ap, line);
	fmt = va_arg(ap, char *);
	if (fmt != NULL) {
		if (strchr(fmt, '%') == NULL)
			n += snprintf(p, sizeof(log.data) - n, "%s", fmt);
		else
			n += vsnprintf(p, sizeof(log.data) - n, fmt, ap);
	}
	va_end(ap);

	/* check printed symbols count */
	if (n > -1 && n < (int)sizeof(log.data)) {
		log.length = n;
	} else {
		fprintf(stderr, "Log pack error\n");
		log.length = 0;
	}

	/* calculate message length */
	len = sizeof(log.type) + sizeof(log.length) + log.length;

	/* lock socket and send */
	real_pthread_mutex_lock(&(gTraceInfo.socket.sockMutex));

	if(unlikely(gTraceInfo.socket.daemonSock != -1)) {
		res = send(gTraceInfo.socket.daemonSock, &log, len,
			   MSG_DONTWAIT | MSG_NOSIGNAL);
	} else {
		/* if socket is not connected, out to stderr */
		fprintf(stderr, "%s %s\n", msg_code_to_srt(msgType), log.data);
		fflush(stderr);
	}

	real_pthread_mutex_unlock(&(gTraceInfo.socket.sockMutex));

	/* return result */
	return (res == len);
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

int preBlockBegin(void)
{
	if(gTraceInfo.init_complete <= 0)
		return 0;

	return 1;
}

/*************************************************************************
 * helper info getter functions
 *************************************************************************/

unsigned int getCurrentEventIndex()
{
	return gTraceInfo.index.eventIndex;
}

void inc_current_event_index(void)
{
	__sync_add_and_fetch(&gTraceInfo.index.eventIndex, 1);
}

uint64_t get_current_nsec(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (uint64_t)tv.tv_sec * 1000 * 1000 * 1000 + tv.tv_usec * 1000;
}

unsigned long get_caller_addr(unsigned long caller_addr)
{
	return caller_addr;
}

unsigned char get_call_type(unsigned char call_type)
{
	return call_type;
}

void write_msg(unsigned long __unused msg_buf,
	       size_t __unused len,
	       unsigned long __unused call_type_p,
	       unsigned long __unused caller_p,
	       unsigned long __unused caller_address)
{
	return;
}

/************************************************************************
 * probe functions
 ************************************************************************/
// update heap memory size through socket
// return 0 if size is updated through socket
// return 1 if size is updated into global variable
int update_heap_memory_size(bool isAdd, size_t size)
{
	if(isAdd)
	{
		__sync_add_and_fetch(&g_total_alloc_size, (long)size);
	}
	else
	{
		__sync_sub_and_fetch(&g_total_alloc_size, (long)size);
	}

	return 0;
}

/* rtdl_next */
void *rtdl_next(const char *symname)
{
	void *symbol;

	symbol = dlsym(RTLD_NEXT, symname);
	if (symbol == NULL || dlerror() != NULL) {
		PRINTERR("dlsym failed <%s>\n", symname);
		exit(41);
	}

	return symbol;
}

void *rtld_default(const char *symname)
{
	void *symbol;

	symbol = dlsym(RTLD_DEFAULT, symname);
	if (symbol == NULL || dlerror() != NULL) {
		PRINTERR("dlsym default failed <%s>\n", symname);
		exit(41);
	}

	return symbol;
}
