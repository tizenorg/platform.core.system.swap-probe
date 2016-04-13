#ifndef __LIBDATHREAD_H__
#define __LIBDATHREAD_H__

typedef void *(*start_routine_t)(void *);

HANDLER_WRAPPERS_DEF(int, pthread_create, pthread_t *, thread, const pthread_attr_t *, attr, start_routine_t, start_routine, void *, arg);
HANDLER_WRAPPERS_DEF(int, pthread_join, pthread_t, thread, void **, retval);
HANDLER_WRAPPERS_DEF(void, pthread_exit, void *, retval);
HANDLER_WRAPPERS_DEF(int, pthread_cancel, pthread_t, thread);
HANDLER_WRAPPERS_DEF(int, pthread_detach, pthread_t, thread);
HANDLER_WRAPPERS_DEF(pthread_t, pthread_self, void);
HANDLER_WRAPPERS_DEF(int, pthread_equal, pthread_t, t1, pthread_t, t2);
HANDLER_WRAPPERS_DEF(int, pthread_setcancelstate, int, state, int *, oldstate);
HANDLER_WRAPPERS_DEF(int, pthread_setcanceltype, int, type, int *, oldtype);
HANDLER_WRAPPERS_DEF(int, pthread_attr_init, pthread_attr_t *, attr);
HANDLER_WRAPPERS_DEF(int, pthread_attr_destroy, pthread_attr_t *, attr);
HANDLER_WRAPPERS_DEF(int, pthread_attr_getdetachstate, const pthread_attr_t *, attr, int *, detachstate);
HANDLER_WRAPPERS_DEF(int, pthread_attr_setdetachstate, pthread_attr_t *, attr, int, detachstate);
HANDLER_WRAPPERS_DEF(int, pthread_attr_getstacksize, const pthread_attr_t *, attr, size_t *, stacksize);
HANDLER_WRAPPERS_DEF(int, pthread_attr_setstacksize, pthread_attr_t *, attr, size_t, stacksize);
//HANDLER_WRAPPERS_DEF(int, pthread_attr_getstackaddr, const pthread_attr_t *, attr, void **, stackaddr);
//HANDLER_WRAPPERS_DEF(int, pthread_attr_setstackaddr, pthread_attr_t *, attr, void *, stackaddr);
HANDLER_WRAPPERS_DEF(int, pthread_attr_getinheritsched, const pthread_attr_t *, attr, int *, inheritsched);
HANDLER_WRAPPERS_DEF(int, pthread_attr_setinheritsched, pthread_attr_t *, attr, int, inheritsched);
HANDLER_WRAPPERS_DEF(int, pthread_attr_getschedparam, const pthread_attr_t *, attr, struct sched_param *, param);
HANDLER_WRAPPERS_DEF(int, pthread_attr_setschedparam, pthread_attr_t *, attr, const struct sched_param *, param);
HANDLER_WRAPPERS_DEF(int, pthread_attr_getschedpolicy, const pthread_attr_t *, attr, int *, policy)
HANDLER_WRAPPERS_DEF(int, pthread_attr_setschedpolicy, pthread_attr_t *, attr, int, policy)
HANDLER_WRAPPERS_DEF(int, pthread_attr_getguardsize, const pthread_attr_t *, attr, size_t *, guardsize)
HANDLER_WRAPPERS_DEF(int, pthread_attr_setguardsize, pthread_attr_t *, attr, size_t, guardsize)
HANDLER_WRAPPERS_DEF(int, pthread_attr_getscope, const pthread_attr_t *, attr, int *, contentionscope)
HANDLER_WRAPPERS_DEF(int, pthread_attr_setscope, pthread_attr_t *, attr, int, contentionscope)
HANDLER_WRAPPERS_DEF(int, pthread_attr_getstack, const pthread_attr_t *, attr, void **, stackaddr, size_t *, stacksize);
HANDLER_WRAPPERS_DEF(int, pthread_attr_setstack, pthread_attr_t *, attr, void *, stackaddr, size_t, stacksize);

#endif /* __LIBDATHREAD_H__ */
