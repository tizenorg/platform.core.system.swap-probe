/*
 *  DA probe
 *
 * Copyright 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Woojin Jung <woojin2.jung@samsung.com>
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
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

#include <pthread.h>
#include <time.h>
#include <errno.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dahelper.h"
#include "da_sync.h"

#include "binproto.h"

static enum DaOptions _sopt = OPT_THREAD;

int pthread_mutex_init(pthread_mutex_t *mutex, 
		const pthread_mutexattr_t *attr) {
	static int (*pthread_mutex_initp)(pthread_mutex_t *mutex,
			const pthread_mutexattr_t *attr);

	BEFORE_ORIGINAL_SYNC(pthread_mutex_init, LIBPTHREAD);

	ret = pthread_mutex_initp(mutex, attr);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutex_init,
				 ret, mutex, SYNC_PTHREAD_MUTEX, 
				 SYNC_API_OTHER, "pp", mutex, attr);

	return ret;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex) {
	static int (*pthread_mutex_destroyp)(pthread_mutex_t *mutex);

	BEFORE_ORIGINAL_SYNC(pthread_mutex_destroy, LIBPTHREAD);

	ret = pthread_mutex_destroyp(mutex);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutex_destroy,
				 ret, mutex, SYNC_PTHREAD_MUTEX, 
				 SYNC_API_OTHER, "p", mutex);

	return ret;
}

int real_pthread_mutex_lock(pthread_mutex_t *mutex) {
	static int (*pthread_mutex_lockp)(pthread_mutex_t *mutex);

	GET_REAL_FUNC(pthread_mutex_lock, LIBPTHREAD);

	return pthread_mutex_lockp(mutex);
}

int pthread_mutex_lock(pthread_mutex_t *mutex) {
	static int (*pthread_mutex_lockp)(pthread_mutex_t *mutex);
	
	DECLARE_VARIABLE_STANDARD;
	GET_REAL_FUNC(pthread_mutex_lock, LIBPTHREAD);

	PRE_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_SYNC,
			  API_ID_pthread_mutex_lock,
			  "p", mutex);
	PACK_COMMON_END(0, 0, blockresult);
	PACK_SYNC(mutex, SYNC_PTHREAD_MUTEX, SYNC_API_ACQUIRE_WAIT_START);
	FLUSH_LOCAL_BUF();

	PRE_PROBEBLOCK_END();

	ret = pthread_mutex_lockp(mutex);

	// send WAIT_END log
	newerrno = errno;
	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC,
				  API_ID_pthread_mutex_lock,
				  "p", mutex);
		PACK_COMMON_END(ret, errno, blockresult);
		PACK_SYNC(mutex, SYNC_PTHREAD_MUTEX, SYNC_API_ACQUIRE_WAIT_END);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}

	return ret;
}

int pthread_mutex_timedlock(pthread_mutex_t *mutex,
		const struct timespec *abs_timeout) {
	static int (*pthread_mutex_timedlockp)(pthread_mutex_t *mutex,
			const struct timespec *abs_timeout);

	DECLARE_VARIABLE_STANDARD;
	GET_REAL_FUNC(pthread_mutex_timedlock, LIBPTHREAD);

	PRE_PROBEBLOCK_BEGIN();

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_SYNC,
			  API_ID_pthread_mutex_timedlock,
			  "pp", mutex, abs_timeout);
	PACK_COMMON_END(0, 0, blockresult);
	PACK_SYNC(mutex, SYNC_PTHREAD_MUTEX, SYNC_API_ACQUIRE_WAIT_START);
	FLUSH_LOCAL_BUF();

	PRE_PROBEBLOCK_END();

	ret = pthread_mutex_timedlockp(mutex, abs_timeout);

	// send WAIT_END log
	newerrno = errno;
	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC,
				  API_ID_pthread_mutex_timedlock,
				  "pp", mutex, abs_timeout);
		PACK_COMMON_END(ret, errno, blockresult);
		PACK_SYNC(mutex, SYNC_PTHREAD_MUTEX, SYNC_API_ACQUIRE_WAIT_END);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	
	return ret;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex) {
	static int (*pthread_mutex_trylockp)(pthread_mutex_t *mutex);

	BEFORE_ORIGINAL_SYNC(pthread_mutex_trylock, LIBPTHREAD);

	ret = pthread_mutex_trylockp(mutex);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutex_trylock,
				 ret, mutex, SYNC_PTHREAD_MUTEX, 
				 SYNC_API_TRY_ACQUIRE, "p", mutex);
	
	return ret;
}

int real_pthread_mutex_unlock(pthread_mutex_t *mutex) {
	static int (*pthread_mutex_unlockp)(pthread_mutex_t *mutex);

	GET_REAL_FUNC(pthread_mutex_unlock, LIBPTHREAD);

	return pthread_mutex_unlockp(mutex);
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
	static int (*pthread_mutex_unlockp)(pthread_mutex_t *mutex);

	BEFORE_ORIGINAL_SYNC(pthread_mutex_unlock, LIBPTHREAD);

	ret = pthread_mutex_unlockp(mutex);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutex_unlock,
				 ret, mutex, SYNC_PTHREAD_MUTEX, 
				 SYNC_API_RELEASE, "p", mutex);
	
	return ret;
}

int pthread_mutexattr_init(pthread_mutexattr_t *attr) {
	static int (*pthread_mutexattr_initp)(pthread_mutexattr_t *attr);

	BEFORE_ORIGINAL_SYNC(pthread_mutexattr_init, LIBPTHREAD);

	ret = pthread_mutexattr_initp(attr);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutexattr_init,
				 ret, 0, SYNC_PTHREAD_MUTEX,
				 SYNC_API_OTHER, "p", attr);

	return ret;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr) {
	static int (*pthread_mutexattr_destroyp)(pthread_mutexattr_t *attr);

	BEFORE_ORIGINAL_SYNC(pthread_mutexattr_destroy, LIBPTHREAD);

	ret = pthread_mutexattr_destroyp(attr);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutexattr_destroy,
				 ret, 0, SYNC_PTHREAD_MUTEX,
				 SYNC_API_OTHER, "p", attr);

	return ret;
}

int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr,
		int *prioceiling) {
	static int (*pthread_mutexattr_getprioceilingp)(
			const pthread_mutexattr_t *attr, int *prioceiling);

	BEFORE_ORIGINAL_SYNC(pthread_mutexattr_getprioceiling, LIBPTHREAD);

	ret = pthread_mutexattr_getprioceilingp(attr, prioceiling);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutexattr_getprioceiling,
				 ret, 0, SYNC_PTHREAD_MUTEX,
				 SYNC_API_OTHER, "pp", attr, prioceiling);

	return ret;
}

int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, 
		int prioceiling) {
	static int (*pthread_mutexattr_setprioceilingp)(
			pthread_mutexattr_t *attr, int prioceiling);

	BEFORE_ORIGINAL_SYNC(pthread_mutexattr_setprioceiling, LIBPTHREAD);

	ret = pthread_mutexattr_setprioceilingp(attr, prioceiling);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutexattr_setprioceiling,
				 ret, 0, SYNC_PTHREAD_MUTEX,
				 SYNC_API_OTHER, "pd", attr, prioceiling);

	return ret;
}

int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, 
		int *protocol) {
	static int (*pthread_mutexattr_getprotocolp)(
			const pthread_mutexattr_t *attr, int *protocol);

	BEFORE_ORIGINAL_SYNC(pthread_mutexattr_getprotocol, LIBPTHREAD);

	ret = pthread_mutexattr_getprotocolp(attr, protocol);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutexattr_getprotocol,
				 ret, 0, SYNC_PTHREAD_MUTEX,
				 SYNC_API_OTHER, "pp", attr, protocol);

	return ret;
}

int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, 
		int protocol) {
	static int (*pthread_mutexattr_setprotocolp)(
			pthread_mutexattr_t *attr, int protocol);

	BEFORE_ORIGINAL_SYNC(pthread_mutexattr_setprotocol, LIBPTHREAD);

	ret = pthread_mutexattr_setprotocolp(attr, protocol);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutexattr_setprotocol,
				 ret, 0, SYNC_PTHREAD_MUTEX,
				 SYNC_API_OTHER, "pd", attr, protocol);

	return ret;
}

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, 
		int *pshared) {
	static int (*pthread_mutexattr_getpsharedp)(
			const pthread_mutexattr_t *attr, int *pshared);

	BEFORE_ORIGINAL_SYNC(pthread_mutexattr_getpshared, LIBPTHREAD);

	ret = pthread_mutexattr_getpsharedp(attr, pshared);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutexattr_getpshared,
				 ret, 0, SYNC_PTHREAD_MUTEX,
				 SYNC_API_OTHER, "pp", attr, pshared);

	return ret;
}

int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, 
		int pshared) {
	static int (*pthread_mutexattr_setpsharedp)(
			pthread_mutexattr_t *attr, int pshared);

	BEFORE_ORIGINAL_SYNC(pthread_mutexattr_setpshared, LIBPTHREAD);

	ret = pthread_mutexattr_setpsharedp(attr, pshared);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutexattr_setpshared,
				 ret, 0, SYNC_PTHREAD_MUTEX,
				 SYNC_API_OTHER, "pd", attr, pshared);

	return ret;
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type) {
	static int (*pthread_mutexattr_gettypep)(
			const pthread_mutexattr_t *attr, int *type);

	BEFORE_ORIGINAL_SYNC(pthread_mutexattr_gettype, LIBPTHREAD);

	ret = pthread_mutexattr_gettypep(attr, type);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutexattr_gettype,
				 ret, 0, SYNC_PTHREAD_MUTEX,
				 SYNC_API_OTHER, "pp", attr, type);

	return ret;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type) {
	static int (*pthread_mutexattr_settypep)(
			pthread_mutexattr_t *attr, int type);

	BEFORE_ORIGINAL_SYNC(pthread_mutexattr_settype, LIBPTHREAD);

	ret = pthread_mutexattr_settypep(attr, type);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_mutexattr_settype,
				 ret, 0, SYNC_PTHREAD_MUTEX,
				 SYNC_API_OTHER, "pd", attr, type);

	return ret;
}

/*
int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex,
		int *prioceiling);
int pthread_mutex_setprioceiling(pthread_mutex_t *mutex,
		int prioceiling, int *old_ceiling);
*/

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr) {
	static int (*pthread_cond_initp)(pthread_cond_t *cond,
			const pthread_condattr_t *attr);

	BEFORE_ORIGINAL_SYNC(pthread_cond_init, LIBPTHREAD);

	ret = pthread_cond_initp(cond, attr);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_cond_init,
				 ret, cond, SYNC_PTHREAD_COND_VARIABLE, 
				 SYNC_API_OTHER, "pp", cond, attr);

	return ret;
}

int pthread_cond_destroy(pthread_cond_t *cond) {
	static int (*pthread_cond_destroyp)(pthread_cond_t *cond);

	BEFORE_ORIGINAL_SYNC(pthread_cond_destroy, LIBPTHREAD);

	ret = pthread_cond_destroyp(cond);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_cond_destroy,
				 ret, cond, SYNC_PTHREAD_COND_VARIABLE, 
				 SYNC_API_OTHER, "p", cond);

	return ret;
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
	static int (*pthread_cond_waitp)(pthread_cond_t *cond,
			pthread_mutex_t *mutex);

	DECLARE_VARIABLE_STANDARD;
	GET_REAL_FUNC(pthread_cond_wait, LIBPTHREAD);

	PRE_PROBEBLOCK_BEGIN();
	// send WAIT_START log
	
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_SYNC,
			  API_ID_pthread_cond_wait,
			  "pp", cond, mutex);
	PACK_COMMON_END(0, 0, blockresult);
	PACK_SYNC(cond, SYNC_PTHREAD_COND_VARIABLE, SYNC_API_COND_WAIT_START);
	FLUSH_LOCAL_BUF();

	PRE_PROBEBLOCK_END();

	ret = pthread_cond_waitp(cond, mutex);

	// send WAIT_END log
	newerrno = errno;
	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC,
				  API_ID_pthread_cond_wait,
				  "pp", cond, mutex);
		PACK_COMMON_END(ret, errno, blockresult);
		PACK_SYNC(cond, SYNC_PTHREAD_COND_VARIABLE, SYNC_API_COND_WAIT_END);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}

	return ret;
}

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
		const struct timespec *abstime) {
	static int (*pthread_cond_timedwaitp)(pthread_cond_t *cond,
				pthread_mutex_t *mutex, const struct timespec *abstime);

	DECLARE_VARIABLE_STANDARD;
	GET_REAL_FUNC(pthread_cond_timedwait, LIBPTHREAD);

	PRE_PROBEBLOCK_BEGIN();
	// send WAIT_START log

	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_SYNC,
			  API_ID_pthread_cond_timedwait,
			  "ppp", cond, mutex, abstime);
	PACK_COMMON_END(0, 0, blockresult);
	PACK_SYNC(cond, SYNC_PTHREAD_COND_VARIABLE, SYNC_API_COND_WAIT_START);
	FLUSH_LOCAL_BUF();

	PRE_PROBEBLOCK_END();

	ret = pthread_cond_timedwaitp(cond, mutex, abstime);

	// send WAIT_END log
	newerrno = errno;
	if(postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC,
				  API_ID_pthread_cond_timedwait,
				  "ppp", cond, mutex, abstime);
		PACK_COMMON_END(ret, errno, blockresult);
		PACK_SYNC(cond, SYNC_PTHREAD_COND_VARIABLE, SYNC_API_COND_WAIT_END);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}

	return ret;
}

int pthread_cond_signal(pthread_cond_t *cond) {
	static int (*pthread_cond_signalp)(pthread_cond_t *cond);

	BEFORE_ORIGINAL_SYNC(pthread_cond_signal, LIBPTHREAD);

	ret = pthread_cond_signalp(cond);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_cond_signal,
				 ret, cond, SYNC_PTHREAD_COND_VARIABLE, 
			SYNC_API_NOTIFY, "p", cond);
	
	return ret;
}

int pthread_cond_broadcast(pthread_cond_t *cond) {
	static int (*pthread_cond_broadcastp)(pthread_cond_t *cond);

	BEFORE_ORIGINAL_SYNC(pthread_cond_broadcast, LIBPTHREAD);

	ret = pthread_cond_broadcastp(cond);

	AFTER_PACK_ORIGINAL_SYNC(API_ID_pthread_cond_broadcast,
				 ret, cond, SYNC_PTHREAD_COND_VARIABLE, 
				 SYNC_API_NOTIFY_ALL, "p", cond);
	
	return ret;
}

/*
int pthread_condattr_init(pthread_condattr_t *attr);
int pthread_condattr_destroy(pthread_condattr_t *attr);
int pthread_condattr_getclock(const pthread_condattr_t *attr,
		clockid_t *clock_id);
int pthread_condattr_setclock(pthread_condattr_t *attr,
		clockid_t clock_id);
int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared);
int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);
*/

/*
int pthread_rwlock_init(pthread_rwlock_t *rwlock,
		const pthread_rwlockattr_t *attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock); 
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_timedrdlock(pthread_rwlock_t *rwlock,
		const struct timespec *abs_timeout);
int pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock,
		const struct timespec *abs_timeout);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr,
		int *pshared);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr,
		int pshared);

int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
int pthread_spin_destroy(pthread_spinlock_t *lock);
int pthread_spin_lock(pthread_spinlock_t *lock);
int pthread_spin_trylock(pthread_spinlock_t *lock);
int pthread_spin_unlock(pthread_spinlock_t *lock);

int pthread_barrier_init(pthread_barrier_t *barrier,
		const pthread_barrierattr_t *attr, unsigned int count);
int pthread_barrier_destroy(pthread_barrier_t *barrier);
int pthread_barrier_wait(pthread_barrier_t *barrier);
int pthread_barrierattr_init(pthread_barrierattr_t *attr);
int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, 
		int *pshared);
int pthread_barrierattr_setpshared(pthread_barrierattr_t, 
		int pshared);
*/

