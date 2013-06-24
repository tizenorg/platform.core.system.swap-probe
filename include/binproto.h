#ifndef __BIN_PROTO_H__
#define __BIN_PROTO_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

// getpid/gettid
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#include <sys/time.h>

#define MSG_PROBE_MEMORY 0x3001
#define MSG_PROBE_UICONTROL 0x3002
#define MSG_PROBE_UIEVENT 0x3003
#define MSG_PROBE_RESOUCE 0x3004
#define MSG_PROBE_LIFECYCLE 0x30015
#define MSG_PROBE_SCREENSHOT 0x3006
#define MSG_PROBE_SCENE 0x3007
#define MSG_PROBE_THREAD 0x3008
#define MSG_PROBE_CUSTOM 0x3009
#define MSG_PROBE_SYNC 0x3010

// TODO: remove this copy-paste
#define CALLER_ADDRESS							\
	((void*) __builtin_extract_return_addr(__builtin_return_address(0)))



// COMMON
static inline char *pack_int32(char *to, uint32_t val)
{
	*(uint32_t *)to = val;
	return to + sizeof(uint32_t);
}

static inline char *pack_int64(char *to, uint64_t val)
{
	*(uint64_t *)to = val;
	return to + sizeof(uint64_t);
}

static inline char *pack_string(char *to, const char *str)
{
	size_t len = strlen(str) + 1;
	strncpy(to, str, len);
	return to + len;
}

static inline char *pack_timestamp(char *to)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	to = pack_int32(to, tv.tv_sec);
	to = pack_int32(to, tv.tv_usec * 1000);

	return to;
}

static inline char *pack_args(char *to, const char *fmt, ...)
{
	va_list args;
	uint32_t num = strlen(fmt);
	const char *t = fmt;

	memcpy(to, &num, sizeof(num));
	to += sizeof(num);

	va_start(args, fmt);

	uint8_t c;
	uint32_t d;
	uint64_t x;
	uint64_t p;
	float f;
	double w;
	char *s;
	int n;

	for (t = fmt; *t != '\0'; t++) {
		switch (*t) {
		case 'c':
			c = (uint8_t)va_arg(args, uint32_t);
			*to++ = *t;
			memcpy(to, &c, sizeof(c));
			to += sizeof(c);
			break;
		case 'd':
			d = va_arg(args, uint32_t);
			*to++ = *t;
			memcpy(to, &d, sizeof(d));
			to += sizeof(d);
			break;
		case 'x':
			x = (uint64_t)va_arg(args, uint64_t);
			*to++ = *t;
			memcpy(to, &x, sizeof(x));
			to += sizeof(x);
			break;
		case 'p':
			p = (uintptr_t)va_arg(args, uint64_t);
			*to++ = *t;
			memcpy(to, &p, sizeof(p));
			to += sizeof(p);
			break;
		case 'f':
			f = (float)va_arg(args, double);
			*to++ = *t;
			memcpy(to, &f, sizeof(f));
			to += sizeof(f);
			break;
		case 'w':
			w = va_arg(args, double);
			*to++ = *t;
			memcpy(to, &w, sizeof(w));
			to += sizeof(w);
			break;
		case 's':
			s = va_arg(args, char *);
			*to++ = *t;
			n = strlen(s) + 1;
			strncpy(to, s, n);
			to += n;
			break;
		default:
			break;
		}
	}

	va_end(args);

	return to;
}

#define BUF_PTR p
#define RET_PTR ret_p
#define PACK_INT32(val)				\
	BUF_PTR = pack_int32(BUF_PTR, val);
#define PACK_INT64(val)				\
	BUF_PTR = pack_int64(BUF_PTR, val);
#define PACK_STRING(str)			\
	BUF_PTR = pack_string(BUF_PTR, str);

#define PACK_COMMON_BEGIN(msg_id, api_id, fmt, ...)		\
	do {							\
		BUF_PTR = pack_int32(BUF_PTR, msg_id);		\
		BUF_PTR = pack_int32(BUF_PTR, 0);		\
		BUF_PTR = pack_timestamp(BUF_PTR);		\
		BUF_PTR = pack_int32(BUF_PTR, 0);		\
		BUF_PTR = pack_int32(BUF_PTR, api_id);		\
		BUF_PTR = pack_int32(BUF_PTR, getpid());		\
		BUF_PTR = pack_int32(BUF_PTR, syscall(__NR_gettid));	\
		BUF_PTR = pack_args(BUF_PTR, fmt, __VA_ARGS__);	\
		RET_PTR = BUF_PTR;		\
	} while (0)

#define PACK_COMMON_END(ret, errn, intern_call)		\
	do {							\
		BUF_PTR = pack_int64(BUF_PTR, (uintptr_t)(ret));	\
		BUF_PTR = pack_int32(BUF_PTR, (uint32_t)errn);	\
		BUF_PTR = pack_int32(BUF_PTR, (uint32_t)intern_call);	\
		BUF_PTR = pack_int64(BUF_PTR, (uintptr_t)CALLER_ADDRESS); \
		BUF_PTR = pack_int32(BUF_PTR, 0);		\
		BUF_PTR = pack_int32(BUF_PTR, 0);		\
	} while (0)

#define PACK_RETURN_END(ret)								\
		RET_PTR = pack_int64(RET_PTR, (uintptr_t)(ret));

#define PACK_MEMORY(size, memory_api_type, addr)		\
	do {							\
		BUF_PTR = pack_int32(p, size);			\
		BUF_PTR = pack_int32(p, memory_api_type);	\
		BUF_PTR = pack_int64(p, (uintptr_t)addr);	\
	} while (0)

#define PACK_UICONTROL(control)						\
	do {								\
		if (unlikely(control == NULL)) {			\
			BUF_PTR = pack_string(BUF_PTR, "");		\
			BUF_PTR = pack_string(BUF_PTR, "");		\
			BUF_PTR = pack_int64(BUF_PTR, 0);		\
		} else {						\
			char *type = NULL, *name = NULL;		\
			if (find_object_hash((void*)(control),		\
					     &type, &name) == 1) {	\
				BUF_PTR = pack_string(BUF_PTR, type);	\
				BUF_PTR = pack_string(BUF_PTR, name);	\
			} else {					\
				BUF_PTR = pack_string(BUF_PTR, "");	\
				BUF_PTR = pack_string(BUF_PTR, "");	\
			}						\
			BUF_PTR = pack_int64(BUF_PTR, (uintptr_t)(control)); \
		}							\
	} while(0)

#define PACK_UIEVENT(event_type, detail_type, x, y, info1, info2)	\
	do {								\
		BUF_PTR = pack_int32(BUF_PTR, event_type);		\
		BUF_PTR = pack_int32(BUF_PTR, detail_type);		\
		BUF_PTR = pack_int32(BUF_PTR, x);			\
		BUF_PTR = pack_int32(BUF_PTR, y);			\
		BUF_PTR = pack_string(BUF_PTR, info1);			\
		BUF_PTR = pack_int32(BUF_PTR, info2);			\
	} while (0)

/* #define PACK_RESOURCE(size, fd_value, fd_type, fd_api_type, file_size,	\ */
/* 		      file_path)					\ */
/* 	do {								\ */
/* 		BUF_PTR = pack_int32(BUF_PTR, size);			\ */
/* 		BUF_PTR = pack_int32(BUF_PTR, fd_value);		\ */
/* 		BUF_PTR = pack_int32(BUF_PTR, fd_type);			\ */
/* 		BUF_PTR = pack_int32(BUF_PTR, fd_api_type);		\ */
/* 		BUF_PTR = pack_int32(BUF_PTR, file_size);		\ */
/* 		BUF_PTR = pack_string(BUF_PTR, file_path);		\ */
/* 	} while (0) */

/* #define PACK_SCREENSHOT(image_file_path, orienation)			\ */
/* 		do {							\ */
/* 			  BUF_PTR = pack_string(BUF_PTR, image_file_path); \ */
/* 			  BUF_PTR = pack_int32(BUF_PTR, orienation);	\ */
/* 		  } while (0) */

/* #define PACK_SCENE(scene_name, form_name, form_pointer,		\ */
/* 		   panel_name, panel_pointer, transition_time,	\ */
/* 		   user_transition_time)			\ */
/* 		do {							\ */
/* 	BUF_PTR = pack_string(BUF_PTR, scene_name);				\ */
/* 	BUF_PTR = pack_string(BUF_PTR, form_name);				\ */
/* 	BUF_PTR = pack_int64(BUF_PTR, form_pointer);			\ */
/* 	BUF_PTR = pack_string(BUF_PTR, panel_name);				\ */
/* 	BUF_PTR = pack_int64(BUF_PTR, panel_pointer);			\ */
/* 	BUF_PTR = pack_int32(BUF_PTR, transition_time);			\ */
/* 	BUF_PTR = pack_int32(BUF_PTR, user_transition_time);		\ */
/* 		} while (0) */

/* #define PACK_THREAD(BUF_PTRthread_id, osp_thread_id, thread_type, api_type)	\ */
/* 	BUF_PTR = pack_int32(BUF_PTR, pthread_id);					\ */
/* 	BUF_PTR = pack_int32(BUF_PTR, osp_thread_id);				\ */
/* 	BUF_PTR = pack_int32(BUF_PTR, thread_type);					\ */
/* 	BUF_PTR = pack_int32(BUF_PTR, api_type); */

/* #define PACK_CUSTOM(handle, type, name, color, value)	\ */
/* 	BUF_PTR = pack_int32(BUF_PTR, handle);			\ */
/* 	BUF_PTR = pack_int32(BUF_PTR, type);			\ */
/* 	BUF_PTR = pack_string(BUF_PTR, name);			\ */
/* 	BUF_PTR = pack_int32(BUF_PTR, color);			\ */
/* 	BUF_PTR = pack_int32(BUF_PTR, value); */

#define PACK_SYNC(sync_val, sync_type, api_type)		     \
	do {						     	     \
		BUF_PTR = pack_int64(BUF_PTR, (uintptr_t)sync_val);  \
		BUF_PTR = pack_int32(BUF_PTR, sync_type);	     \
		BUF_PTR = pack_int32(BUF_PTR, api_type);	     \
	} while (0)

#define LOG_PATH "/tmp/trace.bin"
#define OPEN_LOG()					\
		do {					\
			log_fd = creat(LOG_PATH, 0644);	\
			if (log_fd == -1) {		\
				exit(1);		\
			}				\
		} while (0)
#define CLOSE_LOG()				\
		do {				\
			if (log_fd > 0) {	\
				close(log_fd);	\
			}			\
		} while (0)



#define LOCAL_BUF_SIZE 1024
#define PREPARE_LOCAL_BUF()			\
		char buf[LOCAL_BUF_SIZE];	\
		char *p = buf;			\
		char *ret_p = NULL;

#define MSG_LEN_OFFSET 16
#define MSG_HDR_LEN 20
#define FLUSH_LOCAL_BUF()						\
		*(uint32_t *)(buf + MSG_LEN_OFFSET) = (p - buf) - MSG_HDR_LEN; \
		write(log_fd, buf, p - buf);


/* data */
extern int log_fd;

/* int main(int argc, char **argv) */
/* { */
/* 	char buf[1024]; */
/* 	char *p = buf; */

/* 	p = PACK_COMMON_BEGIN(p, 42, "cdxpfws", 'a', 10, (uint64_t)80, */
/* 			      (uint64_t)0, 0.19, 0.33, "hello!"); */
/* 	p = PACK_COMMON_END(p, 0); */

/* 	int fd = creat("out.bin", 0644); */
/* 	if (fd == -1) { */
/* 		printf("error\n"); */
/* 		exit(1); */
/* 	} */

/* 	write(fd, buf, p - buf); */
/* 	close(fd); */

/* 	return 0; */
/* } */

#endif /* __BIN_PROTO_H__ */
