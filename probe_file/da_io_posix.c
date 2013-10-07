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
#include "dautil.h"
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

static enum DaOptions _sopt = OPT_FILE;

int open(const char* path, int oflag, ...)
{
	static int (*openp)(const char* path, int oflag, ...);
	int mode = 0;

	BEFORE_ORIGINAL_FILE_NOFILTER(open, LIBC);
	_filepath = (char*)path;

	if(oflag & O_CREAT)
	{
		va_list arg;
		va_start(arg, oflag);
		mode = va_arg(arg, int);
		va_end(arg);
	}

	ret = openp(path, oflag, mode);

	AFTER_PACK_ORIGINAL_FD(API_ID_open,
				   ret, 0, ret, FD_API_OPEN, "sdd", path, oflag, mode);

	return ret;
}

int openat(int fd, const char* path, int oflag, ...)
{
	static int (*openatp)(int fd, const char* path, int oflag, ...);
	int mode = 0;

	BEFORE_ORIGINAL_FILE_NOFILTER(openat, LIBC);
	_filepath = (char*)path;

	if(oflag & O_CREAT)
	{
		va_list arg;
		va_start(arg, oflag);
		mode = va_arg(arg, int);
		va_end(arg);
	}

	ret = openatp(fd, path, oflag, mode);

	AFTER_PACK_ORIGINAL_FD(API_ID_openat,
				   ret, 0, ret, FD_API_OPEN, "dsdd", fd, path, oflag, mode);

	return ret;
}

int creat(const char* path, mode_t mode)
{
	static int (*creatp)(const char* path, mode_t mode);

	BEFORE_ORIGINAL_FILE_NOFILTER(creat, LIBC);
	_filepath = (char*)path;

	ret = creatp(path, mode);

	AFTER_PACK_ORIGINAL_FD(API_ID_creat,
				   ret, 0, ret, FD_API_OPEN, "sd", path, mode);

	return ret;
}

int close(int fd)
{
	static int (*closep)(int fd);
	DECLARE_VARIABLE_FD;

	GET_REAL_FUNC(close, LIBC);

	bfiltering = false;
	PRE_PROBEBLOCK_BEGIN();
	_fstatret = fstat(fd, &_statbuf);
	PRE_PROBEBLOCK_END();

	ret = closep(fd);

	POST_PACK_PROBEBLOCK_BEGIN();
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
			  API_ID_close,
			  "d", fd);
	PACK_COMMON_END(ret, newerrno, blockresult);
	POST_PACK_PROBEBLOCK_MIDDLE_FD(0, fd, FD_API_CLOSE);
	POST_PACK_PROBEBLOCK_END();

	return ret;
}

off_t lseek(int fd, off_t offset, int whence)
{
	static int (*lseekp)(int fd, off_t offset, int whence);
	off_t offret;

	BEFORE_ORIGINAL_FILE(lseek, LIBC);

	offret = lseekp(fd, offset, whence);

	AFTER_PACK_ORIGINAL_FD(API_ID_lseek,
				   offret, (unsigned int)offset, fd, FD_API_OTHER,
				   "dxd", fd, offset, whence);

	return offret;
}

int fsync(int fd)
{
	static int (*fsyncp)(int fd);

	BEFORE_ORIGINAL_FILE(fsync, LIBC);

	ret = fsyncp(fd);

	AFTER_PACK_ORIGINAL_FD(API_ID_fsync,
				   ret, 0, fd, FD_API_OTHER, "d", fd);

	return ret;
}

int fdatasync(int fd)
{
	static int (*fdatasyncp)(int fd);

	BEFORE_ORIGINAL_FILE(fdatasync, LIBC);

	ret = fdatasyncp(fd);

	AFTER_PACK_ORIGINAL_FD(API_ID_fdatasync,
				   ret, 0, fd, FD_API_OTHER, "d", fd);

	return ret;
}



int ftruncate(int fd, off_t length)
{
	static int (*ftruncatep)(int fd, off_t length);

	BEFORE_ORIGINAL_FILE(ftruncate, LIBC);

	ret = ftruncatep(fd, length);

	AFTER_PACK_ORIGINAL_FD(API_ID_ftruncate,
				   ret, (unsigned int)length, fd, FD_API_DIRECTORY, "dx", fd, length);

	return ret;
}

int fchown(int fd, uid_t owner, gid_t group)
{
	static int (*fchownp)(int fd, uid_t owner, gid_t group);

	BEFORE_ORIGINAL_FILE(fchown, LIBC);
	ret = fchownp(fd, owner, group);
	AFTER_PACK_ORIGINAL_FD(API_ID_fchown, ret, 0, fd, FD_API_PERMISSION,
			  "ddd", fd, owner, group);
	return ret;
}




int lockf(int fd, int function, off_t size)
{
	static int (*lockfp)(int fd, int function, off_t size);

	BEFORE_ORIGINAL_FILE(lockf, LIBC);
	ret = lockfp(fd, function, size);
	AFTER_PACK_ORIGINAL_FD(API_ID_lockf,
				   ret, (unsigned int)size, fd, FD_API_PERMISSION,
				   "ddx", fd, function, size);
	return ret;
}



int fchmod(int fd, mode_t mode)
{
	static int (*fchmodp)(int fd, mode_t mode);

	BEFORE_ORIGINAL_FILE(fchmod, LIBC);
	ret = fchmodp(fd, mode);
	AFTER_PACK_ORIGINAL_FD(API_ID_fchmod,
				   ret, 0, fd, FD_API_PERMISSION, "dd", fd, mode);
	return ret;
}

// *****************************************************************
// Read / Write APIs
// *****************************************************************

ssize_t pread(int fd, void *buf, size_t nbyte, off_t offset)
{
	static ssize_t (*preadp)(int fd, void *buf, size_t nbyte, off_t offset);
	ssize_t sret;

	BEFORE_ORIGINAL_START_END_FD(API_ID_pread, pread, LIBC, fd,
				FD_API_READ_START, "dpxx", fd, buf, nbyte, offset);

	sret = preadp(fd, buf, nbyte, offset);

	AFTER_ORIGINAL_START_END_FD(API_ID_pread, sret, (unsigned int)sret, fd,
				FD_API_READ_END, "dpxx", fd, buf, nbyte, offset);

	return sret;
}
ssize_t read(int fd, void *buf, size_t nbyte)
{
	static ssize_t (*readp)(int fildes, void *buf, size_t nbyte);
	ssize_t sret;

	BEFORE_ORIGINAL_START_END_FD(API_ID_read, read, LIBC, fd, FD_API_READ_START,
				"dpx", fd, buf, nbyte);

	sret = readp(fd, buf, nbyte);

	AFTER_ORIGINAL_START_END_FD(API_ID_read, sret, (unsigned int)sret, fd,
				FD_API_READ_END, "dpx", fd, buf, nbyte);

	return sret;
}

ssize_t pwrite(int fd, const void *buf, size_t nbyte, off_t offset)
{
	static ssize_t (*pwritep)(int fd, const void *buf, size_t nbyte, off_t offset);
	ssize_t sret;

	BEFORE_ORIGINAL_START_END_FD(API_ID_pwrite, pwrite, LIBC, fd, FD_API_WRITE_START,
				 "dpxx", fd, buf, nbyte, offset);

	sret = pwritep(fd, buf, nbyte, offset);

	DEFINE_FILESIZE_FD(fd);
	AFTER_ORIGINAL_START_END_FD(API_ID_pwrite, sret, (unsigned int)sret, fd,
				   FD_API_WRITE_END, "dpxx", fd, buf, nbyte, offset);

	return sret;
}

ssize_t write(int fd, const void *buf, size_t nbyte)
{
	static ssize_t (*writep)(int fildes, const void *buf, size_t nbyte);
	ssize_t sret;

	BEFORE_ORIGINAL_START_END_FD(API_ID_write, write, LIBC, fd, FD_API_WRITE_START,
				 "dpx", fd, buf, nbyte);

	sret = writep(fd, buf, nbyte);

	DEFINE_FILESIZE_FD(fd);
	AFTER_ORIGINAL_START_END_FD(API_ID_write, sret, (unsigned int)sret, fd,
				FD_API_WRITE_END, "dpx", fd, buf, nbyte);

	return sret;
}


ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{
	static ssize_t (*readvp)(int fd, const struct iovec *iov, int iovcnt);
	ssize_t sret;

	BEFORE_ORIGINAL_START_END_FD(API_ID_readv, readv, LIBC, fd, FD_API_READ_START,
				 "dpd", fd, iov, iovcnt);

	sret = readvp(fd,iov,iovcnt);

	AFTER_ORIGINAL_START_END_FD(API_ID_readv, sret, (unsigned int)sret, fd,
				FD_API_READ_END, "dpd", fd, iov, iovcnt);

	return sret;
}

// why writev is commented ?
#if 0
ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
	static ssize_t (*writevp)(int fd, const struct iovec *iov, int iovcnt);

	MAKE_RESOURCE_PREBLOCK(writev, LIBC,3,VT_INT,fd,VT_PTR,iov,VT_INT,iovcnt);
	ssize_t ret;
	ret = writevp(fd,iov,iovcnt);
	MAKE_RESOURCE_POSTBLOCK(VT_SSIZE_T,ret,VT_SSIZE_T,ret,VT_INT,fd, FD_API_WRITE);
	return ret;
}
#endif








// *****************************************************************
// File Attributes APIs
// *****************************************************************

int fcntl(int fd, int cmd, ...)
{
	static int (*fcntlp)(int fd, int cmd, ...);
	int arg = 0;

	BEFORE_ORIGINAL_FILE(fcntl, LIBC);

	va_list argl;
	va_start(argl, cmd);
	arg = va_arg(argl, int);
	va_end(argl);

	ret = fcntlp(fd, cmd, arg);

	AFTER_PACK_ORIGINAL_FD(API_ID_fcntl,
				   ret, 0, fd, FD_API_OTHER, "ddd", fd, cmd, arg);

	return ret;
}

int dup(int fd)
{
	static int (*dupp)(int fd);

	BEFORE_ORIGINAL_FILE_NOFILTER(dup, LIBC);

	ret = dupp(fd);

	AFTER_PACK_ORIGINAL_FD(API_ID_dup,
				   ret, 0, ret, FD_API_OPEN, "d", fd);

	return ret;
}

int dup2(int fd, int fd2)
{
	static int (*dup2p)(int fd, int fd2);

	BEFORE_ORIGINAL_FILE_NOFILTER(dup2, LIBC);

	ret = dup2p(fd, fd2);

	AFTER_PACK_ORIGINAL_FD(API_ID_dup2,
				   ret, 0, ret, FD_API_OPEN, "dd", fd, fd2);

	return ret;
}

//FIXME dlsym error
// fstat is not in LIBC
#if 0
int fstat(int fd, struct stat *buf)
{
	static int (*fstatp)(int fd, struct stat *buf);

	BEFORE_ORIGINAL_FILE(fstat, LIBC);
	ret = fstatp(fd, buf);
	AFTER_PACK_ORIGINAL_FD(ret, 0, fd, FD_API_OTHER, "dp", fd, buf);
	return ret;
}
#endif

int futimens(int fd, const struct timespec times[2])
{
	static int (*futimensp)(int fd, const struct timespec times[2]);

	BEFORE_ORIGINAL_FILE(futimens, LIBC);
	ret = futimensp(fd, times);
	AFTER_PACK_ORIGINAL_FD(API_ID_futimens,
				   ret, 0, fd, FD_API_OTHER, "dp", fd, times);
	return ret;
}
