#ifndef __LIBDASYNC_H__
#define __LIBDASYNC_H__

HANDLER_WRAPPERS_DEF(int, pthread_mutex_init, pthread_mutex_t *, mutex, const pthread_mutexattr_t *, attr);
HANDLER_WRAPPERS_DEF(int, pthread_mutex_destroy, pthread_mutex_t *, mutex);
HANDLER_WRAPPERS_DEF(int, pthread_mutex_lock, pthread_mutex_t *, mutex);
HANDLER_WRAPPERS_DEF(int, pthread_mutex_timedlock, pthread_mutex_t *, mutex, const struct timespec *, abs_timeout);
HANDLER_WRAPPERS_DEF(int, pthread_mutex_trylock, pthread_mutex_t *, mutex);
HANDLER_WRAPPERS_DEF(int, pthread_mutex_unlock, pthread_mutex_t *, mutex);
HANDLER_WRAPPERS_DEF(int, pthread_mutexattr_init, pthread_mutexattr_t *, attr);
HANDLER_WRAPPERS_DEF(int, pthread_mutexattr_destroy, pthread_mutexattr_t *, attr);
HANDLER_WRAPPERS_DEF(int, pthread_mutexattr_getprioceiling, const pthread_mutexattr_t *, attr, int *, prioceiling);
HANDLER_WRAPPERS_DEF(int, pthread_mutexattr_setprioceiling, pthread_mutexattr_t *, attr, int, prioceiling);
HANDLER_WRAPPERS_DEF(int, pthread_mutexattr_getprotocol, const pthread_mutexattr_t *, attr, int *, protocol);
HANDLER_WRAPPERS_DEF(int, pthread_mutexattr_setprotocol, pthread_mutexattr_t *, attr, int, protocol);
HANDLER_WRAPPERS_DEF(int, pthread_mutexattr_getpshared, const pthread_mutexattr_t *, attr, int *, pshared);
HANDLER_WRAPPERS_DEF(int, pthread_mutexattr_setpshared, pthread_mutexattr_t *, attr, int, pshared);
HANDLER_WRAPPERS_DEF(int, pthread_mutexattr_gettype, const pthread_mutexattr_t *, attr, int *, type);
HANDLER_WRAPPERS_DEF(int, pthread_mutexattr_settype, pthread_mutexattr_t *, attr, int, type);
HANDLER_WRAPPERS_DEF(int, pthread_cond_init, pthread_cond_t *, cond, const pthread_condattr_t *, attr);
HANDLER_WRAPPERS_DEF(int, pthread_cond_destroy, pthread_cond_t *, cond);
HANDLER_WRAPPERS_DEF(int, pthread_cond_wait, pthread_cond_t *, cond, pthread_mutex_t *, mutex);
HANDLER_WRAPPERS_DEF(int, pthread_cond_timedwait, pthread_cond_t *, cond, pthread_mutex_t *, mutex, const struct timespec *, abstime);
HANDLER_WRAPPERS_DEF(int, pthread_cond_signal, pthread_cond_t *, cond);
HANDLER_WRAPPERS_DEF(int, pthread_cond_broadcast, pthread_cond_t *, cond);

#endif /* __LIBDASYNC_H__ */
