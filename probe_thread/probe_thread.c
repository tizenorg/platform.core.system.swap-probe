#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "libdasync.h"
//#include "libdathread.h"

#define PROBES_LIST \
	X(PROBE_NAME(pthread_create), "pthread_create") \
	X(PROBE_NAME(pthread_join), "pthread_join") \
	X(PROBE_NAME(pthread_exit), "pthread_exit") \
	X(PROBE_NAME(pthread_cancel), "pthread_cancel") \
	X(PROBE_NAME(pthread_detach), "pthread_detach") \
	X(PROBE_NAME(pthread_self), "pthread_self") \
	X(PROBE_NAME(pthread_equal), "pthread_equal") \
	X(PROBE_NAME(pthread_setcancelstate), "pthread_setcancelstate") \
	X(PROBE_NAME(pthread_setcanceltype), "pthread_setcanceltype") \
	X(PROBE_NAME(pthread_attr_init), "pthread_attr_init") \
	X(PROBE_NAME(pthread_attr_destroy), "pthread_attr_destroy") \
	X(PROBE_NAME(pthread_attr_getdetachstate), "pthread_attr_getdetachstate") \
	X(PROBE_NAME(pthread_attr_setdetachstate), "pthread_attr_setdetachstate") \
	X(PROBE_NAME(pthread_attr_getstacksize), "pthread_attr_getstacksize") \
	X(PROBE_NAME(pthread_attr_setstacksize), "pthread_attr_setstacksize") \
	X(PROBE_NAME(pthread_attr_getinheritsched), "pthread_attr_getinheritsched") \
	X(PROBE_NAME(pthread_attr_setinheritsched), "pthread_attr_setinheritsched") \
	X(PROBE_NAME(pthread_attr_getschedparam), "pthread_attr_getschedparam") \
	X(PROBE_NAME(pthread_attr_setschedparam), "pthread_attr_setschedparam") \
	X(PROBE_NAME(pthread_attr_getschedpolicy), "pthread_attr_getschedpolicy") \
	X(PROBE_NAME(pthread_attr_setschedpolicy), "pthread_attr_setschedpolicy") \
	X(PROBE_NAME(pthread_attr_getguardsize), "pthread_attr_getguardsize") \
	X(PROBE_NAME(pthread_attr_setguardsize), "pthread_attr_setguardsize") \
	X(PROBE_NAME(pthread_attr_getscope), "pthread_attr_getscope") \
	X(PROBE_NAME(pthread_attr_setscope), "pthread_attr_setscope") \
	X(PROBE_NAME(pthread_attr_getstack), "pthread_attr_getstack") \
	X(PROBE_NAME(pthread_attr_setstack), "pthread_attr_setstack") \
	X(PROBE_NAME(pthread_mutex_init), "pthread_mutex_init") \
	X(PROBE_NAME(pthread_mutex_destroy), "pthread_mutex_destroy") \
	X(PROBE_NAME(pthread_mutex_lock), "pthread_mutex_lock") \
	X(PROBE_NAME(pthread_mutex_timedlock), "pthread_mutex_timedlock") \
	X(PROBE_NAME(pthread_mutex_trylock), "pthread_mutex_trylock") \
	X(PROBE_NAME(pthread_mutex_unlock), "pthread_mutex_unlock") \
	X(PROBE_NAME(pthread_mutexattr_init), "pthread_mutexattr_init") \
	X(PROBE_NAME(pthread_mutexattr_destroy), "pthread_mutexattr_destroy") \
	X(PROBE_NAME(pthread_mutexattr_getprioceiling), "pthread_mutexattr_getprioceiling") \
	X(PROBE_NAME(pthread_mutexattr_setprioceiling), "pthread_mutexattr_setprioceiling") \
	X(PROBE_NAME(pthread_mutexattr_getprotocol), "pthread_mutexattr_getprotocol") \
	X(PROBE_NAME(pthread_mutexattr_setprotocol), "pthread_mutexattr_setprotocol") \
	X(PROBE_NAME(pthread_mutexattr_getpshared), "pthread_mutexattr_getpshared") \
	X(PROBE_NAME(pthread_mutexattr_setpshared), "pthread_mutexattr_setpshared") \
	X(PROBE_NAME(pthread_mutexattr_gettype), "pthread_mutexattr_gettype") \
	X(PROBE_NAME(pthread_mutexattr_settype), "pthread_mutexattr_settype") \
	X(PROBE_NAME(pthread_cond_init), "pthread_cond_init") \
	X(PROBE_NAME(pthread_cond_destroy), "pthread_cond_destroy") \
	X(PROBE_NAME(pthread_cond_wait), "pthread_cond_wait") \
	X(PROBE_NAME(pthread_cond_timedwait), "pthread_cond_timedwait") \
	X(PROBE_NAME(pthread_cond_signal), "pthread_cond_signal") \
	X(PROBE_NAME(pthread_cond_broadcast), "pthread_cond_broadcast")



/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
#define X(func_name, orig_name) { & func_name, orig_name },

static struct probe_desc_t thread_probes[] = {
	PROBES_LIST
};

#undef X

/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

struct feature_desc_t thread_feature = {
	.feature = FL_THREAD_API_PROBING,
	.cnt = (PROBES_LIST),
	.probes = thread_probes
};

struct feature_desc_t thread_feature_always = {
	.feature = FL_THREAD_API_ALWAYS_PROBING,
	.cnt = (PROBES_LIST),
	.probes = thread_probes
};

#undef X

#undef PROBES_LIST
