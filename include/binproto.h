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

// TODO: remove this copy-paste
#define CALLER_ADDRESS	\
	((void*) __builtin_extract_return_addr(__builtin_return_address(0)))



// COMMON
static  char *pack_int32(char *to, uint32_t val)
{
	*(uint32_t *)to = val;
	return to + sizeof(uint32_t);
}

static  char *pack_int64(char *to, uint64_t val)
{
	*(uint64_t *)to = val;
	return to + sizeof(uint64_t);
}

static  char *pack_addr(char *to, unsigned long addr)
{
	return pack_int64(to, (intptr_t)addr);
}

static  char *pack_string(char *to, const char *str)
{
	size_t len = strlen(str) + 1;
	strncpy(to, str, len);
	return to + len;
}


static  char *pack_api_id(char *to, uint32_t api_id)
{
	return pack_int32(to, api_id);
}

static  char *pack_args(char *to, const char *fmt, ...)
{
	va_list args;
	uint32_t num = strlen(fmt);
	const char *t = fmt;

	/* printf("num = %d\n", num); */

	memcpy(to, &num, sizeof(num));
	to += sizeof(num);

	memcpy(to, fmt, num);
	to += num;

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
			/* printf("char = %c\n", c); */
			memcpy(to, &c, sizeof(c));
			to += sizeof(c);
			break;
		case 'd':
			d = va_arg(args, uint32_t);
			/* printf("int = %d\n", d); */
			memcpy(to, &d, sizeof(d));
			to += sizeof(d);
			break;
		case 'x':
			x = va_arg(args, uint64_t);
			/* printf("long = %lld\n", x); */
			memcpy(to, &x, sizeof(x));
			to += sizeof(x);
			break;
		case 'p':
			p = va_arg(args, uint64_t);
			/* printf("pointer = %p\n", (void *)p); */
			memcpy(to, &p, sizeof(p));
			to += sizeof(p);
			break;
		case 'f':
			f = (float)va_arg(args, double);
			/* printf("float = %f\n", f); */
			memcpy(to, &f, sizeof(f));
			to += sizeof(f);
			break;
		case 'w':
			w = va_arg(args, double);
			/* printf("double = %e\n", w); */
			memcpy(to, &w, sizeof(w));
			to += sizeof(w);
			break;
		case 's':
			s = va_arg(args, char *);
			/* printf("string = %s\n", s); */
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

static  char *pack_pid(char *to)
{
	return pack_int32(to, getpid());
}

static  char *pack_tid(char *to)
{
	return pack_int32(to, syscall(__NR_gettid));
}

static  char *pack_return(char *to, uint64_t ret)
{
	return pack_int64(to, ret);
}

static  char *pack_pc(char *to, uint64_t pc)
{
	return pack_int64(to, pc);
}

static  char *pack_errno(char *to, uint32_t en)
{
	return pack_int32(to, en);
}

static  char *pack_internal_call(char *to)
{
	// TODO: where to get int call?
	return pack_int32(to, 0);
}

static  char *pack_caller_pc(char *to)
{
	return pack_int64(to, (uintptr_t)CALLER_ADDRESS);
}


#define PACK_COMMON_BEGIN(to, api_id, fmt, ...) \
	to = pack_api_id(to, api_id); \
	to = pack_pid(to); \
	to = pack_tid(to); \
	to = pack_args(to, fmt, __VA_ARGS__)

#define PACK_COMMON_END(to, ret, pc)	     \
	to = pack_return(to, (uintptr_t)ret); \
	to = pack_pc(to, (uintptr_t)pc);	     \
	to = pack_errno(to, (uint32_t)newerrno);     \
	to = pack_internal_call(to); \
	to = pack_caller_pc(to);

#define PACK_MEMORY(to, size, memory_api_type, addr) \
	to = pack_int32(to, size); \
	to = pack_int32(to, memory_api_type); \
	to = pack_int64(to, (uintptr_t)addr);

#define PACK_UICONTROL(to, parent_name, parent_class_name,		\
		       parent_pointer, child_name, child_class_name,	\
		       child_pointer) \
	to = pack_string(to, parent_name); \
	to = pack_string(to, parent_class_name); \
	to = pack_int64(to, parent_pointer); \
	to = pack_string(to, child_name); \
	to = pack_string(to, child_class_name); \
	to = pack_int64(to, child_pointer);

#define PACK_UIEVENT(to, event_type, detail_type, x, y, info1, info2) \
	to = pack_int32(to, event_type); \
	to = pack_int32(to, detail_type); \
	to = pack_int32(to, x); \
	to = pack_int32(to, y); \
	to = pack_int32(to, info1); \
	to = pack_int32(to, info2);

#define PACK_RESOURCE(to, size, fd_value, fd_type, fd_api_type, \
		      file_size, file_path) \
	to = pack_int32(to, size); \
	to = pack_int32(to, fd_value); \
	to = pack_int32(to, fd_type); \
	to = pack_int32(to, fd_api_type); \
	to = pack_int32(to, file_size); \
	to = pack_string(to, file_path);

#define PACK_LIFECYCLE(to)

#define PACK_SCREENSHOT(to, image_file_path, orienation) \
	to = pack_string(to, image_file_path); \
	to = pack_int32(to, orienation);

#define PACK_SCENE(to, scene_name, form_name, form_pointer,	\
		   panel_name, panel_pointer, transition_time,	\
		   user_transition_time)			\
	to = pack_string(to, scene_name); \
	to = pack_string(to, form_name); \
	to = pack_int64(to, form_pointer); \
	to = pack_string(to, panel_name); \
	to = pack_int64(to, panel_pointer); \
	to = pack_int32(to, transition_time); \
	to = pack_int32(to, user_transition_time);

#define PACK_THREAD(to, pthread_id, osp_thread_id, thread_type, api_type) \
	to = pack_int32(to, pthread_id); \
	to = pack_int32(to, osp_thread_id); \
	to = pack_int32(to, thread_type); \
	to = pack_int32(to, api_type);

#define PACK_CUSTOM(to, handle, type, name, color, value) \
	to = pack_int32(to, handle); \
	to = pack_int32(to, type); \
	to = pack_string(to, name); \
	to = pack_int32(to, color); \
	to = pack_int32(to, value);

#define PACK_SYNC(to, sync_val, sync_type, api_type) \
	to = pack_int32(to, sync_val);		     \
	to = pack_int32(to, sync_type);		     \
	to = pack_int32(to, api_type);

#define LOG_PATH "/tmp/trace.bin"
#define OPEN_LOG()				\
	log_fd = creat(LOG_PATH, 0644);		\
	if (log_fd == -1) {			\
		exit(1);			\
	}

#define CLOSE_LOG()				\
	if (log_fd > 0) {			\
		close(log_fd);			\
	}


#define LOCAL_BUF_SIZE 1024
#define PREPARE_LOCAL_BUF()			\
	char buf[LOCAL_BUF_SIZE];		\
	char *p = buf;

#define FLUSH_LOCAL_BUF()			\
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
