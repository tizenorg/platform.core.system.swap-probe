#ifndef __DA_IO_POSIX_H__
#define __DA_IO_POSIX_H__

HANDLER_WRAPPERS_DEF(int, open, const char*, path, int, oflag, ...);
HANDLER_WRAPPERS_DEF(int, openat, int, fd, const char*, path, int, oflag, ...);
HANDLER_WRAPPERS_DEF(int, creat, const char*, path, mode_t, mode);
HANDLER_WRAPPERS_DEF(int, close, int, fd);
HANDLER_WRAPPERS_DEF(off_t, lseek, int, fd, off_t, offset, int, whence);
HANDLER_WRAPPERS_DEF(int, fsync, int, fd);
HANDLER_WRAPPERS_DEF(int, fdatasync, int, fd);
HANDLER_WRAPPERS_DEF(int, ftruncate, int, fd, off_t, length);
HANDLER_WRAPPERS_DEF(int, fchown, int, fd, uid_t, owner, gid_t, group);
HANDLER_WRAPPERS_DEF(int, lockf, int, fd, int, function, off_t, size);
HANDLER_WRAPPERS_DEF(int, fchmod, int, fd, mode_t, mode);
HANDLER_WRAPPERS_DEF(ssize_t, pread, int, fd, void *, buf, size_t, nbyte, off_t, offset);
HANDLER_WRAPPERS_DEF(ssize_t, read, int, fd, void *, buf, size_t, nbyte);
HANDLER_WRAPPERS_DEF(ssize_t, pwrite, int, fd, const void *, buf, size_t, nbyte, off_t, offset);
HANDLER_WRAPPERS_DEF(ssize_t, write, int, fd, const void *, buf, size_t, nbyte);
HANDLER_WRAPPERS_DEF(ssize_t, readv, int, fd, const struct iovec *, iov, int, iovcnt);
//HANDLER_WRAPPERS_DEF(ssize_t, writev, int, fd, const struct iovec *, iov, int, iovcnt);
HANDLER_WRAPPERS_DEF(int, fcntl, int, fd, int, cmd, ...);
HANDLER_WRAPPERS_DEF(int, dup, int, fd);
HANDLER_WRAPPERS_DEF(int, dup2, int, fd, int, fd2);
//HANDLER_WRAPPERS_DEF(int, fstat, int, fd, struct stat *, buf);
HANDLER_WRAPPERS_DEF(int, futimens, int, fd, const struct timespec *, times);

#endif /* __DA_IO_POSIX_H__ */
