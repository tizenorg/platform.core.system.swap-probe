/*
 *  DA probe
 *
 * Copyright 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Woojin Jung <woojin2.jung@samsung.com>
 * Jaewon Lim <jaewon81.lim@samsung.com>
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

#include <pthread.h>
#include <errno.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dahelper.h"
#include "da_thread.h"

#include "binproto.h"

typedef struct thread_routine_call_t {
	void *(*thread_routine)(void *);
	void *argument;
} thread_routine_call;

static enum DaOptions _sopt = OPT_THREAD;

// called when pthread_exit, pthread_cancel is called
void _da_cleanup_handler(void *data)
{
	pthread_t pSelf;

	probeInfo_t	probeInfo; log_t log;
	INIT_LOG;
	
	PRE_UNCONDITIONAL_BLOCK_BEGIN();
	APPEND_LOG_BASIC(LC_THREAD);
	// input,ret,PCAddr,errno,InternalCall,CallerPCAddr
	log.length += sprintf(log.data + log.length, "`,`,`,0`,0`,1`,0");
	pSelf = pthread_self();
	log.length += sprintf(log.data + log.length, "`,%lu`,`,%d`,%d",
			pSelf, THREAD_PTHREAD, THREAD_API_INTERNAL_STOP);
	APPEND_LOG_NULL_CALLSTACK();
	printLog(&log, MSG_LOG);
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "p", data);
	PACK_COMMON_END(0, 0, 1);
	PACK_THREAD(pSelf, THREAD_PTHREAD, THREAD_API_INTERNAL_STOP);
	FLUSH_LOCAL_BUF();
	
	PRE_UNCONDITIONAL_BLOCK_END();

	return;
}

void *_da_ThreadProc(void *params)
{
	void *ret;
	thread_routine_call *ptrc;
	ptrc = (thread_routine_call *) params;
	pthread_t pSelf;

	probeInfo_t	probeInfo; log_t log;

	// send INTERNAL_START log
	INIT_LOG;

	PRE_UNCONDITIONAL_BLOCK_BEGIN();
	APPEND_LOG_BASIC(LC_THREAD);
	// input,ret,PCAddr,errno,InternalCall,CallerPCAddr
	log.length += sprintf(log.data + log.length, "`,`,`,0`,0`,1`,0");
	pSelf = pthread_self();
	log.length += sprintf(log.data + log.length, "`,%lu`,`,%d`,%d",
			pSelf, THREAD_PTHREAD, THREAD_API_INTERNAL_START);
	APPEND_LOG_NULL_CALLSTACK();
	printLog(&log, MSG_LOG);
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "p", params);
	PACK_COMMON_END(0, 0, 1);
	PACK_THREAD(pSelf, THREAD_PTHREAD, THREAD_API_INTERNAL_START);
	FLUSH_LOCAL_BUF();
	
	PRE_UNCONDITIONAL_BLOCK_END();

	pthread_cleanup_push(_da_cleanup_handler, NULL);
	// call user-defined thread routine
	ret = ptrc->thread_routine(ptrc->argument);
	pthread_cleanup_pop(0);

	// send INTERNAL_STOP log
	INIT_LOG;

	PRE_UNCONDITIONAL_BLOCK_BEGIN();
	APPEND_LOG_BASIC(LC_THREAD);
	// input,ret,PCAddr,errno,InternalCall,CallerPCAddr
	log.length += sprintf(log.data + log.length, "`,`,`,0`,0`,1`,0");
	pSelf = pthread_self();
	log.length += sprintf(log.data + log.length, "`,%lu`,`,%d`,%d",
			pSelf, THREAD_PTHREAD, THREAD_API_INTERNAL_STOP);
	APPEND_LOG_NULL_CALLSTACK();
	printLog(&log, MSG_LOG);
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "p", params);
	PACK_COMMON_END(ret, 0, 1);
	PACK_THREAD(pSelf, THREAD_PTHREAD, THREAD_API_INTERNAL_STOP);
	FLUSH_LOCAL_BUF();
	
	PRE_UNCONDITIONAL_BLOCK_END();
	
	free(ptrc);
	return ret;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
		void *(*start_routine) (void*), void *arg)
{
	static int (*pthread_createp)(pthread_t *thread,
			const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

	BEFORE_ORIGINAL_THREAD(pthread_create, LIBPTHREAD);

	if(blockresult)
	{
		probeBlockStart();	
		thread_routine_call *ptrc = 
			(thread_routine_call *) malloc(sizeof(thread_routine_call));
		ptrc->thread_routine = start_routine;
		ptrc->argument = arg;
		probeBlockEnd();

		ret = pthread_createp(thread, attr, _da_ThreadProc, (void *) ptrc);
	}
	else // when pthread_create is called inside probe so (ex. custom chart, sampling thread)
	{
		ret = pthread_createp(thread, attr, start_routine, arg);
	}

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, *thread, THREAD_API_START, 
			"%p,%p,%p,%p", thread, attr, start_routine, arg);

	AFTER_PACK_ORIGINAL_THREAD(ret, *thread, THREAD_API_START, 
			"pppp", thread, attr, start_routine, arg);

	return ret;
}

int pthread_join(pthread_t thread, void **retval)
{
	static int (*pthread_joinp)(pthread_t thread, void **retval);

	DECLARE_VARIABLE_STANDARD; log_t log;
	GET_REAL_FUNC(pthread_join, LIBPTHREAD);

	PRE_PROBEBLOCK_BEGIN();
	// send WAIT_START log
	INIT_LOG;
	APPEND_LOG_BASIC(LC_THREAD);
	APPEND_LOG_INPUT("%lu,%p", thread, retval);
	//ret,PCAddr,errno,InternalCall
	log.length += sprintf(log.data + log.length, "`,`,0`,0`,%d", blockresult);
	APPEND_LOG_CALLER();
	log.length += sprintf(log.data + log.length, "`,%lu`,`,%d`,%d", thread, 
			THREAD_PTHREAD, THREAD_API_WAIT_START);
	APPEND_LOG_NULL_CALLSTACK();
	printLog(&log, MSG_LOG);
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "xp", thread, retval);
	PACK_COMMON_END(0, 0, blockresult);
	PACK_THREAD(thread, THREAD_PTHREAD, THREAD_API_WAIT_START);
	FLUSH_LOCAL_BUF();
	
	PRE_PROBEBLOCK_END();

	ret = pthread_joinp(thread, retval);

	// send WAIT_END log
	newerrno = errno;
	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);
		INIT_LOG;
		APPEND_LOG_BASIC(LC_THREAD);
		APPEND_LOG_INPUT("%lu,%p", thread, retval);
		APPEND_LOG_RESULT(VT_INT, ret);
		APPEND_LOG_CALLER();

		POST_PROBEBLOCK_MIDDLE_THREAD(thread, THREAD_API_WAIT_END);
		POST_PROBEBLOCK_CALLSTACK();
		printLog(&log, MSG_LOG);
		
		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "xp", thread, retval);
		PACK_COMMON_END(ret, errno, blockresult);
		PACK_THREAD(thread, THREAD_PTHREAD, THREAD_API_WAIT_END);
		FLUSH_LOCAL_BUF();
	
		postBlockEnd();
	}
	
	return ret;	
}

void pthread_exit(void *retval)
{
	pthread_t pSelf;
	static void (*pthread_exitp)(void *retval) __attribute__((noreturn));

	DECLARE_VARIABLE_STANDARD; log_t log;
	GET_REAL_FUNC(pthread_exit, LIBPTHREAD);

	PRE_PROBEBLOCK_BEGIN();
	newerrno = 0;
	INIT_LOG;
	APPEND_LOG_BASIC(LC_THREAD);
	APPEND_LOG_INPUT("%p", retval);
	//ret,PCAddr,errno,InternalCall
	log.length += sprintf(log.data + log.length, "`,`,0`,0`,%d", blockresult);
	APPEND_LOG_CALLER();
	pSelf = pthread_self();
	log.length += sprintf(log.data + log.length, "`,%lu`,`,%d`,%d",
			pSelf, THREAD_PTHREAD, THREAD_API_EXIT);
	APPEND_LOG_NULL_CALLSTACK();
	printLog(&log, MSG_LOG);
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "p", retval);
	PACK_COMMON_END(0, 0, blockresult);
	PACK_THREAD(pSelf, THREAD_PTHREAD, THREAD_API_EXIT);
	FLUSH_LOCAL_BUF();
	
	PRE_PROBEBLOCK_END();

	pthread_exitp(retval);
}

int pthread_cancel(pthread_t thread)
{
	static int (*pthread_cancelp)(pthread_t thread);

	BEFORE_ORIGINAL_THREAD(pthread_cancel, LIBPTHREAD);

	ret = pthread_cancelp(thread);
	
	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_STOP, "%lu", thread);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_STOP, "x", thread);

	return ret;
}

int pthread_detach(pthread_t thread)
{
	static int (*pthread_detachp)(pthread_t thread);

	BEFORE_ORIGINAL_THREAD(pthread_detach, LIBPTHREAD);

	ret = pthread_detachp(thread);
	
	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, "%lu", thread);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, "x", thread);

	return ret;
}

pthread_t pthread_self(void)
{
	pthread_t ret_pthr;
	static pthread_t (*pthread_selfp)(void);

	BEFORE_ORIGINAL_THREAD(pthread_self, LIBPTHREAD);

	ret_pthr = pthread_selfp();
	
	newerrno = errno;
	if(postBlockBegin(blockresult)) {
		INIT_LOG;
		APPEND_LOG_BASIC(LC_THREAD);
		log.length += sprintf(log.data + log.length, "`,");
		APPEND_LOG_RESULT(VT_ULONG, ret_pthr);
		APPEND_LOG_CALLER();

		POST_PROBEBLOCK_MIDDLE_THREAD(ret_pthr, THREAD_API_OTHER);
		POST_PROBEBLOCK_CALLSTACK();
		printLog(&log, MSG_LOG);

		AFTER_PACK_ORIGINAL_THREAD(ret_pthr, ret_pthr, THREAD_API_OTHER, "", 0);

		postBlockEnd();
	}

	return ret_pthr;
}

int pthread_equal(pthread_t t1, pthread_t t2)
{
	static int (*pthread_equalp)(pthread_t t1, pthread_t t2);

	BEFORE_ORIGINAL_THREAD(pthread_equal, LIBPTHREAD);

	ret = pthread_equalp(t1, t2);
	
	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, t1, THREAD_API_OTHER, "%lu,%lu", t1, t2);

	AFTER_PACK_ORIGINAL_THREAD(ret, t1, THREAD_API_OTHER, "xx", t1, t2);

	return ret;
}

int pthread_setcancelstate(int state, int *oldstate)
{
	pthread_t pSelf;
	static int (*pthread_setcancelstatep)(int state, int *oldstate);

	BEFORE_ORIGINAL_THREAD(pthread_setcancelstate, LIBPTHREAD);

	pSelf = pthread_self();
	ret = pthread_setcancelstatep(state, oldstate);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, pSelf, THREAD_API_OTHER, 
			"%d,%p", state, oldstate);

	AFTER_PACK_ORIGINAL_THREAD(ret, pSelf, THREAD_API_OTHER, 
			"dp", state, oldstate);

	return ret;
}

int pthread_setcanceltype(int type, int *oldtype)
{
	pthread_t pSelf;
	static int (*pthread_setcanceltypep)(int type, int *oldtype);

	BEFORE_ORIGINAL_THREAD(pthread_setcanceltype, LIBPTHREAD);

	pSelf = pthread_self();
	ret = pthread_setcanceltypep(type, oldtype);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, pSelf, THREAD_API_OTHER, 
			"%d,%p", type, oldtype);

	AFTER_PACK_ORIGINAL_THREAD(ret, pSelf, THREAD_API_OTHER, 
			"dp", type, oldtype);

	return ret;
}

int pthread_attr_init(pthread_attr_t *attr)
{
	pthread_t thread = 0;
	static int (*pthread_attr_initp)(pthread_attr_t *attr);

	BEFORE_ORIGINAL_THREAD(pthread_attr_init, LIBPTHREAD);

	ret = pthread_attr_initp(attr);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, "%p", attr);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, "p", attr);

	return ret;
}

int pthread_attr_destroy(pthread_attr_t *attr) 
{
	pthread_t thread = 0;
	static int (*pthread_attr_destroyp)(pthread_attr_t *attr);

	BEFORE_ORIGINAL_THREAD(pthread_attr_destroy, LIBPTHREAD);

	ret = pthread_attr_destroyp(attr);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, "%p", attr);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, "p", attr);

	return ret;
}

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getdetachstatep)(const pthread_attr_t *attr,
			int *detachstate);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getdetachstate, LIBPTHREAD);

	ret = pthread_attr_getdetachstatep(attr, detachstate);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, detachstate);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pp", attr, detachstate);

	return ret;
}

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setdetachstatep)(pthread_attr_t *attr, 
			int detachstate);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setdetachstate, LIBPTHREAD);

	ret = pthread_attr_setdetachstatep(attr, detachstate);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%d", attr, detachstate);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pd", attr, detachstate);

	return ret;
}

int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getstacksizep)(const pthread_attr_t *attr,
			size_t *stacksize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getstacksize, LIBPTHREAD);

	ret = pthread_attr_getstacksizep(attr, stacksize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, stacksize);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pp", attr, stacksize);

	return ret;
}

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setstacksizep)(pthread_attr_t *attr, 
			size_t stacksize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setstacksize, LIBPTHREAD);

	ret = pthread_attr_setstacksizep(attr, stacksize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%u", attr, stacksize);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"px", attr, stacksize);

	return ret;
}

int pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getstackaddrp)(const pthread_attr_t *attr,
			void **stackaddr);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getstackaddr, LIBPTHREAD);

	ret = pthread_attr_getstackaddrp(attr, stackaddr);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, stackaddr);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pp", attr, stackaddr);

	return ret;
}

int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setstackaddrp)(pthread_attr_t *attr, 
			void *stackaddr);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setstackaddr, LIBPTHREAD);

	ret = pthread_attr_setstackaddrp(attr, stackaddr);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, stackaddr);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pp", attr, stackaddr);

	return ret;
}

int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getinheritschedp)(const pthread_attr_t *attr,
			int *inheritsched);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getinheritsched, LIBPTHREAD);

	ret = pthread_attr_getinheritschedp(attr, inheritsched);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, inheritsched);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pp", attr, inheritsched);

	return ret;
}

int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setinheritschedp)(pthread_attr_t *attr, 
			int inheritsched);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setinheritsched, LIBPTHREAD);

	ret = pthread_attr_setinheritschedp(attr, inheritsched);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%d", attr, inheritsched);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pd", attr, inheritsched);

	return ret;
}

int pthread_attr_getschedparam(const pthread_attr_t *attr,
		struct sched_param *param)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getschedparamp)(const pthread_attr_t *attr,
			struct sched_param *param);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getschedparam, LIBPTHREAD);

	ret = pthread_attr_getschedparamp(attr, param);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, param);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pp", attr, param);

	return ret;
}

int pthread_attr_setschedparam(pthread_attr_t *attr, 
		const struct sched_param *param)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setschedparamp)(pthread_attr_t *attr,
			const struct sched_param *param);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setschedparam, LIBPTHREAD);

	ret = pthread_attr_setschedparamp(attr, param);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, param);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pp", attr, param);

	return ret;
}

int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getschedpolicyp)(const pthread_attr_t *attr,
			int *policy);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getschedpolicy, LIBPTHREAD);

	ret = pthread_attr_getschedpolicyp(attr, policy);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, policy);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pp", attr, policy);

	return ret;
}

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setschedpolicyp)(pthread_attr_t *attr, 
			int policy);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setschedpolicy, LIBPTHREAD);

	ret = pthread_attr_setschedpolicyp(attr, policy);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%d", attr, policy);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pd", attr, policy);

	return ret;
}

int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getguardsizep)(const pthread_attr_t *attr,
			size_t *guardsize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getguardsize, LIBPTHREAD);

	ret = pthread_attr_getguardsizep(attr, guardsize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, guardsize);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pp", attr, guardsize);

	return ret;
}

int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setguardsizep)(pthread_attr_t *attr, 
			size_t guardsize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setguardsize, LIBPTHREAD);

	ret = pthread_attr_setguardsizep(attr, guardsize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%u", attr, guardsize);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"px", attr, guardsize);

	return ret;
}

int pthread_attr_getscope(const pthread_attr_t *attr, int *contentionscope)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getscopep)(const pthread_attr_t *attr,
			int *contentionscope);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getscope, LIBPTHREAD);

	ret = pthread_attr_getscopep(attr, contentionscope);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, contentionscope);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pp", attr, contentionscope);

	return ret;
}

int pthread_attr_setscope(pthread_attr_t *attr, int contentionscope)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setscopep)(pthread_attr_t *attr, 
			int contentionscope);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setscope, LIBPTHREAD);

	ret = pthread_attr_setscopep(attr, contentionscope);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%d", attr, contentionscope);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"pd", attr, contentionscope);

	return ret;
}

int pthread_attr_getstack(const pthread_attr_t *attr, 
		void **stackaddr, size_t *stacksize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getstackp)(const pthread_attr_t *attr,
			void **stackaddr, size_t *stacksize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getstack, LIBPTHREAD);

	ret = pthread_attr_getstackp(attr, stackaddr, stacksize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p,%p", attr, stackaddr, stacksize);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"ppp", attr, stackaddr, stacksize);

	return ret;
}

int pthread_attr_setstack(pthread_attr_t *attr,
		void *stackaddr, size_t stacksize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setstackp)(pthread_attr_t *attr,
			void *stackaddr, size_t stacksize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setstack, LIBPTHREAD);

	ret = pthread_attr_setstackp(attr, stackaddr, stacksize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p,%u", attr, stackaddr, stacksize);

	AFTER_PACK_ORIGINAL_THREAD(ret, thread, THREAD_API_OTHER, 
			"ppx", attr, stackaddr, stacksize);

	return ret;
}

/*
void pthread_testcancel(void);

int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
int pthread_key_delete(pthread_key_t key);

int pthread_getconcurrency(void);
int pthread_setconcurrency(int new_level);
int pthread_getcpuclockid(pthread_t thread_id, clockid_t *clock_id);
int pthread_getschedparam(pthread_t thread, int *policy,
		struct sched_param *param);
int pthread_setschedparam(pthread_t thread, int policy,
		const struct sched_param *param);
int pthread_setschedprio(pthread_t thread, int prio);
void *pthread_getspecific(pthread_key_t key);
int pthread_setspecific(pthread_key_t key, const void *value);

int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));

int pthread_atfork(void (*prepare)(void), void (*parent)(void),
		void (*child)(void));
*/

