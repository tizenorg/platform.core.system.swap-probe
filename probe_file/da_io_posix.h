#ifndef __DA_IO_POSIX_H__
#define __DA_IO_POSIX_H__

int PROBE_NAME(open)(const char* path, int oflag, ...);
int PROBE_NAME(openat)(int fd, const char* path, int oflag, ...);
int PROBE_NAME(creat)(const char* path, mode_t mode);
int PROBE_NAME(close)(int fd);
off_t PROBE_NAME(lseek)(int fd, off_t offset, int whence);
int PROBE_NAME(fsync)(int fd);
int PROBE_NAME(fdatasync)(int fd);
int PROBE_NAME(ftruncate)(int fd, off_t length);
int PROBE_NAME(fchown)(int fd, uid_t owner, gid_t group);
int PROBE_NAME(lockf)(int fd, int function, off_t size);
int PROBE_NAME(fchmod)(int fd, mode_t mode);
ssize_t PROBE_NAME(pread)(int fd, void *buf, size_t nbyte, off_t offset);
ssize_t PROBE_NAME(read)(int fd, void *buf, size_t nbyte);
ssize_t PROBE_NAME(pwrite)(int fd, const void *buf, size_t nbyte, off_t offset);
ssize_t PROBE_NAME(write)(int fd, const void *buf, size_t nbyte);
ssize_t PROBE_NAME(readv)(int fd, const struct iovec *iov, int iovcnt);
//ssize_t PROBE_NAME(writev)(int fd, const struct iovec *iov, int iovcnt);
int PROBE_NAME(fcntl)(int fd, int cmd, ...);
int PROBE_NAME(dup)(int fd);
int PROBE_NAME(dup2)(int fd, int fd2);
//int PROBE_NAME(fstat)(int fd, struct stat *buf);
int PROBE_NAME(futimens)(int fd, const struct timespec times[2]);

#endif /* __DA_IO_POSIX_H__ */
