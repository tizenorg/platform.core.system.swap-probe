/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
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

	AFTER_ORIGINAL_FD(0, ret, FD_API_OPEN, "%s, %d, %d", path, oflag, mode);

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

	AFTER_ORIGINAL_FD(0, ret, FD_API_OPEN, "%d, %s, %d, %d", fd, path, oflag, mode);

	return ret;
}

int creat(const char* path, mode_t mode)
{
	static int (*creatp)(const char* path, mode_t mode);

	BEFORE_ORIGINAL_FILE_NOFILTER(creat, LIBC);
	_filepath = (char*)path;

	ret = creatp(path, mode);

	AFTER_ORIGINAL_FD(0, ret, FD_API_OPEN, "%s, %d", path, mode);

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

	POST_PROBEBLOCK_BEGIN(LC_RESOURCE, VT_INT, ret,	"%d", fd);
	POST_PROBEBLOCK_MIDDLE_FD(0, fd, FD_API_CLOSE);
	POST_PROBEBLOCK_CALLSTACK_RESOURCE(FD_API_CLOSE);
	POST_PROBEBLOCK_END();

	return ret;
}

int access(const char *path, int amode)
{
	static int (*accessp)(const char *path, int amode);

	BEFORE_ORIGINAL_FILE(access, LIBC);
	_filepath = (char*)path;

	ret = accessp(path, amode);

	AFTER_ORIGINAL_NOFD(0, FD_API_PERMISSION, "%s, %d", path, amode);

	return ret;
}

int faccessat(int fd, const char *path, int amode, int flag)
{
	static int (*faccessatp)(int fd, const char *path, int amode, int flag);

	BEFORE_ORIGINAL_FILE(faccessat, LIBC);
	_filepath = (char*)path;

	ret = faccessatp(fd, path, amode, flag);

	AFTER_ORIGINAL_FD(0, fd, FD_API_PERMISSION,
			"%d, %s, %d, %d", fd, path, amode, flag);

	return ret;
}

off_t lseek(int fd, off_t offset, int whence)
{
	static int (*lseekp)(int fd, off_t offset, int whence);
	off_t offret;

	BEFORE_ORIGINAL_FILE(lseek, LIBC);

	offret = lseekp(fd, offset, whence);

	AFTER_ORIGINAL_FD_RET(VT_OFF_T, offret, (unsigned int)offset, fd, FD_API_OTHER,
			"%d, %ld, %d", fd, offset, whence);

	return offret;
}

int fsync(int fd)
{
	static int (*fsyncp)(int fd);

	BEFORE_ORIGINAL_FILE(fsync, LIBC);

	ret = fsyncp(fd);

	AFTER_ORIGINAL_FD(0, fd, FD_API_OTHER, "%d", fd);

	return ret;
}

int fdatasync(int fd)
{
	static int (*fdatasyncp)(int fd);

	BEFORE_ORIGINAL_FILE(fdatasync, LIBC);

	ret = fdatasyncp(fd);

	AFTER_ORIGINAL_FD(0, fd, FD_API_OTHER, "%d", fd);

	return ret;
}

int truncate(const char *path, off_t length)
{
	static int (*truncatep)(const char *path, off_t length);

	BEFORE_ORIGINAL_FILE(truncate, LIBC);
	_filepath = (char*)path;

	ret = truncatep(path, length);

	AFTER_ORIGINAL_NOFD((unsigned int)length, FD_API_DIRECTORY,
			"%s, %ld", path, length);

	return ret;
}

int ftruncate(int fd, off_t length)
{
	static int (*ftruncatep)(int fd, off_t length);

	BEFORE_ORIGINAL_FILE(ftruncate, LIBC);

	ret = ftruncatep(fd, length);

	AFTER_ORIGINAL_FD((unsigned int)length, fd, FD_API_DIRECTORY,
			"%d, %ld", fd, length);

	return ret;
}

#if 1
//not return FD, only make file
int mkfifo(const char *path, mode_t mode)
{
	static int (*mkfifop)(const char *path, mode_t mode);

	BEFORE_ORIGINAL_FILE(mkfifo, LIBC);
	_filepath = (char*)path;

	ret = mkfifop(path, mode);

	AFTER_ORIGINAL_NOFD(0, FD_API_OTHER, "%s, %u", path, mode);

	return ret;
}

int mkfifoat(int fd, const char *path, mode_t mode)
{
	static int (*mkfifoatp)(int fd, const char *path, mode_t mode);

	BEFORE_ORIGINAL_FILE(mkfifoat, LIBC);
	_filepath = (char*)path;

	ret = mkfifoatp(fd, path, mode);

	AFTER_ORIGINAL_FD(0, fd, FD_API_OTHER,
			"%d, %s, %u", fd, path, mode);

	return ret;
}

int mknod(const char *path, mode_t mode, dev_t dev)
{
	static int (*mknodp)(const char *path, mode_t mode, dev_t dev);

	BEFORE_ORIGINAL_FILE(mknod, LIBC);
	_filepath = (char*)path;

	ret = mknodp(path, mode, dev);

	AFTER_ORIGINAL_NOFD(0, FD_API_OTHER,
			"%s, %u, %lu", path, mode, (unsigned long int)dev);

	return ret;
}

int mknodat(int fd, const char *path, mode_t mode, dev_t dev)
{
	static int (*mknodatp)(int fd, const char *path, mode_t mode, dev_t dev);

	BEFORE_ORIGINAL_FILE(mknodat, LIBC);
	_filepath = (char*)path;

	ret = mknodatp(fd,path, mode,dev);

	AFTER_ORIGINAL_FD(0, fd, FD_API_OTHER,
			"%d, %s, %u, %lu", fd, path, mode, (unsigned long int)dev);

	return ret;
}
#endif

// ****************************************************************
// File Permission APIs
// ****************************************************************
int chown(const char *path, uid_t owner, gid_t group)
{
	static int (*chownp)(const char *path, uid_t owner, gid_t group);

	BEFORE_ORIGINAL_FILE(chown, LIBC);
	_filepath = (char*)path;
	ret = chownp(path, owner, group);
	AFTER_ORIGINAL_NOFD(0, FD_API_PERMISSION, "%s, %u, %u", path, owner, group);
	return ret;
}

int fchownat(int fd, const char *path, uid_t owner, gid_t group, int flag)
{
	static int (*fchownatp)(int fd, const char *path, uid_t owner, gid_t group, int flag);

	BEFORE_ORIGINAL_FILE(fchownat, LIBC);
	_filepath = (char*)path;
	ret = fchownatp(fd, path, owner, group, flag);
	AFTER_ORIGINAL_FD(0, fd, FD_API_PERMISSION,
			"%d, %s, %u, %u, %d", fd, path, owner, group, flag);
	return ret;
}

int fchown(int fd, uid_t owner, gid_t group)
{
	static int (*fchownp)(int fd, uid_t owner, gid_t group);

	BEFORE_ORIGINAL_FILE(fchown, LIBC);
	ret = fchownp(fd, owner, group);
	AFTER_ORIGINAL_FD(0, fd, FD_API_PERMISSION, "%d, %u, %u", fd, owner, group);
	return ret;
}

int lchown(const char *path, uid_t owner, gid_t group)
{
	static int (*lchownp)(const char *path, uid_t owner, gid_t group);

	BEFORE_ORIGINAL_FILE(lchown, LIBC);
	_filepath = (char*)path;
	ret = lchownp(path, owner, group);
	AFTER_ORIGINAL_NOFD(0, FD_API_PERMISSION, "%s, %u, %u", path, owner, group);
	return ret;
}

int lockf(int fd, int function, off_t size)
{
	static int (*lockfp)(int fd, int function, off_t size);

	BEFORE_ORIGINAL_FILE(lockf, LIBC);
	ret = lockfp(fd, function, size);
	AFTER_ORIGINAL_FD((unsigned int)size, fd, FD_API_PERMISSION,
			"%d, %d, %ld", fd, function, size);
	return ret;
}

int chmod(const char *path, mode_t mode)
{
	static int (*chmodp)(const char *path, mode_t mode);

	BEFORE_ORIGINAL_FILE(chmod, LIBC);
	_filepath = (char*)path;
	ret = chmodp(path, mode);
	AFTER_ORIGINAL_NOFD(0, FD_API_PERMISSION, "%s, %u", path, mode);
	return ret;
}

int fchmodat(int fd, const char *path, mode_t mode, int flag)
{
	static int (*fchmodatp)(int fd, const char *path, mode_t mode, int flag);

	BEFORE_ORIGINAL_FILE(fchmodat, LIBC);
	_filepath = (char*)path;
	ret = fchmodatp(fd, path, mode, flag);
	AFTER_ORIGINAL_FD(0, fd, FD_API_PERMISSION,
			"%d, %s, %u, %d", fd, path, mode, flag);
	return ret;
}

int fchmod(int fd, mode_t mode)
{
	static int (*fchmodp)(int fd, mode_t mode);

	BEFORE_ORIGINAL_FILE(fchmod, LIBC);
	ret = fchmodp(fd, mode);
	AFTER_ORIGINAL_FD(0, fd, FD_API_PERMISSION, "%d, %u", fd, mode);
	return ret;
}

// *****************************************************************
// Read / Write APIs
// *****************************************************************

ssize_t pread(int fd, void *buf, size_t nbyte, off_t offset)
{
	static ssize_t (*preadp)(int fd, void *buf, size_t nbyte, off_t offset);
	ssize_t sret;

	BEFORE_ORIGINAL_FILE(pread, LIBC);

	sret = preadp(fd, buf, nbyte, offset);

	AFTER_ORIGINAL_FD_RET(VT_SSIZE_T, sret, (unsigned int)sret, fd, FD_API_READ,
			"%d, %p, %u, %ld", fd, buf, nbyte, offset);

	return sret;
}
ssize_t read(int fd, void *buf, size_t nbyte)
{
	static ssize_t (*readp)(int fildes, void *buf, size_t nbyte);
	ssize_t sret;

	BEFORE_ORIGINAL_FILE(read, LIBC);

	sret = readp(fd, buf, nbyte);

	AFTER_ORIGINAL_FD_RET(VT_SSIZE_T, sret, (unsigned int)sret, fd, FD_API_READ,
			"%d, %p, %u", fd, buf, nbyte);

	return sret;
}

ssize_t pwrite(int fd, const void *buf, size_t nbyte, off_t offset)
{
	static ssize_t (*pwritep)(int fd, const void *buf, size_t nbyte, off_t offset);
	ssize_t sret;

	BEFORE_ORIGINAL_FILE(pwrite, LIBC);

	sret = pwritep(fd, buf, nbyte, offset);

	AFTER_ORIGINAL_FD_RET(VT_SSIZE_T, sret, (unsigned int)sret, fd, FD_API_WRITE,
			"%d, %p, %u, %ld", fd, buf, nbyte, offset);

	return sret;
}

ssize_t write(int fd, const void *buf, size_t nbyte)
{
	static ssize_t (*writep)(int fildes, const void *buf, size_t nbyte);
	ssize_t sret;

	BEFORE_ORIGINAL_FILE(write, LIBC);

	sret = writep(fd, buf, nbyte);

	AFTER_ORIGINAL_FD_RET(VT_SSIZE_T, sret, (unsigned int)sret, fd, FD_API_WRITE,
			"%d, %p, %u", fd, buf, nbyte);

	return sret;
}


ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{
	static ssize_t (*readvp)(int fd, const struct iovec *iov, int iovcnt);
	ssize_t sret;

	BEFORE_ORIGINAL_FILE(readv, LIBC);
	sret = readvp(fd,iov,iovcnt);

	AFTER_ORIGINAL_FD_RET(VT_SSIZE_T, sret, (unsigned int)sret, fd, FD_API_READ,
			"%d, %p, %d", fd, iov, iovcnt);

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
// Directory I/O
// *****************************************************************

int rmdir(const char *path)
{
	static int (*rmdirp)(const char *path);

	BEFORE_ORIGINAL_FILE(rmdir, LIBC);
	_filepath = (char*)path;
	ret = rmdirp(path);
	AFTER_ORIGINAL_NOFD(0, FD_API_DIRECTORY, "%s", path);
	return ret;
}

int fchdir(int fd)
{
	static int (*fchdirp)(int fd);

	BEFORE_ORIGINAL_FILE(fchdir, LIBC);
	ret = fchdirp(fd);
	AFTER_ORIGINAL_FD(0, fd, FD_API_DIRECTORY, "%d", fd);
	return ret;
}

int chdir(const char *path)
{
	static int (*chdirp)(const char *path);

	BEFORE_ORIGINAL_FILE(chdir, LIBC);
	_filepath = (char*)path;
	ret = chdirp(path);
	AFTER_ORIGINAL_NOFD(0, FD_API_DIRECTORY, "%s", path);
	return ret;
}

int link(const char *path1, const char *path2)
{
	static int (*linkp)(const char *path1, const char *path2);

	BEFORE_ORIGINAL_FILE(link, LIBC);
	_filepath = (char*)path1;
	ret = linkp(path1, path2);
	AFTER_ORIGINAL_NOFD(0, FD_API_DIRECTORY, "%s, %s", path1, path2);
	return ret;
}

int linkat(int fd1, const char *path1, int fd2, const char *path2, int flag)
{
	static int (*linkatp)(int fd1, const char *path1, int fd2, const char *path2, int flag);

	BEFORE_ORIGINAL_FILE(linkat, LIBC);
	_filepath = (char*)path1;
	ret = linkatp(fd1, path1, fd2, path2, flag);
	AFTER_ORIGINAL_FD(0, fd2, FD_API_DIRECTORY,
			"%d, %s, %d, %s, %d", fd1, path2, fd2, path2, flag);
	return ret;
}

int unlink(const char *path)
{
	static int (*unlinkp)(const char *path);

	BEFORE_ORIGINAL_FILE(unlink, LIBC);
	_filepath = (char*)path;
	ret = unlinkp(path);
	AFTER_ORIGINAL_NOFD(0, FD_API_DIRECTORY, "%s", path);
	return ret;
}

int unlinkat(int fd, const char *path, int flag)
{
	static int (*unlinkatp)(int fd, const char *path, int flag);

	BEFORE_ORIGINAL_FILE(unlinkat, LIBC);
	_filepath = (char*)path;
	ret = unlinkatp(fd, path, flag);
	AFTER_ORIGINAL_FD(0, fd, FD_API_DIRECTORY, "%d, %s, %d", fd, path, flag);
	return ret;
}

int symlink(const char *path1, const char *path2)
{
	static int (*symlinkp)(const char *path1, const char *path2);

	BEFORE_ORIGINAL_FILE(symlink, LIBC);
	_filepath = (char*)path1;
	ret = symlinkp(path1, path2);
	AFTER_ORIGINAL_NOFD(0, FD_API_DIRECTORY, "%s, %s", path1, path2);
	return ret;
}

int symlinkat(const char *path1, int fd, const char *path2)
{
	static int (*symlinkatp)(const char *path1, int fd, const char *path2);

	BEFORE_ORIGINAL_FILE(symlinkat, LIBC);
	_filepath = (char*)path1;
	ret = symlinkatp(path1, fd, path2);
	AFTER_ORIGINAL_FD(0, fd, FD_API_DIRECTORY, "%s, %d, %s", path1, fd, path2);
	return ret;
}

ssize_t readlink(const char* path, char* buf, size_t bufsize)
{
	static int (*readlinkp)(const char* path, char* buf, size_t bufsize);
	ssize_t sret;

	BEFORE_ORIGINAL_FILE(readlink, LIBC);
	_filepath = (char*)path;

	sret = readlinkp(path, buf, bufsize);

	AFTER_ORIGINAL_NOFD_RET(VT_SSIZE_T, sret, bufsize, FD_API_DIRECTORY,
			"%s, %s, %u", path, buf, bufsize);

	return sret;
}

ssize_t readlinkat(int fd, const char * path, char * buf, size_t bufsize)
{
	static int (*readlinkatp)(int fd, const char * path, char * buf, size_t bufsize);
	ssize_t sret;

	BEFORE_ORIGINAL_FILE(readlinkat, LIBC);
	_filepath = (char*)path;

	sret = readlinkatp(fd, path, buf, bufsize);

	AFTER_ORIGINAL_FD_RET(VT_SSIZE_T, sret, bufsize, fd, FD_API_DIRECTORY,
			"%d, %s, %s, %u", fd, path, buf, bufsize);

	return sret;
}

int mkdir(const char *path, mode_t mode)
{
	static int (*mkdirp)(const char *path, mode_t mode);

	BEFORE_ORIGINAL_FILE(mkdir, LIBC);
	_filepath = (char*)path;
	ret = mkdirp(path, mode);
	AFTER_ORIGINAL_NOFD(0, FD_API_DIRECTORY, "%s, %u", path, mode);
	return ret;
}

int mkdirat(int fd, const char *path, mode_t mode)
{
	static int (*mkdiratp)(int fd, const char *path, mode_t mode);

	BEFORE_ORIGINAL_FILE(mkdirat, LIBC);
	_filepath = (char*)path;
	ret = mkdiratp(fd, path, mode);
	AFTER_ORIGINAL_FD(0, fd, FD_API_DIRECTORY, "%d, %s, %u", fd, path, mode);
	return ret;
}

int closedir(DIR *dirp)
{
	static int (*closedirp)(DIR *dirp);

	BEFORE_ORIGINAL_FILE(closedir, LIBC);
	ret = closedirp(dirp);
	AFTER_ORIGINAL_NOFD(0, FD_API_DIRECTORY, "%p", dirp);
	return ret;
}

DIR *fdopendir(int fd)
{
	static DIR* (*fdopendirp)(int fd);
	DIR* dret;

	BEFORE_ORIGINAL_FILE(fdopendir, LIBC);

	dret = fdopendirp(fd);

	AFTER_ORIGINAL_FD_RET(VT_PTR, dret, 0, fd, FD_API_DIRECTORY, "%d", fd);

	return dret;
}

DIR *opendir(const char *dirname)
{
	static DIR* (*opendirp)(const char *dirname);
	DIR* dret;

	BEFORE_ORIGINAL_FILE(opendir, LIBC);
	_filepath = (char*)dirname;

	dret = opendirp(dirname);

	AFTER_ORIGINAL_NOFD_RET(VT_PTR, dret, 0, FD_API_DIRECTORY, "%s", dirname);

	return dret;
}

struct dirent *readdir(DIR *dirp)
{
	static struct dirent* (*readdirp)(DIR *dirp);
	struct dirent* dret;

	BEFORE_ORIGINAL_FILE(readdir, LIBC);

	dret = readdirp(dirp);
	
	AFTER_ORIGINAL_NOFD_RET(VT_PTR, dret, 0, FD_API_DIRECTORY, "%p", dirp);

	return dret;
}

int readdir_r(DIR * dirp, struct dirent * entry, struct dirent ** result)
{
	static int (*readdir_rp)(DIR * dirp, struct dirent * entry, struct dirent ** result);

	BEFORE_ORIGINAL_FILE(readdir_r, LIBC);
	ret = readdir_rp(dirp, entry, result);
	AFTER_ORIGINAL_NOFD(0, FD_API_DIRECTORY, "%p, %p, %p", dirp, entry, result);
	return ret;
}

void rewinddir(DIR *dirp)
{
	static void (*rewinddirp)(DIR *dirp);

	BEFORE_ORIGINAL_FILE(rewinddir, LIBC);

	rewinddirp(dirp);
	
	AFTER_ORIGINAL_NOFD_RET(VT_NULL, NULL, 0, FD_API_DIRECTORY, "%p", dirp);
}

void seekdir(DIR *dirp, long loc)
{
	static void (*seekdirp)(DIR *dirp, long loc);

	BEFORE_ORIGINAL_FILE(seekdir, LIBC);

	seekdirp(dirp, loc);

	AFTER_ORIGINAL_NOFD_RET(VT_NULL, NULL, 0, FD_API_DIRECTORY,
			"%p, %ld", dirp, loc);
}

long telldir(DIR *dirp)
{
	static int (*telldirp)(DIR *dirp);
	long lret;

	BEFORE_ORIGINAL_FILE(telldir, LIBC);

	lret = telldirp(dirp);

	AFTER_ORIGINAL_NOFD_RET(VT_LONG, lret, 0, FD_API_DIRECTORY, "%p", dirp);

	return lret;
}

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

	AFTER_ORIGINAL_FD(0, fd, FD_API_OTHER, "%d, %d, %d", fd, cmd, arg);

	return ret;
}

int dup(int fd)
{
	static int (*dupp)(int fd);

	BEFORE_ORIGINAL_FILE_NOFILTER(dup, LIBC);

	ret = dupp(fd);

	AFTER_ORIGINAL_FD(0, ret, FD_API_OPEN, "%d", fd);

	return ret;
}

int dup2(int fd, int fd2)
{
	static int (*dup2p)(int fd, int fd2);

	BEFORE_ORIGINAL_FILE_NOFILTER(dup2, LIBC);

	ret = dup2p(fd, fd2);

	AFTER_ORIGINAL_FD(0, ret, FD_API_OPEN, "%d, %d", fd, fd2);

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
	AFTER_ORIGINAL_FD(0, fd, FD_API_OTHER, "%d, %p", fd, buf);
	return ret;
}

int stat(const char * path, struct stat * buf)
{
	static int (*statp)(const char * path, struct stat * buf);

	BEFORE_ORIGINAL_FILE(stat, LIBC);
	_filepath = (char*)path;
	ret = statp(path, buf);
	AFTER_ORIGINAL_NOFD(0, FD_API_OTHER, "%s, %p", path, buf);
	return ret;
}

#endif

int fstatat(int fd, const char * path, struct stat * buf, int flag)
{
	static int (*fstatatp)(int fd, const char * path, struct stat * buf, int flag);

	BEFORE_ORIGINAL_FILE(fstatat, LIBC);
	_filepath = (char*)path;
	ret = fstatatp(fd, path, buf, flag);
	AFTER_ORIGINAL_FD(0, fd, FD_API_OTHER, "%d, %s, %p, %d", fd, path, buf, flag);
	return ret;
}

int lstat(const char * path, struct stat * buf)
{
	static int (*lstatp)(const char * path, struct stat * buf);

	BEFORE_ORIGINAL_FILE(lstat, LIBC);
	_filepath = (char*)path;
	ret = lstatp(path, buf);
	AFTER_ORIGINAL_NOFD(0, FD_API_OTHER, "%s, %p", path, buf);
	return ret;
}

int futimens(int fd, const struct timespec times[2])
{
	static int (*futimensp)(int fd, const struct timespec times[2]);

	BEFORE_ORIGINAL_FILE(futimens, LIBC);
	ret = futimensp(fd, times);
	AFTER_ORIGINAL_FD(0, fd, FD_API_OTHER, "%d, %p", fd, times);
	return ret;
}

int utimensat(int fd, const char *path, const struct timespec times[2], int flag)
{
	static int (*utimensatp)(int fd, const char *path, const struct timespec times[2], int flag);

	BEFORE_ORIGINAL_FILE(utimensat, LIBC);
	_filepath = (char*)path;
	ret = utimensatp(fd, path, times, flag);
	AFTER_ORIGINAL_FD(0, fd, FD_API_OTHER,
			"%d, %s, %p, %d", fd, path, times, flag);
	return ret;
}

int utimes(const char *path, const struct timeval times[2])
{
	static int (*utimesp)(const char *path, const struct timeval times[2]);

	BEFORE_ORIGINAL_FILE(utimes, LIBC);
	_filepath = (char*)path;
	ret = utimesp(path, times);
	AFTER_ORIGINAL_NOFD(0, FD_API_OTHER, "%s, %p", path, times);
	return ret;
}

int utime(const char *path, const struct utimbuf *times)
{
	static int (*utimep)(const char *path, const struct utimbuf *times);

	BEFORE_ORIGINAL_FILE(utime, LIBC);
	_filepath = (char*)path;
	ret = utimep(path, times);
	AFTER_ORIGINAL_NOFD(0, FD_API_OTHER, "%s, %p", path, times);
	return ret;
}


