#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
//#include "da_io_posix.h"
//#include "da_io_stdc.h"

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
	X(PROBE_NAME(writev), "writev") \
	X(PROBE_NAME(fcntl), "fcntl") \
	X(PROBE_NAME(dup), "dup") \
	X(PROBE_NAME(dup2), "dup2") \
	X(PROBE_NAME(fstat), "fstat") \
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
	X(PROBE_NAME(fgets), "fgets") \
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



/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
#define X(func_name, orig_name) { & func_name, orig_name },

static struct probe_desc_t file_probes[] = {
	PROBES_LIST
};

#undef X

/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

static struct feature_desc_t feature = {
	.feature = FL_FILE_API_PROBING,
	.cnt = (PROBES_LIST),
	.probes = file_probes
};

static struct feature_desc_t feature_always = {
	.feature = FL_FILE_API_ALWAYS_PROBING,
	.cnt = (PROBES_LIST),
	.probes = file_probes
};

#undef X

#undef PROBES_LIST

struct feature_desc_t *file_feature = &feature;
struct feature_desc_t *file_feature_always = &feature_always;
