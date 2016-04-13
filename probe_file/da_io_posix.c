/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
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

#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dahelper.h"
#include "da_io.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <utime.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/uio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "binproto.h"
#include "real_functions.h"
#include "file_probes_list.h"
#include "probe_file.h"



static inline char *get_abs_path(int fd, const char *fname,
				 char *buf, size_t bufsiz)
{
	char *path = real_abs_path(fd, buf, bufsiz);

	if (!path)
		path = absolutize_filepath(fname, buf, bufsiz);

	return path;
}

DEF_H(int, open, const char*, path, int, oflag, va_list, args);

DEF_WV(int, open, const char*, path, int, oflag, ...)
{ 
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, open);

	va_start(args, oflag);
	ret = open_handler(orig, INTERNAL_CALL, caller, path, oflag,
			   args);
	va_end(args);

	return ret;
}

DEF_WAV(int, open, const char*, path, int, oflag, ...)
{
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, open);

	va_start(args, oflag);
	ret = open_handler(orig, EXTERNAL_CALL, caller, path,
			   oflag, args);
	va_end(args);

	return ret;
}

DEF_H(int, open, const char*, path, int, oflag, va_list, args)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*openp)(const char* path, int oflag, ...);
	int (*openp)(const char* path, int oflag, ...);
	char buffer[PATH_MAX];
	int mode = 0;

	BEFORE_ORIGINAL_FILE(open, LIBC);

	openp = (void *)orig;

	if(oflag & O_CREAT)
		mode = va_arg(args, int);

	ret = openp(path, oflag, mode);

	_filepath = get_abs_path(ret, path, buffer, PATH_MAX);

	AFTER_PACK_ORIGINAL_FD(API_ID_open, 'd', ret, 0, ret, FD_API_OPEN,
			       "s4dd", path, oflag, mode);

	return ret;
}


DEF_H(int, openat, int, fd, const char*, path, int, oflag, va_list, args);

DEF_WV(int, openat, int, fd, const char*, path, int, oflag, ...)
{
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, openat);

	va_start(args, oflag);
	ret = openat_handler(orig, INTERNAL_CALL, caller, fd, path,
			     oflag, args);
	va_end(args);

	return ret;
}

DEF_WAV(int, openat, int, fd, const char*, path, int, oflag, ...)
{
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, openat);

	va_start(args, oflag);
	ret = openat_handler(orig, EXTERNAL_CALL, caller, fd,
			     path, oflag, args);
	va_end(args);

	return ret;
}

DEF_H(int, openat, int, fd, const char*, path, int, oflag, va_list, args)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*openatp)(int fd, const char* path, int oflag, ...);
	int (*openatp)(int fd, const char* path, int oflag, ...);
	char buffer[PATH_MAX];
	int mode = 0;

	BEFORE_ORIGINAL_FILE(openat, LIBC);

	openatp = (void *)orig;

	if(oflag & O_CREAT)
		mode = va_arg(args, int);

	ret = openatp(fd, path, oflag, mode);

	_filepath = get_abs_path(ret, path, buffer, PATH_MAX);

	AFTER_PACK_ORIGINAL_FD(API_ID_openat, 'd', ret, 0, ret, FD_API_OPEN,
			       "ds4dd", fd, path, oflag, mode);

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, creat, const char*, path, mode_t, mode)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*creatp)(const char* path, mode_t mode);
	int (*creatp)(const char* path, mode_t mode);
	char buffer[PATH_MAX];

	BEFORE_ORIGINAL_FILE(creat, LIBC);

	creatp = (void *)orig;

	ret = creatp(path, mode);

	_filepath = get_abs_path(ret, path, buffer, PATH_MAX);

	AFTER_PACK_ORIGINAL_FD(API_ID_creat, 'd', ret, 0, ret, FD_API_OPEN,
			       "s4d", path, mode);

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, close, int, fd)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*closep)(int fd);
	int (*closep)(int fd);
	DECLARE_VARIABLE_FD;

//	GET_REAL_FUNC(close, LIBC);
	closep = (void *)orig;

	PRE_PROBEBLOCK_BEGIN();
	_fstatret = fstat(fd, &_statbuf);
	PRE_PROBEBLOCK_END();

	ret = closep(fd);

	POST_PACK_PROBEBLOCK_BEGIN();
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
			  API_ID_close,
			  "d", fd);
	PACK_COMMON_END('d', ret, newerrno, call_type, caller);
	POST_PACK_PROBEBLOCK_MIDDLE_FD(0, fd, FD_API_CLOSE);
	POST_PACK_PROBEBLOCK_END();

	return ret;
}

HANDLER_WRAPPERS(file_feature, off_t, lseek, int, fd, off_t, offset, int, whence)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*lseekp)(int fd, off_t offset, int whence);
	int (*lseekp)(int fd, off_t offset, int whence);
	off_t offret;

	BEFORE_ORIGINAL_FILE(lseek, LIBC);
	lseekp = (void *)orig;

	offret = lseekp(fd, offset, whence);

	AFTER_PACK_ORIGINAL_FD(API_ID_lseek,
				   'x', offret, (unsigned int)offset, fd, FD_API_OTHER,
				   "dxd", fd, (uint64_t)(offset), whence);

	return offret;
}

HANDLER_WRAPPERS(file_feature, int, fsync, int, fd)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fsyncp)(int fd);
	int (*fsyncp)(int fd);

	BEFORE_ORIGINAL_FILE(fsync, LIBC);
	fsyncp = (void *)orig;

	ret = fsyncp(fd);

	AFTER_PACK_ORIGINAL_FD(API_ID_fsync,
				   'd', ret, 0, fd, FD_API_OTHER, "d", fd);

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, fdatasync, int, fd)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fdatasyncp)(int fd);
	int (*fdatasyncp)(int fd);

	BEFORE_ORIGINAL_FILE(fdatasync, LIBC);
	fdatasyncp = (void *)orig;

	ret = fdatasyncp(fd);

	AFTER_PACK_ORIGINAL_FD(API_ID_fdatasync,
				   'd', ret, 0, fd, FD_API_OTHER, "d", fd);

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, ftruncate, int, fd, off_t, length)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*ftruncatep)(int fd, off_t length);
	int (*ftruncatep)(int fd, off_t length);

	BEFORE_ORIGINAL_FILE(ftruncate, LIBC);
	ftruncatep = (void *)orig;

	ret = ftruncatep(fd, length);

	AFTER_PACK_ORIGINAL_FD(API_ID_ftruncate,
			       'd', ret, (unsigned int)length, fd,
			       FD_API_DIRECTORY,
			       "dx", fd, (uint64_t)(length));

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, fchown, int, fd, uid_t, owner, gid_t, group)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fchownp)(int fd, uid_t owner, gid_t group);
	int (*fchownp)(int fd, uid_t owner, gid_t group);

	BEFORE_ORIGINAL_FILE(fchown, LIBC);
	fchownp = (void *)orig;

	ret = fchownp(fd, owner, group);
	AFTER_PACK_ORIGINAL_FD(API_ID_fchown, 'd', ret, 0, fd, FD_API_PERMISSION,
			  "ddd", fd, owner, group);
	return ret;
}

HANDLER_WRAPPERS(file_feature, int, lockf, int, fd, int, function, off_t, size)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*lockfp)(int fd, int function, off_t size);
	int (*lockfp)(int fd, int function, off_t size);
	int api_type = FD_API_PERMISSION;

	BEFORE_ORIGINAL_FILE(lockf, LIBC);
	lockfp = (void *)orig;
	ret = lockfp(fd, function, size);

	switch (function) {
	case F_LOCK:
	case F_TLOCK:
		api_type = FD_API_LOCK_START;
		break;
	case F_ULOCK:
		api_type = FD_API_LOCK_END;
		break;
	}

	AFTER_PACK_ORIGINAL_FD(API_ID_lockf,
			       'd', ret, (unsigned int)size, fd, api_type,
			       "ddx", fd, function, (uint64_t)(size));
	return ret;
}

HANDLER_WRAPPERS(file_feature, int, fchmod, int, fd, mode_t, mode)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fchmodp)(int fd, mode_t mode);
	int (*fchmodp)(int fd, mode_t mode);

	BEFORE_ORIGINAL_FILE(fchmod, LIBC);
	fchmodp = (void *)orig;
	ret = fchmodp(fd, mode);
	AFTER_PACK_ORIGINAL_FD(API_ID_fchmod,
				   'd', ret, 0, fd, FD_API_PERMISSION,
				   "dd", fd, mode);
	return ret;
}

// *****************************************************************
// Read / Write APIs
// *****************************************************************

HANDLER_WRAPPERS(file_feature, ssize_t, pread, int, fd, void *, buf,
		 size_t, nbyte, off_t, offset)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static ssize_t (*preadp)(int fd, void *buf, size_t nbyte, off_t offset);
	ssize_t (*preadp)(int fd, void *buf, size_t nbyte, off_t offset);
	ssize_t sret;

	BEFORE_ORIGINAL_START_END_FD(API_ID_pread, 'x', pread, LIBC, fd,
				     FD_API_READ_START, "dpxx", fd,
				     voidp_to_uint64(buf),
				     (uint64_t)(nbyte),
				     (uint64_t)(offset));

	preadp = (void *)orig;
	sret = preadp(fd, buf, nbyte, offset);

	AFTER_ORIGINAL_START_END_FD(API_ID_pread, 'x', sret, (unsigned int)sret, fd,
				    FD_API_READ_END, "dpxx", fd,
				    voidp_to_uint64(buf),
				    (uint64_t)(nbyte),
				    (uint64_t)(offset));

	return sret;
}

HANDLER_WRAPPERS(file_feature, ssize_t, read, int, fd, void *, buf, size_t, nbyte)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static ssize_t (*readp)(int fildes, void *buf, size_t nbyte);
	ssize_t (*readp)(int fildes, void *buf, size_t nbyte);
	ssize_t sret;

	BEFORE_ORIGINAL_START_END_FD(API_ID_read, 'x', read, LIBC, fd, FD_API_READ_START,
				     "dpx", fd, voidp_to_uint64(buf),
				     (uint64_t)(nbyte));

	readp = (void *)orig;

	sret = readp(fd, buf, nbyte);

	AFTER_ORIGINAL_START_END_FD(API_ID_read, 'x', sret, (unsigned int)sret, fd,
				    FD_API_READ_END, "dpx", fd,
				    voidp_to_uint64(buf),
				    (uint64_t)(nbyte));

	return sret;
}

HANDLER_WRAPPERS(file_feature, ssize_t, pwrite, int, fd, const void *, buf,
		 size_t, nbyte, off_t, offset)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static ssize_t (*pwritep)(int fd, const void *buf, size_t nbyte, off_t offset);
	ssize_t (*pwritep)(int fd, const void *buf, size_t nbyte, off_t offset);
	ssize_t sret;

	BEFORE_ORIGINAL_START_END_FD(API_ID_pwrite, 'x', pwrite, LIBC, fd, FD_API_WRITE_START,
				     "dpxx", fd, voidp_to_uint64(buf),
				     (uint64_t)(nbyte),
				     (uint64_t)(offset));

	pwritep = (void *)orig;

	sret = pwritep(fd, buf, nbyte, offset);

	DEFINE_FILESIZE_FD(fd);
	AFTER_ORIGINAL_START_END_FD(API_ID_pwrite, 'x', sret, (unsigned int)sret, fd,
				   FD_API_WRITE_END, "dpxx", fd,
				    voidp_to_uint64(buf),
				    (uint64_t)(nbyte),
				    (uint64_t)(offset));

	return sret;
}

HANDLER_WRAPPERS(file_feature, ssize_t, write, int, fd, const void *, buf,
		 size_t, nbyte)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static ssize_t (*writep)(int fildes, const void *buf, size_t nbyte);
	ssize_t (*writep)(int fildes, const void *buf, size_t nbyte);
	ssize_t sret;

	BEFORE_ORIGINAL_START_END_FD(API_ID_write, 'x', write, LIBC, fd, FD_API_WRITE_START,
				     "dpx", fd, voidp_to_uint64(buf),
				     (uint64_t)(nbyte));
	writep = (void *)orig;

	sret = writep(fd, buf, nbyte);

	DEFINE_FILESIZE_FD(fd);
	AFTER_ORIGINAL_START_END_FD(API_ID_write, 'x', sret, (unsigned int)sret, fd,
				    FD_API_WRITE_END, "dpx", fd,
				    voidp_to_uint64(buf),
				    (uint64_t)(nbyte));

	return sret;
}


HANDLER_WRAPPERS(file_feature, ssize_t, readv, int, fd,
		 const struct iovec *, iov, int, iovcnt)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static ssize_t (*readvp)(int fd, const struct iovec *iov, int iovcnt);
	ssize_t (*readvp)(int fd, const struct iovec *iov, int iovcnt);
	ssize_t sret;

	BEFORE_ORIGINAL_START_END_FD(API_ID_readv, 'x', readv, LIBC, fd, FD_API_READ_START,
				     "dpd", fd, voidp_to_uint64(iov), iovcnt);

	readvp = (void *)orig;
	sret = readvp(fd,iov,iovcnt);

	AFTER_ORIGINAL_START_END_FD(API_ID_readv, 'x', sret, (unsigned int)sret, fd,
				    FD_API_READ_END, "dpd", fd,
				    voidp_to_uint64(iov), iovcnt);

	return sret;
}

// why writev is commented ?
#if 0
HANDLER_WRAPPERS(file_feature, ssize_t, writev, int, fd, const struct iovec *, iov, int, iovcnt)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static ssize_t (*writevp)(int fd, const struct iovec *iov, int iovcnt);
	ssize_t (*writevp)(int fd, const struct iovec *iov, int iovcnt);

	MAKE_RESOURCE_PREBLOCK(writev, LIBC,3,VT_INT,fd,VT_PTR,iov,VT_INT,iovcnt);
	ssize_t ret;
	writevp = (void *)orig;
	ret = writevp(fd,iov,iovcnt);
	MAKE_RESOURCE_POSTBLOCK(VT_SSIZE_T,ret,VT_SSIZE_T,ret,VT_INT,fd, FD_API_WRITE);
	return ret;
}
#endif








// *****************************************************************
// File Attributes APIs
// *****************************************************************
DEF_H(int, fcntl, int, fd, int, cmd, va_list, args);

DEF_WV(int, fcntl, int, fd, int, cmd, ...)
{
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, fcntl);

	va_start(args, cmd);
	ret = fcntl_handler(orig, INTERNAL_CALL, caller, fd, cmd, args);
	va_end(args);

	return ret;
}

DEF_WAV(int, fcntl, int, fd, int, cmd, ...)
{
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, fcntl);

	va_start(args, cmd);
	ret = fcntl_handler(orig, EXTERNAL_CALL, caller, fd,
			    cmd, args);
	va_end(args);

	return ret;
}

DEF_H(int, fcntl, int, fd, int, cmd, va_list, args)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fcntlp)(int fd, int cmd, ...);
	int (*fcntlp)(int fd, int cmd, ...);
	int arg = 0, api_type = FD_API_OTHER;
	int64_t start = 0;
	int64_t len = 0;
	int type = 0;
	int whence = 0;

	BEFORE_ORIGINAL_FILE(fcntl, LIBC);
	fcntlp = (void *)orig;

	arg = va_arg(args, int);

	if (cmd == F_SETLK || cmd == F_SETLKW) {
		struct flock *flock = (struct flock *)arg;
		type = flock->l_type;
		whence = flock->l_whence;
		start = flock->l_start;
		len = flock->l_len;

		switch (type) {
		case F_RDLCK:
		case F_WRLCK:
			api_type = FD_API_LOCK_START;
			break;
		case F_UNLCK:
			api_type = FD_API_LOCK_RELEASE;
			break;
		}
	}

	/* before lock event */
	if (api_type == FD_API_LOCK_START)
		AFTER_PACK_ORIGINAL_FD_MIDDLE(API_ID_fcntl,
					      'd', ret, 0, fd, api_type,
					      "ddddxx", fd, cmd, type,
					      whence, start, len);

	/* call original lock function */
	ret = fcntlp(fd, cmd, arg);

	/* after lock event */
	if (api_type != FD_API_OTHER) {
		if (api_type == FD_API_LOCK_START)
			api_type = FD_API_LOCK_END;
		/* pack FD_API_LOCK_END or FD_API_UNLOCK events */
		AFTER_PACK_ORIGINAL_FD(API_ID_fcntl,
				       'd', ret, 0, fd, api_type,
				       "ddddxx", fd, cmd, type,
				       whence, start, len);
	} else {
		/* pack FD_API_OTHER */
		AFTER_PACK_ORIGINAL_FD(API_ID_fcntl,
				       'd', ret, 0, fd, api_type,
				       "ddd", fd, cmd, arg);
	}

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, dup, int, fd)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*dupp)(int fd);
	int (*dupp)(int fd);

	BEFORE_ORIGINAL_FILE(dup, LIBC);
	dupp = (void *)orig;
	ret = dupp(fd);

	AFTER_PACK_ORIGINAL_FD(API_ID_dup,
				   'd', ret, 0, ret, FD_API_OPEN, "d", fd);

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, dup2, int, fd, int, fd2)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*dup2p)(int fd, int fd2);
	int (*dup2p)(int fd, int fd2);

	BEFORE_ORIGINAL_FILE(dup2, LIBC);
	dup2p = (void *)orig;
	ret = dup2p(fd, fd2);

	AFTER_PACK_ORIGINAL_FD(API_ID_dup2,
				   'd', ret, 0, ret, FD_API_OPEN, "dd", fd, fd2);

	return ret;
}

//FIXME dlsym error
// fstat is not in LIBC
#if 0
HANDLER_WRAPPERS(file_feature, int, fstat, int, fd, struct stat *, buf)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fstatp)(int fd, struct stat *buf);
	int (*fstatp)(int fd, struct stat *buf);

	BEFORE_ORIGINAL_FILE(fstat, LIBC);
	fstatp = (void *)orig;
	ret = fstatp(fd, buf);
	AFTER_PACK_ORIGINAL_FD(ret, 0, fd, FD_API_OTHER, "dp", fd,
			       voidp_to_uint64(buf));
	return ret;
}
#endif

HANDLER_WRAPPERS(file_feature, int, futimens, int, fd,
		 const struct timespec *, times)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*futimensp)(int fd, const struct timespec times[2]);
	int (*futimensp)(int fd, const struct timespec times[2]);

	BEFORE_ORIGINAL_FILE(futimens, LIBC);
	futimensp = (void *)orig;
	ret = futimensp(fd, times);
	AFTER_PACK_ORIGINAL_FD(API_ID_futimens,
			       'd', ret, 0, fd, FD_API_OTHER, "dp", fd,
			       voidp_to_uint64(times));
	return ret;
}
