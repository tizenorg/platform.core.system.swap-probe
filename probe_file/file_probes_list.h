#ifndef __FILE_PROBES_LIST_H__
#define __FILE_PROBES_LIST_H__

#define PROBES_LIST \
	X(PROBE_NAME(open), "open") \
	X(PROBE_NAME(openat), "openat") \
	X(PROBE_NAME(creat), "creat") \
	X(PROBE_NAME(close), "close") \
	X(PROBE_NAME(lseek), "lseek") \
	X(PROBE_NAME(fsync), "fsync") \
	X(PROBE_NAME(fdatasync), "fdatasync") \
	X(PROBE_NAME(ftruncate), "ftruncate") \
	X(PROBE_NAME(fchown), "fchown") \
	X(PROBE_NAME(lockf), "lockf") \
	X(PROBE_NAME(fchmod), "fchmod") \
	X(PROBE_NAME(pread), "pread") \
	X(PROBE_NAME(read), "read") \
	X(PROBE_NAME(pwrite), "pwrite") \
	X(PROBE_NAME(write), "write") \
	X(PROBE_NAME(readv), "readv") \
	X(PROBE_NAME(fcntl), "fcntl") \
	X(PROBE_NAME(dup), "dup") \
	X(PROBE_NAME(dup2), "dup2") \
	X(PROBE_NAME(futimens), "futimens") \
	X(PROBE_NAME(fopen), "fopen") \
	X(PROBE_NAME(freopen), "freopen") \
	X(PROBE_NAME(fdopen), "fdopen") \
	X(PROBE_NAME(fflush), "fflush") \
	X(PROBE_NAME(fclose), "fclose") \
	X(PROBE_NAME(tmpfile), "tmpfile") \
	X(PROBE_NAME(fgetpos), "fgetpos") \
	X(PROBE_NAME(fseek), "fseek") \
	X(PROBE_NAME(fsetpos), "fsetpos") \
	X(PROBE_NAME(ftell), "ftell") \
	X(PROBE_NAME(rewind), "rewind") \
	X(PROBE_NAME(clearerr), "clearerr") \
	X(PROBE_NAME(feof), "feof") \
	X(PROBE_NAME(ferror), "ferror") \
	X(PROBE_NAME(fileno), "fileno") \
	X(PROBE_NAME(vfprintf), "vfprintf") \
	X(PROBE_NAME(vfscanf), "vfscanf") \
	X(PROBE_NAME(fgetc), "fgetc") \
	X(PROBE_NAME(fputc), "fputc") \
	X(PROBE_NAME(fputs), "fputs") \
	X(PROBE_NAME(getc), "getc") \
	X(PROBE_NAME(putc), "putc") \
	X(PROBE_NAME(ungetc), "ungetc") \
	X(PROBE_NAME(fread), "fread") \
	X(PROBE_NAME(fwrite), "fwrite") \
	X(PROBE_NAME(fprintf), "fprintf") \
	X(PROBE_NAME(fscanf), "fscanf") \
	X(PROBE_NAME(printf), "printf") \
	X(PROBE_NAME(scanf), "scanf") \
	X(PROBE_NAME(getchar), "getchar") \
	X(PROBE_NAME(putchar), "putchar") \
	X(PROBE_NAME(gets), "gets") \
	X(PROBE_NAME(puts), "puts") \
	X(PROBE_NAME(setbuf), "setbuf") \
	X(PROBE_NAME(setbuffer), "setbuffer") \
	X(PROBE_NAME(setlinebuf), "setlinebuf") \
	X(PROBE_NAME(setvbuf), "setvbuf")

#endif /* __FILE_PROBES_LIST_H__ */
