/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Nikita Kalyazin <n.kalyazin@samsung.com>
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
 * - Samsung RnD Institute Russia
 *
 */

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

#include <sys/socket.h>
#include "dahelper.h"
#include "api_id_mapping.h"

#define MSG_PROBE_MEMORY 0x0101
#define MSG_PROBE_UICONTROL 0x0102
#define MSG_PROBE_UIEVENT 0x0103
#define MSG_PROBE_RESOURCE 0x0104
#define MSG_PROBE_LIFECYCLE 0x0105
#define MSG_PROBE_SCREENSHOT 0x0106
#define MSG_PROBE_SCENE 0x0107
#define MSG_PROBE_THREAD 0x0108
#define MSG_PROBE_CUSTOM 0x0109
#define MSG_PROBE_SYNC 0x0110
#define MSG_PROBE_NETWORK 0x0111
#define MSG_PROBE_GL 0x0112

#define SCREENSHOT_DIRECTORY	"/tmp/da"
#define MAX_FILENAME_LEN_TO_PACK 256

// TODO: remove this copy-paste
#define CALLER_ADDRESS							\
	((void*) __builtin_extract_return_addr(__builtin_return_address(0)))

static inline uint64_t voidp_to_uint64(const void *p)
{
	return (uint64_t)(uintptr_t)p;
}

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

static char __attribute__((used)) *pack_string(char *to, const char *str)
{
	if (!str) {
		*to = '\0';
		return to + 1;
	} else {
		size_t len = strlen(str) + 1;
		strncpy(to, str, len);
		return to + len;
	}
}

static char __attribute__((used)) *pack_bin(char *to, const char *from,
					    uint32_t len)
{
	memcpy(to, from, len);
	return to + len;
}

static inline char *pack_double(char *to, double val)
{
	*(double *)to = val;
	return to + sizeof(double);
}

static inline char *pack_timestamp(char *to)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	to = pack_int32(to, tv.tv_sec);
	to = pack_int32(to, tv.tv_usec * 1000);

	return to;
}

static char __attribute__((used)) *pack_value_by_type(char *to, const char t, va_list *args)
{
	uint8_t c;
	uint32_t d;
	uint64_t x;
	uint64_t p;
	float f;
	double w;
	char *s;
	int n;

	*to++ = t;

	switch (t) {
	case 'c':
		c = (uint8_t)va_arg(*args, uint32_t);
		memcpy(to, &c, sizeof(c));
		to += sizeof(c);
		break;
	case 'd':
		d = va_arg(*args, uint32_t);
		memcpy(to, &d, sizeof(d));
		to += sizeof(d);
		break;
	case 'x':
		x = 0; // real value may be less then uint64_t
		x = (unsigned long)(uint64_t)va_arg(*args, uint64_t);
		memcpy(to, &x, sizeof(x));
		to += sizeof(x);
		break;
	case 'p':
		p = 0; // real value may be less then uint64_t
		p = (unsigned long)(uintptr_t)va_arg(*args, uint64_t);
		memcpy(to, &p, sizeof(p));
		to += sizeof(p);
		break;
	case 'f':
		f = (float)va_arg(*args, double);
		memcpy(to, &f, sizeof(f));
		to += sizeof(f);
		break;
	case 'w':
		w = va_arg(*args, double);
		memcpy(to, &w, sizeof(w));
		to += sizeof(w);
		break;
	case 's':
		s = va_arg(*args, char *);
		n = strlen(s) + 1;
		strncpy(to, s, n);
		to += n;
		break;
	case 'v':
	case 'n':
		break;
	default:
		to--;
		break;
	}

	return to;
}

static char __attribute__((used)) *pack_args(char *to, const char *fmt, ...)
{
	va_list args;
	uint32_t num = strlen(fmt);
	const char *t = fmt;

	if(*t == '\0') {
		num = 0;
	}

	memcpy(to, &num, sizeof(num));
	to += sizeof(num);

	va_start(args, fmt);

	for (t = fmt; *t != '\0'; t++)
		to = pack_value_by_type(to, *t, &args);

	va_end(args);

	return to;
}

static char __attribute__((used)) *pack_ret(char *to, char ret_type, ...)
{
	va_list args;

	va_start(args, ret_type);
	to = pack_value_by_type(to, ret_type, &args);
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
	do {	/* PACK_COMMON_BEGIN*/				\
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

#define PACK_COMMON_END(ret_type, ret, errn, intern_call)			\
	do {	/* PACK_COMMON_END */						\
		BUF_PTR = pack_ret(BUF_PTR, ret_type, (uintptr_t)ret);		\
		BUF_PTR = pack_int64(BUF_PTR, (uint64_t)errn);			\
		BUF_PTR = pack_int32(BUF_PTR, (uint32_t)intern_call);		\
		BUF_PTR = pack_int64(BUF_PTR, (uintptr_t)CALLER_ADDRESS); 	\
		BUF_PTR = pack_int32(BUF_PTR, 0);				\
		BUF_PTR = pack_int32(BUF_PTR, 0);				\
	} while (0)

#define PACK_RETURN_END(ret_type, ret)						\
		RET_PTR = pack_ret(RET_PTR, ret_type, (uintptr_t)ret);

#define PACK_MEMORY(size, memory_api_type, addr)		\
	do {	/* PACK_MEMORY */					\
		BUF_PTR = pack_int64(BUF_PTR, size);			\
		BUF_PTR = pack_int32(BUF_PTR, memory_api_type);	\
		BUF_PTR = pack_int64(BUF_PTR, (uintptr_t)addr);	\
	} while (0)

#define PACK_UICONTROL(control)						\
	do {								\
		if (unlikely(control == NULL)) {			\
			BUF_PTR = pack_string(BUF_PTR, "");		\
			BUF_PTR = pack_string(BUF_PTR, "");		\
			BUF_PTR = pack_int64(BUF_PTR, 0);		\
		} else {						\
			char *type = NULL, *name = NULL;		\
			if (find_uiobject_hash((void*)(control),	\
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
	do {	/* PACK_UIEVENT */					\
		BUF_PTR = pack_int32(BUF_PTR, event_type);		\
		BUF_PTR = pack_int32(BUF_PTR, detail_type);		\
		BUF_PTR = pack_int32(BUF_PTR, x);			\
		BUF_PTR = pack_int32(BUF_PTR, y);			\
		BUF_PTR = pack_string(BUF_PTR, info1);			\
		BUF_PTR = pack_int32(BUF_PTR, info2);			\
	} while (0)

#define PACK_RESOURCE(size, fd_value, fd_api_type, file_size,	\
		      file_path)					\
	do {	/* PACK_RESOURCE */					\
		BUF_PTR = pack_int64(BUF_PTR, size);			\
		BUF_PTR = pack_int64(BUF_PTR, fd_value);		\
		BUF_PTR = pack_int32(BUF_PTR, fd_api_type);		\
		BUF_PTR = pack_int64(BUF_PTR, file_size);		\
		BUF_PTR = pack_string(BUF_PTR, file_path);		\
	} while (0)

#define PACK_SCREENSHOT(image_file_path, orientation)				\
		do {	/* PACK_SCREENSHOT */					\
			  BUF_PTR = pack_string(BUF_PTR, image_file_path); 	\
			  BUF_PTR = pack_int32(BUF_PTR, orientation);		\
		} while (0)

#define PACK_SCENE(scene_name, formid, pform, panelid, ppanel, transition, user)	\
	do {										\
		BUF_PTR = pack_string(BUF_PTR,  scene_name);				\
		if (unlikely(pform == NULL)) {						\
			BUF_PTR = pack_string(BUF_PTR, "");				\
			BUF_PTR = pack_int64(BUF_PTR, 0);				\
		} else {								\
			char *type = NULL, *name = NULL;				\
			if (find_uiobject_hash((void*)(pform), &type, &name) == 1) {	\
				BUF_PTR = pack_string(BUF_PTR, name);			\
			} else {							\
				BUF_PTR = pack_string(BUF_PTR, "");			\
			}								\
			BUF_PTR = pack_int64(BUF_PTR, (uintptr_t)pform); 		\
		}									\
		if (unlikely(ppanel == NULL)) {						\
			BUF_PTR = pack_string(BUF_PTR, "");				\
			BUF_PTR = pack_int64(BUF_PTR, 0);				\
		} else {								\
			char *type = NULL, *name = NULL;				\
			if (find_uiobject_hash((void*)(ppanel), &type, &name) == 1) {	\
				BUF_PTR = pack_string(BUF_PTR, name);			\
			} else {							\
				BUF_PTR = pack_string(BUF_PTR, "");			\
			}								\
			BUF_PTR = pack_int64(BUF_PTR, (uintptr_t)(ppanel)); 		\
		}									\
		BUF_PTR = pack_int64(BUF_PTR, transition);				\
		BUF_PTR = pack_int64(BUF_PTR, user);					\
	} while(0)

 #define PACK_THREAD(thread_id, thread_type, api_type)			\
	 do {								\
	if(thread_type == THREAD_PTHREAD) {	                        \
	    BUF_PTR = pack_int64(BUF_PTR, thread_id);			\
	    BUF_PTR = pack_int64(BUF_PTR, 0);				\
	} else if(thread_type == THREAD_TIZENTHREAD_WORKER		\
		    || thread_type == THREAD_TIZENTHREAD_EVENTDRIVEN) {   \
	    BUF_PTR = pack_int64(BUF_PTR, 0);				\
	    BUF_PTR = pack_int64(BUF_PTR, thread_id);			\
	} else {							\
	    BUF_PTR = pack_int64(BUF_PTR, 0);				\
	    BUF_PTR = pack_int64(BUF_PTR, 0);				\
	}				                                \
	BUF_PTR = pack_int32(BUF_PTR, thread_type);	                \
	BUF_PTR = pack_int32(BUF_PTR, api_type);		        \
	} while (0)

#define PACK_CUSTOM(handle, type, name, color, value)		\
	do {						    	\
		BUF_PTR = pack_int32(BUF_PTR, handle);		\
		BUF_PTR = pack_int32(BUF_PTR, type);		\
		BUF_PTR = pack_string(BUF_PTR, name);		\
		BUF_PTR = pack_int32(BUF_PTR, color);		\
		BUF_PTR = pack_double(BUF_PTR, value);		\
	} while (0)

#define PACK_SYNC(sync_val, sync_type, api_type)		     \
	do {						     	     \
		BUF_PTR = pack_int64(BUF_PTR, (uintptr_t)sync_val);  \
		BUF_PTR = pack_int32(BUF_PTR, sync_type);	     \
		BUF_PTR = pack_int32(BUF_PTR, api_type);	     \
	} while (0)


#define MAX_SHADER_LEN (4 * 1024)
#define ADD_LOCAL_BUF_SIZE (1024)
#define MAX_LOCAL_BUF_SIZE (MAX_SHADER_LEN + ADD_LOCAL_BUF_SIZE)
#define LOCAL_BUF msg_buf

#define PREPARE_LOCAL_BUF()			\
		char LOCAL_BUF[MAX_LOCAL_BUF_SIZE];		\
		char *BUF_PTR = LOCAL_BUF;			\
		char *RET_PTR = NULL

#define MSG_LEN_OFFSET 16
#define MSG_HDR_LEN 20
#define FLUSH_LOCAL_BUF()						\
		*(uint32_t *)(msg_buf + MSG_LEN_OFFSET) = (p - msg_buf) - MSG_HDR_LEN; \
		send(gTraceInfo.socket.daemonSock, msg_buf, (p - msg_buf), 0)

// =========================== post block macro ===========================

#define POST_PACK_PROBEBLOCK_BEGIN()					\
	newerrno = errno;						\
	if(postBlockBegin(blockresult)) {

#define POST_PACK_PROBEBLOCK_END() 					\
		postBlockEnd();						\
	}								\
	errno = (newerrno != 0) ? newerrno : olderrno

/* int main(int argc, char **argv) */
/* { */
/* 	char buf[1024]; */
/* 	char *p = buf; */

/* 	p = PACK_COMMON_BEGIN(p, 42, "cdxpfws", 'a', 10, (uint64_t)80, */
/* 			      (uint64_t)0, 0.19, 0.33, "hello!"); */
/* 	p = PACK_COMMON_END('p', p, 0); */

/* 	int fd = creat("out.bin", 0644); */
/* 	if (fd == -1) { */
/* 		printf("error\n"); */
/* 		exit(1); */
/* 	} */

/* 	write(fd, buf, p - buf); */
/* 	close(fd); */

/* 	return 0; */
/* } */

// --------------------------- REDESIGN ----------------------------------

#include "pack_args.h"
#include "daprobe.h"

static char __attribute__((used)) *pack_by_type(char *to, const struct packable_t pack)
{
	if (pack.h < A_TAIL)
		to = (char *)pack_handler[pack.h](to, (void *)pack.arg);
	else
		PRINTERR("wrong function id");

	return to;
}

static char __attribute__((used)) *__pack_by_type(char *to, const enum handler h,
						void *arg)
{
	if (h < A_TAIL)
		to = (char *)pack_handler[h](to, arg);
	else
		PRINTERR("wrong function id");

	return to;
}


static char __attribute__((used)) *pack_args1(char *to, const struct packable_t packable[], uint32_t len)
{
	uint32_t i;
	struct packable_t *pack;
	memcpy(to, &len, sizeof(len));
	to += sizeof(len);

	pack = (struct packable_t *)packable;
	for (i = 0; i < len; i++) {
		*to = pack_type_char[pack->h];
		to++;
		if (pack->h < A_TAIL) {
			to = (char *)pack_handler[pack->h](to, (void *)pack->arg);
		} else {
			PRINTERR("wrong function id");
		}
		pack++;
	}

	return to;
}

static char __attribute__((used)) *pack_ret1(char *to, const struct packable_t pack)
{
	if (pack.h < A_TAIL) {
		*to = pack_type_char[pack.h];
		to++;
		to = (char *)pack_handler[pack.h](to, (void *)pack.arg);
	} else {
		PRINTERR("wrong function id");
	}

	return to;
}

static inline char *pack_p64(char *to, void *val)
{
	*(uint64_t *)to = voidp_to_uint64(val);
	return to + sizeof(uint64_t);
}

static inline char *pack_void(char *to, void __attribute__((unused))*val)
{
	return to;
}


static char __attribute__((used)) *pack_filepath(char *to, char *path)
{
	int len;

	if (path == NULL)
		path = (char *)"<NULL>";

	len = strlen(path);

	if (len > MAX_FILENAME_LEN_TO_PACK) {
		char buf[MAX_FILENAME_LEN_TO_PACK];
		memcpy(buf, path, MAX_FILENAME_LEN_TO_PACK);
		buf[MAX_FILENAME_LEN_TO_PACK-1] = '\0';
		buf[MAX_FILENAME_LEN_TO_PACK-2] = '.';
		buf[MAX_FILENAME_LEN_TO_PACK-3] = '.';
		buf[MAX_FILENAME_LEN_TO_PACK-4] = '.';
		to = pack_string(to, buf);
	} else {
		to = pack_string(to, path);
	}

	return to;
}

struct shader_info_t {
	uint32_t GL_shader_size;
	char *GL_shader;
	int index;
	uint32_t elapsed_size;
};

static char __attribute__((used)) *pack_shader(char *to, const struct shader_info_t *info)
{
	if ( (info->GL_shader_size <= MAX_SHADER_LEN) &&
	     (info->GL_shader_size <= info->elapsed_size)) {
		/* pack shaders to buffer */
		to = pack_string(to, info->GL_shader);
	} else {
		/* pack shaders to file */
		char dst_path[MAX_PATH_LENGTH];
		char dst_path_pack[MAX_PATH_LENGTH];
		FILE *file;
		sprintf(dst_path, SCREENSHOT_DIRECTORY "/%d_%d.shd",
			getpid(), info->index);
		file = fopen(dst_path, "w");
		if (file != NULL) {
			fwrite(info->GL_shader, info->GL_shader_size, 1, file);
			fclose(file);
		}
		sprintf(dst_path_pack, "FILE:%s", dst_path);
		to = pack_string(to, dst_path_pack);
	}
	return to;
}

#define PACK_COMMON_BEGIN1(msg_id, api_id, ...)		\
	do {	/* PACK_COMMON_BEGIN1 */				\
		BUF_PTR = pack_int32(BUF_PTR, msg_id);		\
		BUF_PTR = pack_int32(BUF_PTR, 0);		\
		BUF_PTR = pack_timestamp(BUF_PTR);		\
		BUF_PTR = pack_int32(BUF_PTR, 0);		\
		BUF_PTR = pack_int32(BUF_PTR, api_id);		\
		BUF_PTR = pack_int32(BUF_PTR, getpid());		\
		BUF_PTR = pack_int32(BUF_PTR, syscall(__NR_gettid));	\
		const struct packable_t args[] = __VA_ARGS__;		\
		BUF_PTR = pack_args1(BUF_PTR, args, sizeof(args)/sizeof(args[0]));	\
		RET_PTR = BUF_PTR;		\
	} while (0)

#define PACK_COMMON_END1(ret_type, ret, errn, intern_call)			\
	do {	/* PACK_COMMON_END1 */						\
		const struct packable_t result_ret = {ret_type, (void *)ret};	\
		BUF_PTR = pack_ret1(BUF_PTR, result_ret);			\
		BUF_PTR = pack_int64(BUF_PTR, (uint64_t)errn);			\
		BUF_PTR = pack_int32(BUF_PTR, (uint32_t)intern_call);		\
		BUF_PTR = pack_int64(BUF_PTR, (uintptr_t)CALLER_ADDRESS); 	\
		BUF_PTR = pack_int32(BUF_PTR, 0);				\
		BUF_PTR = pack_int32(BUF_PTR, 0);				\
	} while (0)


#endif /* __BIN_PROTO_H__ */
