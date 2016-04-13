#ifndef __LIBDATHREAD_H__
#define __LIBDATHREAD_H__

int PROBE_NAME(pthread_create)(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void*), void *arg);
int PROBE_NAME(pthread_join)(pthread_t thread, void **retval);
void PROBE_NAME(pthread_exit)(void *retval);
int PROBE_NAME(pthread_cancel)(pthread_t thread);
int PROBE_NAME(pthread_detach)(pthread_t thread);
pthread_t PROBE_NAME(pthread_self)(void);
int PROBE_NAME(pthread_equal)(pthread_t t1, pthread_t t2);
int PROBE_NAME(pthread_setcancelstate)(int state, int *oldstate);
int PROBE_NAME(pthread_setcanceltype)(int type, int *oldtype);
int PROBE_NAME(pthread_attr_init)(pthread_attr_t *attr);
int PROBE_NAME(pthread_attr_destroy)(pthread_attr_t *attr);
int PROBE_NAME(pthread_attr_getdetachstate)(const pthread_attr_t *attr, int *detachstate);
int PROBE_NAME(pthread_attr_setdetachstate)(pthread_attr_t *attr, int detachstate);
int PROBE_NAME(pthread_attr_getstacksize)(const pthread_attr_t *attr, size_t *stacksize);
int PROBE_NAME(pthread_attr_setstacksize)(pthread_attr_t *attr, size_t stacksize);
//int PROBE_NAME(pthread_attr_getstackaddr)(const pthread_attr_t *attr, void **stackaddr);
//int PROBE_NAME(pthread_attr_setstackaddr)(pthread_attr_t *attr, void *stackaddr);
int PROBE_NAME(pthread_attr_getinheritsched)(const pthread_attr_t *attr, int *inheritsched);
int PROBE_NAME(pthread_attr_setinheritsched)(pthread_attr_t *attr, int inheritsched);
int PROBE_NAME(pthread_attr_getschedparam)(const pthread_attr_t *attr, struct sched_param *param);
int PROBE_NAME(pthread_attr_setschedparam)(pthread_attr_t *attr, const struct sched_param *param);
int PROBE_NAME(pthread_attr_getschedpolicy)(const pthread_attr_t *attr, int *policy)
int PROBE_NAME(pthread_attr_setschedpolicy)(pthread_attr_t *attr, int policy)
int PROBE_NAME(pthread_attr_getguardsize)(const pthread_attr_t *attr, size_t *guardsize)
int PROBE_NAME(pthread_attr_setguardsize)(pthread_attr_t *attr, size_t guardsize)
int PROBE_NAME(pthread_attr_getscope)(const pthread_attr_t *attr, int *contentionscope)
int PROBE_NAME(pthread_attr_setscope)(pthread_attr_t *attr, int contentionscope)
int PROBE_NAME(pthread_attr_getstack)(const pthread_attr_t *attr, void **stackaddr, size_t *stacksize);
int PROBE_NAME(pthread_attr_setstack)(pthread_attr_t *attr, void *stackaddr, size_t stacksize);

#endif /* __LIBDATHREAD_H__ */
