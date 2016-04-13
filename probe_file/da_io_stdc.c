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
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dahelper.h"
#include "da_io.h"

#include "binproto.h"
#include "real_functions.h"
#include "aliases.h"
#include "file_probes_list.h"
#include "probe_file.h"



static inline char *get_abs_path(FILE *file, const char *fname,
				 char *buf, size_t bufsiz)
{
	char *path = file ? real_abs_path(fileno(file), buf, bufsiz): NULL;

	if (!path)
		path = absolutize_filepath(fname, buf, bufsiz);

	return path;
}

HANDLER_WRAPPERS(file_feature, FILE*, fopen, const char*, filename,
		 const char*, mode)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static FILE* (*fopenp)(const char* filename, const char* mode);
	FILE* (*fopenp)(const char* filename, const char* mode);
	char buffer[PATH_MAX];
	FILE* fret;

	BEFORE_ORIGINAL_FILE(fopen, LIBC);
	fopenp = (void *)orig;
	fret = fopenp(filename, mode);

	_filepath = get_abs_path(fret, filename, buffer, PATH_MAX);

	AFTER_PACK_ORIGINAL_FILEP(API_ID_fopen,
				  'p', fret, 0, fret, FD_API_OPEN, "s4s",
				  filename, mode);

	return fret;
}

HANDLER_WRAPPERS(file_feature, FILE*, freopen, const char *, filename, const char *, mode, FILE *, stream)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static FILE* (*freopenp)(const char *filename, const char *mode,
//				 FILE *stream);
	FILE* (*freopenp)(const char *filename, const char *mode, FILE *stream);
	char buffer[PATH_MAX];
 	FILE* fret;

	BEFORE_ORIGINAL_FILE(freopen, LIBC);
	freopenp = (void *)orig;
	fret = freopenp(filename, mode, stream);

	_filepath = get_abs_path(fret, filename, buffer, PATH_MAX);

	AFTER_PACK_ORIGINAL_FILEP(API_ID_freopen, 'p', fret, 0, fret, FD_API_OPEN,
				  "s4sp", filename, mode,
				  voidp_to_uint64(stream));

	return fret;
}

HANDLER_WRAPPERS(file_feature, FILE*, fdopen, int, fildes, const char *, mode)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static FILE* (*fdopenp)(int fildes, const char *mode);
	FILE* (*fdopenp)(int fildes, const char *mode);
 	FILE* fret;

	BEFORE_ORIGINAL_FILE(fdopen, LIBC);
	fdopenp = (void *)orig;
	fret = fdopenp(fildes, mode);

	AFTER_PACK_ORIGINAL_FILEP(API_ID_fdopen,
				  'p', fret, 0, fret, FD_API_OPEN, "ds", fildes, mode);

	return fret;
}

HANDLER_WRAPPERS(file_feature, int, fflush, FILE*, stream)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fflushp)(FILE* stream);
	int (*fflushp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(fflush, LIBC);
	fflushp = (void *)orig;
	ret = fflushp(stream);
	AFTER_PACK_ORIGINAL_FILEP(API_ID_fflush,
				  'd', ret, 0, stream, FD_API_OTHER, "p",
				  voidp_to_uint64(stream));
	return ret;
}

HANDLER_WRAPPERS(file_feature, int, fclose, FILE*, stream)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fclosep)(FILE* stream);
	int (*fclosep)(FILE* stream);
	DECLARE_VARIABLE_FD;

//	GET_REAL_FUNC(fclose, LIBC);
	fclosep = (void *)orig;

	PRE_PROBEBLOCK_BEGIN();
	GET_FD_FROM_FILEP(stream);
	if(_fd != -1) {
		_fstatret = fstat(_fd, &_statbuf);
	}
	PRE_PROBEBLOCK_END();

	ret = fclosep(stream);

	POST_PACK_PROBEBLOCK_BEGIN();
	PREPARE_LOCAL_BUF();
	PACK_COMMON_BEGIN(MSG_PROBE_RESOURCE,
			  API_ID_fclose,
			  "p", voidp_to_uint64(stream));
	PACK_COMMON_END('d', ret, newerrno, call_type, caller);
	POST_PACK_PROBEBLOCK_MIDDLE_FD(0, _fd, FD_API_CLOSE);
	POST_PACK_PROBEBLOCK_END();

	return ret;
}

HANDLER_WRAPPERS(file_feature, FILE *, tmpfile)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static FILE* (*tmpfilep) ( void );
	FILE* (*tmpfilep) ( void );
	FILE* fret;

	BEFORE_ORIGINAL_FILE(tmpfile, LIBC);
	tmpfilep = (void *)orig;
	_filepath = "<temp file>";
	fret = tmpfilep();
	AFTER_PACK_ORIGINAL_FILEP(API_ID_tmpfile,
				  'p', fret, 0, fret, FD_API_OPEN, "s", "");
	return fret;
}

HANDLER_WRAPPERS(file_feature, int, fgetpos, FILE*, stream, fpos_t*, position)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fgetposp)(FILE* stream, fpos_t* position);
	int (*fgetposp)(FILE* stream, fpos_t* position);

	BEFORE_ORIGINAL_FILE(fgetpos, LIBC);
	fgetposp = (void *)orig;
	ret = fgetposp(stream, position);
	AFTER_PACK_ORIGINAL_FILEP(API_ID_fgetpos,
				  'd', ret, 0, stream, FD_API_OTHER, "pp",
				  voidp_to_uint64(stream),
				  voidp_to_uint64(position));
	return ret;
}

HANDLER_WRAPPERS(file_feature, int, fseek, FILE*, stream, long int, offset, int, origin)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fseekp)(FILE* stream, long int offset, int origin);
	int (*fseekp)(FILE* stream, long int offset, int origin);

	BEFORE_ORIGINAL_FILE(fseek, LIBC);
	fseekp = (void *)orig;
	ret = fseekp(stream, offset, origin);
	AFTER_PACK_ORIGINAL_FILEP(API_ID_fseek,
				  'd', ret, (unsigned int)offset, stream, FD_API_OTHER,
				  "pxd", voidp_to_uint64(stream),
				  (uint64_t)(offset), origin);
	return ret;
}

HANDLER_WRAPPERS(file_feature, int, fsetpos, FILE*, stream, const fpos_t*, pos)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static int (*fsetposp)(FILE* stream, const fpos_t* pos);
	int (*fsetposp)(FILE* stream, const fpos_t* pos);

	BEFORE_ORIGINAL_FILE(fsetpos, LIBC);
	fsetposp = (void *)orig;
	ret = fsetposp(stream, pos);
	AFTER_PACK_ORIGINAL_FILEP(API_ID_fsetpos,
				  'd', ret, 0, stream, FD_API_OTHER, "pp",
				  voidp_to_uint64(stream), voidp_to_uint64(pos));
	return ret;
}

HANDLER_WRAPPERS(file_feature, long int, ftell, FILE*, stream)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static long int (*ftellp)(FILE* stream);
	long int (*ftellp)(FILE* stream);
	long int lret;

	BEFORE_ORIGINAL_FILE(ftell, LIBC);
	ftellp = (void *)orig;
	lret = ftellp(stream);

	AFTER_PACK_ORIGINAL_FILEP(API_ID_ftell,
				  'x', lret, 0, stream, FD_API_OTHER, "p",
				  voidp_to_uint64(stream));

	return lret;
}

HANDLER_WRAPPERS_VOID(file_feature, void, rewind, FILE*, stream)
{
	/* TODO Support old preload */
	/* TODO Support different orig funcs for different bins */
//	static void (*rewindp)(FILE* stream);
	void (*rewindp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(rewind, LIBC);
	rewindp = (void *)orig;
	rewindp(stream);

	AFTER_PACK_ORIGINAL_FILEP(API_ID_rewind,
				  'v', 0, 0, stream, FD_API_OTHER, "p",
				  voidp_to_uint64(stream));
}

HANDLER_WRAPPERS_VOID(file_feature, void, clearerr, FILE*, stream)
{
//	static void (*clearerrp)(FILE* stream);
	void (*clearerrp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(clearerr, LIBC);
	clearerrp = (void *)orig;
	clearerrp(stream);

	AFTER_PACK_ORIGINAL_FILEP(API_ID_clearerr,
				  'v', 0, 0, stream, FD_API_OTHER, "p",
				  voidp_to_uint64(stream));
}

HANDLER_WRAPPERS(file_feature, int, feof, FILE*, stream)
{
//	static int (*feofp)(FILE* stream);
	int (*feofp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(feof, LIBC);
	feofp = (void *)orig;
	ret = feofp(stream);
	AFTER_PACK_ORIGINAL_FILEP(API_ID_feof,
				  'd', ret, 0, stream, FD_API_OTHER, "p",
				  voidp_to_uint64(stream));
	return ret;
}

HANDLER_WRAPPERS(file_feature, int, ferror, FILE*, stream)
{
//	static int (*ferrorp)(FILE* stream);
	int (*ferrorp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(ferror, LIBC);
	ferrorp = (void *)orig;
	ret = ferrorp(stream);
	AFTER_PACK_ORIGINAL_FILEP(API_ID_ferror,
				  'd', ret, 0, stream, FD_API_OTHER, "p",
				  voidp_to_uint64(stream));
	return ret;
}

HANDLER_WRAPPERS(file_feature, int, fileno, FILE*, stream)
{
//	static int (*filenop)(FILE* stream);
	int (*filenop)(FILE* stream);

	BEFORE_ORIGINAL_FILE(fileno, LIBC);
	filenop = (void *)orig;
	ret = filenop(stream);
	AFTER_PACK_ORIGINAL_FILEP(API_ID_fileno,
				  'd', ret, 0, stream, FD_API_OTHER, "p",
				  voidp_to_uint64(stream));
	return ret;
}



// *******************************************************************
// File read / write APIs
// *******************************************************************

HANDLER_WRAPPERS(file_feature, int, vfprintf, FILE*, stream, const char*, format, va_list, arg)
{
//	static int (*vfprintfp)(FILE* stream, const char* format, va_list arg);
	int (*vfprintfp)(FILE* stream, const char* format, va_list arg);

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_vfprintf, 'd', vfprintf, LIBC, stream,
					FD_API_WRITE_START, "ps",
					voidp_to_uint64(stream), format);
	vfprintfp = (void *)orig;
	ret = vfprintfp(stream, format, arg);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_vfprintf, 'd', ret, ret, stream,
				       FD_API_WRITE_END, "ps",
				       voidp_to_uint64(stream), format);

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, vfscanf, FILE*, stream, const char*, format, va_list, arg)
{
//	static int (*vfscanfp)(FILE* stream, const char* format, va_list arg);
	int (*vfscanfp)(FILE* stream, const char* format, va_list arg);

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_vfscanf, 'd', vfscanf, LIBC, stream,
					FD_API_READ_START, "ps",
					voidp_to_uint64(stream), format);
	vfscanfp = (void *)orig;
	ret = vfscanfp(stream, format, arg);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_vfscanf, 'd', ret, ret, stream,
				       FD_API_READ_END, "ps",
				       voidp_to_uint64(stream), format);

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, fgetc, FILE*, stream)
{
//	static int (*fgetcp)(FILE* stream);
	int (*fgetcp)(FILE* stream);

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_fgetc, 'd', fgetc, LIBC, stream,
					FD_API_READ_START, "p",
					voidp_to_uint64(stream));
	fgetcp = (void *)orig;
	ret = fgetcp(stream);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_fgetc, 'd', ret, (ret != EOF), stream,
				       FD_API_READ_END, "p",
				       voidp_to_uint64(stream));

	return ret;
}

#if 0	// why is this commented?
HANDLER_WRAPPERS(file_feature, char*, fgets, char*, str, int, size, FILE*, stream)
{
	static char* (*fgetsp)(char* str, int num, FILE* stream);
	char* cret;

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_fgets, 'p', FD_API_READ_START,
				fgets, LIBC, stream, "sdp", str, size, stream);
	cret = fgetsp(str, size, stream);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_fgets, 'p', cret, (ret == NULL ? 0 : strlen(cret)),
			stream, FD_API_READ_END, "sdp", str, size, stream);

	return cret;
}
#endif

HANDLER_WRAPPERS(file_feature, int, fputc, int, character, FILE*, stream)
{
//	static int (*fputcp)(int character, FILE* stream);
	int (*fputcp)(int character, FILE* stream);

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_fputc, 'd', fputc, LIBC, stream,
					FD_API_WRITE_START, "dp", character,
					voidp_to_uint64(stream));
	fputcp = (void *)orig;
	ret = fputcp(character, stream);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_fputc, 'd', ret, (ret == EOF ? 0 : 1),
				       stream, FD_API_WRITE_END, "dp",
				       character, voidp_to_uint64(stream));

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, fputs, const char*, str, FILE*, stream)
{
//	static int (*fputcp)(int character, FILE* stream);
	int (*fputsp)(const char *, FILE* stream);

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_fputs, 'd', fputs, LIBC, stream,
					FD_API_WRITE_START, "sp", str,
					voidp_to_uint64(stream));
	fputsp = (void *)orig;
	ret = fputsp(str, stream);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_fputs, 'd', ret, ret, stream,
				       FD_API_WRITE_END, "sp", str,
				       voidp_to_uint64(stream));

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, getc, FILE*, stream)
{
//	static int (*getcp)(FILE* stream);
	int (*getcp)(FILE* stream);

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_getc, 'd', getc, LIBC, stream,
					FD_API_READ_START, "p",
					voidp_to_uint64(stream));
	getcp = (void *)orig;
	ret = getcp(stream);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_getc, 'd', ret, (ret == EOF ? 0 : 1), stream,
				       FD_API_READ_END, "p",
				       voidp_to_uint64(stream));

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, putc, int, character, FILE*, stream)
{
//	static int (*putcp)(int character, FILE* stream);
	int (*putcp)(int character, FILE* stream);

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_putc, 'd', putc, LIBC, stream,
					FD_API_WRITE_START, "dp", character,
					voidp_to_uint64(stream));
	putcp = (void *)orig;
	ret = putcp(character, stream);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_putc, 'd', ret, (ret == EOF ? 0 : 1),
				       stream, FD_API_WRITE_END, "dp",
				       character, voidp_to_uint64(stream));

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, ungetc, int, character, FILE*, stream)
{
//	static int (*ungetcp)(int character, FILE* stream);
	int (*ungetcp)(int character, FILE* stream);

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_putc, 'd', ungetc, LIBC, stream,
					FD_API_WRITE_START, "dp", character,
					voidp_to_uint64(stream));
	ungetcp = (void *)orig;
	ret = ungetcp(character, stream);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_ungetc, 'd', ret, 0, stream,
				       FD_API_OTHER, "dp", character,
				       voidp_to_uint64(stream));

	return ret;
}

HANDLER_WRAPPERS(file_feature, size_t, fread, void*, ptr, size_t, size, size_t, count, FILE*, stream)
{
//	static size_t (*freadp)(void* ptr, size_t size, size_t count, FILE* stream);
	size_t (*freadp)(void* ptr, size_t size, size_t count, FILE* stream);
	size_t tret;

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_fread, 'x', fread, LIBC, stream,
					FD_API_READ_START, "pxxp",
					voidp_to_uint64(ptr),
					(uint64_t)(size),
					(uint64_t)(count),
					voidp_to_uint64(stream));

	freadp = (void *)orig;
	tret = freadp(ptr, size, count, stream);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_fread, 'x', tret, tret*size, stream,
				       FD_API_READ_END, "pxxp",
				       voidp_to_uint64(ptr),
				       (uint64_t)(size),
				       (uint64_t)(count),
				       voidp_to_uint64(stream));

	return tret;
}

HANDLER_WRAPPERS(file_feature, size_t, fwrite, const void*, ptr, size_t, size, size_t, count, FILE*, stream)
{
//	static size_t (*fwritep)(const void* ptr, size_t size, size_t count, FILE* stream);
	size_t (*fwritep)(const void* ptr, size_t size, size_t count, FILE* stream);
	size_t tret;

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_fwrite, 'x', fwrite, LIBC, stream,
					FD_API_WRITE_START, "pxxp",
					voidp_to_uint64(ptr),
					(uint64_t)(size),
					(uint64_t)(count),
					voidp_to_uint64(stream));

	fwritep = (void *)orig;
	tret = fwritep(ptr, size, count, stream);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_fwrite, 'x', tret, tret*size, stream,
				       FD_API_WRITE_END, "pxxp",
				       voidp_to_uint64(ptr),
				       (uint64_t)(size),
				       (uint64_t)(count),
				       voidp_to_uint64(stream));

	return tret;
}

// *********************************************************
// variable parameter function
// *********************************************************
DEF_H(int, fprintf, FILE*, stream, const char*, format, va_list, args);

DEF_WV(int, fprintf, FILE*, stream, const char*, format, ...)
{ 
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, fprintf);

	va_start(args, format);
	ret = fprintf_handler(orig, INTERNAL_CALL, caller, stream,
			      format, args);
	va_end(args);

	return ret;
}

DEF_WAV(int, fprintf, FILE*, stream, const char*, format, ...)
{ 
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, fprintf);

	va_start(args, format);
	ret = fprintf_handler(orig, EXTERNAL_CALL, caller,
			      stream, format, args);
	va_end(args);

	return ret;
}

DEF_H(int, fprintf, FILE*, stream, const char*, format, va_list, args)
{
//	static int (*vfprintfp)(FILE* stream, const char* format, ...);
	int (*vfprintfp)(FILE* stream, const char* format, ...);

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_fprintf, 'd', vfprintf, LIBC, stream,
					FD_API_WRITE_START, "ps",
					voidp_to_uint64(stream), format);

	vfprintfp = (void *)orig;
	ret = vfprintfp(stream, format, args);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_fprintf, 'd', ret, ret, stream,
				       FD_API_WRITE_END, "ps",
				       voidp_to_uint64(stream), format);

	return ret;
}

DEF_H(int, fscanf, FILE*, stream, const char*, format, va_list, args);

DEF_WV(int, fscanf, FILE*, stream, const char*, format, ...)
{ 
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, fscanf);

	va_start(args, format);
	ret = fscanf_handler(orig, INTERNAL_CALL, caller, stream,
			     format, args);
	va_end(args);

	return ret;
}

DEF_WAV(int, fscanf, FILE*, stream, const char*, format, ...)
{ 
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, fscanf);

	va_start(args, format);
	ret = fscanf_handler(orig, EXTERNAL_CALL, caller,
			     stream, format, args);
	va_end(args);

	return ret;
}

DEF_H(int, fscanf, FILE*, stream, const char*, format, va_list, args)
{
//	static int (*vfscanfp)(FILE* stream, const char* format, ...);
	int (*vfscanfp)(FILE* stream, const char* format, ...);

	BEFORE_ORIGINAL_START_END_FILEP(API_ID_fscanf, 'd', vfscanf, LIBC, stream,
					FD_API_READ_START, "ps",
					voidp_to_uint64(stream), format);

	vfscanfp = (void *)orig;
	ret = vfscanfp(stream, format, args);

	AFTER_ORIGINAL_START_END_FILEP(API_ID_fscanf, 'd', ret, ret, stream,
				       FD_API_READ_END, "ps",
				       voidp_to_uint64(stream), format);

	return ret;
}

#if !defined(DA_DEBUG_LOG) && !defined(PRINT_STDOUT)
DEF_H(int, printf, const char*, format, va_list, args);

DEF_WV(int, printf, const char*, format, ...)
{ 
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, printf);

	va_start(args, format);
	ret = printf_handler(orig, INTERNAL_CALL, caller, format, args);
	va_end(args);

	return ret;
}

DEF_WAV(int, printf, const char*, format, ...)
{ 
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, printf);

	va_start(args, format);
	ret = printf_handler(orig, EXTERNAL_CALL, caller,
			     format, args);
	va_end(args);

	return ret;
}

DEF_H(int, printf, const char*, format, va_list, args)
{
//	static int (*vprintfp)(const char* format, ...);
	int (*vprintfp)(const char* format, ...);

	BEFORE_ORIGINAL_START_END_NOFD(API_ID_printf, 'd', vprintf, LIBC,
				FD_API_WRITE_START, "s", format);

	vprintfp = (void *)orig;
	ret = vprintfp(format, args);

	AFTER_ORIGINAL_START_END_NOFD(API_ID_printf, 'd', ret, ret,
				FD_API_WRITE_END, "s", format);

	return ret;
}
#endif

DEF_H(int, scanf, const char*, format, va_list, args);

DEF_WV(int, scanf, const char*, format, ...)
{ 
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, scanf);

	va_start(args, format);
	ret = scanf_handler(orig, INTERNAL_CALL, caller, format, args);
	va_end(args);

	return ret;
}

DEF_WAV(int, scanf, const char*, format, ...)
{ 
	uint32_t caller;
	ElfW(Addr) orig;
	va_list args;
	int ret;

	caller = (uint32_t)
	    (__builtin_extract_return_addr(__builtin_return_address(0)));
	orig = GET_ORIG_FUNC(file_feature, scanf);

	va_start(args, format);
	ret = scanf_handler(orig, EXTERNAL_CALL, caller, format,
			    args);
	va_end(args);

	return ret;
}

DEF_H(int, scanf, const char*, format, va_list, args)
{
//	static int (*vscanfp)(const char* format, ...);
	int (*vscanfp)(const char* format, ...);

	BEFORE_ORIGINAL_START_END_NOFD(API_ID_scanf, 'd', vscanf, LIBC,
				FD_API_READ_START, "s", format);

	vscanfp = (void *)orig;
	ret = vscanfp(format, args);

	AFTER_ORIGINAL_START_END_NOFD(API_ID_scanf, 'd', ret, ret,
				 FD_API_READ_END, "s", format);

	return ret;
}


HANDLER_WRAPPERS(file_feature, int, getchar)
{
//	static int (*getcharp)();
	int (*getcharp)();

	BEFORE_ORIGINAL_START_END_NOFD(API_ID_getchar, 'd', getchar, LIBC,
				FD_API_READ_START, "", 0);

	getcharp = (void *)orig;
	ret = getcharp();

	AFTER_ORIGINAL_START_END_NOFD(API_ID_getchar, 'd', ret, (ret == EOF ? 0 : 1),
				FD_API_READ_END, "", 0);

	return ret;
}

HANDLER_WRAPPERS(file_feature, int, putchar, int, c)
{
//	static int (*putcharp)(int c);
	int (*putcharp)(int c);

	BEFORE_ORIGINAL_START_END_NOFD(API_ID_putchar, 'd', putchar, LIBC,
				FD_API_WRITE_START, "d", c);

	putcharp = (void *)orig;
	ret = putcharp(c);

	AFTER_ORIGINAL_START_END_NOFD(API_ID_putchar, 'd', ret, (ret == EOF ? 0 : 1),
				FD_API_WRITE_END, "d", c);

	return ret;
}

HANDLER_WRAPPERS(file_feature, char*, gets, char*, str)
{
//	static char* (*getsp)(char* str);
	char* (*getsp)(char* str);
	char* cret;

	BEFORE_ORIGINAL_START_END_NOFD(API_ID_gets, 'p', gets, LIBC,
				FD_API_READ_START, "s", str);

	getsp = (void *)orig;
	cret = getsp(str);

	AFTER_ORIGINAL_START_END_NOFD(API_ID_gets, 'p', cret, strlen(cret),
				FD_API_READ_END, "s", str);

	return cret;
}

#if !defined(DA_DEBUG_LOG) && !defined(PRINT_STDOUT)
HANDLER_WRAPPERS(file_feature, int, puts, const char*, str)
{
//	static int (*putsp)(const char* str);
	int (*putsp)(const char* str);

	BEFORE_ORIGINAL_START_END_NOFD(API_ID_puts, 'd', puts, LIBC,
				FD_API_WRITE_START, "s", str);

	putsp = (void *)orig;
	ret = putsp(str);

	AFTER_ORIGINAL_START_END_NOFD(API_ID_puts, 'd', ret, ret,
				FD_API_WRITE_END, "s", str);

	return ret;
}
#endif



HANDLER_WRAPPERS_VOID(file_feature, void, setbuf, FILE*, stream, char*, buf)
{
//	static void (*setbufp)(FILE* stream, char* buf);
	void (*setbufp)(FILE* stream, char* buf);

	BEFORE_ORIGINAL_FILE(setbuf, LIBC);

	setbufp = (void *)orig;
	setbufp(stream, buf);

	AFTER_PACK_ORIGINAL_FILEP(API_ID_setbuf,
				  'v', 0, 0, stream, FD_API_OTHER, "pp",
				  voidp_to_uint64(stream),
				  voidp_to_uint64(buf));
}

HANDLER_WRAPPERS_VOID(file_feature, void, setbuffer, FILE*, stream, char*, buf, size_t, size)
{
//	static void (*setbufferp)(FILE* stream, char* buf, size_t size);
	void (*setbufferp)(FILE* stream, char* buf, size_t size);

	BEFORE_ORIGINAL_FILE(setbuffer, LIBC);

	setbufferp = (void *)orig;
	setbufferp(stream, buf, size);

	AFTER_PACK_ORIGINAL_FILEP(API_ID_setbuffer,
				  'v', 0, size, stream, FD_API_OTHER,
				  "ppx", voidp_to_uint64(stream),
				  voidp_to_uint64(buf), (uint64_t)(size));
}

HANDLER_WRAPPERS_VOID(file_feature, void, setlinebuf, FILE*, stream)
{
//	static int (*setlinebufp)(FILE* stream);
	int (*setlinebufp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(setlinebuf, LIBC);

	setlinebufp = (void *)orig;
	setlinebufp(stream);

	AFTER_PACK_ORIGINAL_FILEP(API_ID_setlinebuf,
				  'v', 0, 0, stream, FD_API_OTHER, "p",
				  voidp_to_uint64(stream));
}

HANDLER_WRAPPERS(file_feature, int, setvbuf, FILE*, stream, char*, buf, int, mode, size_t, size)
{
//	static int (*setvbufp)(FILE* stream, char* buf, int mode, size_t size);
	int (*setvbufp)(FILE* stream, char* buf, int mode, size_t size);

	BEFORE_ORIGINAL_FILE(setvbuf, LIBC);

	setvbufp = (void *)orig;
	ret = setvbufp(stream,buf,mode,size);
	AFTER_PACK_ORIGINAL_FILEP(API_ID_setvbuf,
				  'd', ret, size, stream, FD_API_OTHER,
				  "ppdx",
				  voidp_to_uint64(stream),
				  voidp_to_uint64(buf), mode,
				  (uint64_t)(size));
	return ret;
}

/**************************** ALIASES *********************************/
weak_alias_pref(__isoc99_scanf, scanf);
