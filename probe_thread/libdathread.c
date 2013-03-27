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

typedef struct thread_routine_call_t {
	void *(*thread_routine)(void *);
	void *argument;
} thread_routine_call;

// called when pthread_exit, pthread_cancel is called
void _da_cleanup_handler(void *data)
{
	pthread_t pSelf;

	DECLARE_COMMON_VARIABLE;
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
	PRE_UNCONDITIONAL_BLOCK_END();

	return;
}

void *_da_ThreadProc(void *params)
{
	void *ret;
	thread_routine_call *ptrc;
	ptrc = (thread_routine_call *) params;
	pthread_t pSelf;

	DECLARE_COMMON_VARIABLE;

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
	PRE_UNCONDITIONAL_BLOCK_END();
	
	free(ptrc);
	return ret;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
		void *(*start_routine) (void*), void *arg)
{
	static int (*pthread_createp)(pthread_t *thread,
			const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);

	BEFORE_ORIGINAL_THREAD(pthread_create, libpthread.so.0);

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

	return ret;
}

int pthread_join(pthread_t thread, void **retval)
{
	static int (*pthread_joinp)(pthread_t thread, void **retval);

	DECLARE_VARIABLE_STANDARD;
	GET_REAL_FUNC(pthread_join, libpthread.so.0);

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
		postBlockEnd();
	}
	
	return ret;	
}

void pthread_exit(void *retval)
{
	pthread_t pSelf;
	static void (*pthread_exitp)(void *retval) __attribute__((noreturn));

	DECLARE_VARIABLE_STANDARD;
	GET_REAL_FUNC(pthread_exit, libpthread.so.0);

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
	PRE_PROBEBLOCK_END();

	pthread_exitp(retval);
}

int pthread_cancel(pthread_t thread)
{
	static int (*pthread_cancelp)(pthread_t thread);

	BEFORE_ORIGINAL_THREAD(pthread_cancel, libpthread.so.0);

	ret = pthread_cancelp(thread);
	
	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_STOP, "%lu", thread);

	return ret;
}

int pthread_detach(pthread_t thread)
{
	static int (*pthread_detachp)(pthread_t thread);

	BEFORE_ORIGINAL_THREAD(pthread_detach, libpthread.so.0);

	ret = pthread_detachp(thread);
	
	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, "%lu", thread);

	return ret;
}

pthread_t pthread_self(void)
{
	pthread_t ret_pthr;
	static pthread_t (*pthread_selfp)(void);

	BEFORE_ORIGINAL_THREAD(pthread_self, libpthread.so.0);

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
		postBlockEnd();
	}

	return ret_pthr;
}

int pthread_equal(pthread_t t1, pthread_t t2)
{
	static int (*pthread_equalp)(pthread_t t1, pthread_t t2);

	BEFORE_ORIGINAL_THREAD(pthread_equal, libpthread.so.0);

	ret = pthread_equalp(t1, t2);
	
	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, t1, THREAD_API_OTHER, "%lu,%lu", t1, t2);

	return ret;
}

int pthread_setcancelstate(int state, int *oldstate)
{
	pthread_t pSelf;
	static int (*pthread_setcancelstatep)(int state, int *oldstate);

	BEFORE_ORIGINAL_THREAD(pthread_setcancelstate, libpthread.so.0);

	pSelf = pthread_self();
	ret = pthread_setcancelstatep(state, oldstate);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, pSelf, THREAD_API_OTHER, 
			"%d,%p", state, oldstate); 

	return ret;
}

int pthread_setcanceltype(int type, int *oldtype)
{
	pthread_t pSelf;
	static int (*pthread_setcanceltypep)(int type, int *oldtype);

	BEFORE_ORIGINAL_THREAD(pthread_setcanceltype, libpthread.so.0);

	pSelf = pthread_self();
	ret = pthread_setcanceltypep(type, oldtype);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, pSelf, THREAD_API_OTHER, 
			"%d,%p", type, oldtype); 

	return ret;
}

int pthread_attr_init(pthread_attr_t *attr)
{
	pthread_t thread = 0;
	static int (*pthread_attr_initp)(pthread_attr_t *attr);

	BEFORE_ORIGINAL_THREAD(pthread_attr_init, libpthread.so.0);

	ret = pthread_attr_initp(attr);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, "%p", attr);

	return ret;
}

int pthread_attr_destroy(pthread_attr_t *attr) 
{
	pthread_t thread = 0;
	static int (*pthread_attr_destroyp)(pthread_attr_t *attr);

	BEFORE_ORIGINAL_THREAD(pthread_attr_destroy, libpthread.so.0);

	ret = pthread_attr_destroyp(attr);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, "%p", attr);

	return ret;
}

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getdetachstatep)(const pthread_attr_t *attr,
			int *detachstate);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getdetachstate, libpthread.so.0);

	ret = pthread_attr_getdetachstatep(attr, detachstate);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, detachstate);

	return ret;
}

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setdetachstatep)(pthread_attr_t *attr, 
			int detachstate);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setdetachstate, libpthread.so.0);

	ret = pthread_attr_setdetachstatep(attr, detachstate);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%d", attr, detachstate);

	return ret;
}

int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getstacksizep)(const pthread_attr_t *attr,
			size_t *stacksize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getstacksize, libpthread.so.0);

	ret = pthread_attr_getstacksizep(attr, stacksize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, stacksize);

	return ret;
}

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setstacksizep)(pthread_attr_t *attr, 
			size_t stacksize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setstacksize, libpthread.so.0);

	ret = pthread_attr_setstacksizep(attr, stacksize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%u", attr, stacksize);

	return ret;
}

int pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getstackaddrp)(const pthread_attr_t *attr,
			void **stackaddr);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getstackaddr, libpthread.so.0);

	ret = pthread_attr_getstackaddrp(attr, stackaddr);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, stackaddr);

	return ret;
}

int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setstackaddrp)(pthread_attr_t *attr, 
			void *stackaddr);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setstackaddr, libpthread.so.0);

	ret = pthread_attr_setstackaddrp(attr, stackaddr);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, stackaddr);

	return ret;
}

int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getinheritschedp)(const pthread_attr_t *attr,
			int *inheritsched);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getinheritsched, libpthread.so.0);

	ret = pthread_attr_getinheritschedp(attr, inheritsched);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, inheritsched);

	return ret;
}

int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setinheritschedp)(pthread_attr_t *attr, 
			int inheritsched);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setinheritsched, libpthread.so.0);

	ret = pthread_attr_setinheritschedp(attr, inheritsched);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%d", attr, inheritsched);

	return ret;
}

int pthread_attr_getschedparam(const pthread_attr_t *attr,
		struct sched_param *param)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getschedparamp)(const pthread_attr_t *attr,
			struct sched_param *param);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getschedparam, libpthread.so.0);

	ret = pthread_attr_getschedparamp(attr, param);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, param);

	return ret;
}

int pthread_attr_setschedparam(pthread_attr_t *attr, 
		const struct sched_param *param)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setschedparamp)(pthread_attr_t *attr,
			const struct sched_param *param);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setschedparam, libpthread.so.0);

	ret = pthread_attr_setschedparamp(attr, param);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, param);

	return ret;
}

int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getschedpolicyp)(const pthread_attr_t *attr,
			int *policy);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getschedpolicy, libpthread.so.0);

	ret = pthread_attr_getschedpolicyp(attr, policy);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, policy);

	return ret;
}

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setschedpolicyp)(pthread_attr_t *attr, 
			int policy);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setschedpolicy, libpthread.so.0);

	ret = pthread_attr_setschedpolicyp(attr, policy);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%d", attr, policy);

	return ret;
}

int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getguardsizep)(const pthread_attr_t *attr,
			size_t *guardsize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getguardsize, libpthread.so.0);

	ret = pthread_attr_getguardsizep(attr, guardsize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, guardsize);

	return ret;
}

int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setguardsizep)(pthread_attr_t *attr, 
			size_t guardsize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setguardsize, libpthread.so.0);

	ret = pthread_attr_setguardsizep(attr, guardsize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%u", attr, guardsize);

	return ret;
}

int pthread_attr_getscope(const pthread_attr_t *attr, int *contentionscope)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getscopep)(const pthread_attr_t *attr,
			int *contentionscope);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getscope, libpthread.so.0);

	ret = pthread_attr_getscopep(attr, contentionscope);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p", attr, contentionscope);

	return ret;
}

int pthread_attr_setscope(pthread_attr_t *attr, int contentionscope)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setscopep)(pthread_attr_t *attr, 
			int contentionscope);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setscope, libpthread.so.0);

	ret = pthread_attr_setscopep(attr, contentionscope);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%d", attr, contentionscope);

	return ret;
}

int pthread_attr_getstack(const pthread_attr_t *attr, 
		void **stackaddr, size_t *stacksize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_getstackp)(const pthread_attr_t *attr,
			void **stackaddr, size_t *stacksize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_getstack, libpthread.so.0);

	ret = pthread_attr_getstackp(attr, stackaddr, stacksize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p,%p", attr, stackaddr, stacksize);

	return ret;
}

int pthread_attr_setstack(pthread_attr_t *attr,
		void *stackaddr, size_t stacksize)
{
	pthread_t thread = 0;
	static int (*pthread_attr_setstackp)(pthread_attr_t *attr,
			void *stackaddr, size_t stacksize);

	BEFORE_ORIGINAL_THREAD(pthread_attr_setstack, libpthread.so.0);

	ret = pthread_attr_setstackp(attr, stackaddr, stacksize);

	AFTER_ORIGINAL_THREAD_RET(VT_INT, ret, thread, THREAD_API_OTHER, 
			"%p,%p,%u", attr, stackaddr, stacksize);

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

