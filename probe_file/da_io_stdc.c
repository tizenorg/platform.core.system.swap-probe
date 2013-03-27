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
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dautil.h"
#include "da_io.h"

FILE* fopen(const char* filename, const char* mode)
{
	static FILE* (*fopenp)(const char* filename, const char* mode);
	FILE* fret;
	
	BEFORE_ORIGINAL_FILE_NOFILTER(fopen, libc.so.6);
	_filepath = (char*)filename;
	
	fret = fopenp(filename, mode);
	
	AFTER_ORIGINAL_FILEP_RET(VT_PTR, fret, 0, fret, FD_API_OPEN, "%s, %s", filename, mode);

	return fret;
}

FILE* freopen(const char * filename, const char * mode, FILE * stream)
{
	static FILE* (*freopenp)(const char * filename, const char * mode, FILE * stream);
 	FILE* fret;

	BEFORE_ORIGINAL_FILE_NOFILTER(freopen, libc.so.6);
	_filepath = (char*)filename;

	fret = freopenp(filename, mode, stream);

	AFTER_ORIGINAL_FILEP_RET(VT_PTR, fret, 0, fret, FD_API_OPEN,
			"%s, %s, %p", filename, mode, stream);

	return fret;
}

FILE* fdopen(int fildes, const char *mode)
{
	static FILE* (*fdopenp)(int fildes, const char *mode);
 	FILE* fret;

	BEFORE_ORIGINAL_FILE_NOFILTER(fdopen, libc.so.6);

	fret = fdopenp(fildes, mode);

	AFTER_ORIGINAL_FILEP_RET(VT_PTR, fret, 0, fret, FD_API_OPEN, "%d, %s", fildes, mode);

	return fret;
}

int fflush(FILE* stream)
{
	static int (*fflushp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(fflush, libc.so.6);
	ret = fflushp(stream);
	AFTER_ORIGINAL_FILEP(0, stream, FD_API_OTHER, "%p", stream);
	return ret;
}

int fclose(FILE* stream)
{
	static int (*fclosep)(FILE* stream);
	DECLARE_VARIABLE_FD;

	GET_REAL_FUNC(fclose, libc.so.6);

	bfiltering = false;
	PRE_PROBEBLOCK_BEGIN();
	GET_FD_FROM_FILEP(stream);
	_fstatret = fstat(_fd, &_statbuf);
	PRE_PROBEBLOCK_END();

	ret = fclosep(stream);

	POST_PROBEBLOCK_BEGIN(LC_RESOURCE, VT_INT, ret,	"%p", stream);
	POST_PROBEBLOCK_MIDDLE_FD(0, _fd, FD_API_CLOSE);
	POST_PROBEBLOCK_CALLSTACK_RESOURCE(FD_API_CLOSE);
	POST_PROBEBLOCK_END();

	return ret;
}

int remove(const char* filename)
{
	static int (*removep)(const char* filename);

	BEFORE_ORIGINAL_FILE(remove, libc.so.6);
	_filepath = (char*)filename;
	ret = removep(filename);
	AFTER_ORIGINAL_NOFD(0, FD_API_DIRECTORY, "%s", filename);
	return ret;
}

int rename(const char* oldname, const char* newname)
{
	static int (*renamep)(const char* oldname, const char* newname);

	BEFORE_ORIGINAL_FILE(rename, libc.so.6);
	_filepath = (char*)newname;
	ret = renamep(oldname, newname);
	AFTER_ORIGINAL_NOFD(0, FD_API_DIRECTORY, "%s, %s", oldname, newname);
	return ret;
}

FILE * tmpfile ( void )
{
	static FILE* (*tmpfilep) ( void );
	FILE* fret;

	BEFORE_ORIGINAL_FILE_NOFILTER(tmpfile, libc.so.6);
	_filepath = "<temp file>";
	fret = tmpfilep();

	AFTER_ORIGINAL_FILEP_RET(VT_PTR, fret, 0, fret, FD_API_OPEN, "%s", "");

	return fret;
}

int fgetpos(FILE* stream, fpos_t* position)
{
	static int (*fgetposp)(FILE* stream, fpos_t* position);

	BEFORE_ORIGINAL_FILE(fgetpos, libc.so.6);
	ret = fgetposp(stream, position);
	AFTER_ORIGINAL_FILEP(0, stream, FD_API_OTHER, "%p, %p", stream, position);
	return ret;
}

int fseek(FILE* stream, long int offset, int origin)
{
	static int (*fseekp)(FILE* stream, long int offset, int origin);

	BEFORE_ORIGINAL_FILE(fseek, libc.so.6);
	ret = fseekp(stream, offset, origin);
	AFTER_ORIGINAL_FILEP((unsigned int)offset, stream, FD_API_OTHER,
			"%p, %ld, %d", stream, offset, origin);
	return ret;
}

int fsetpos(FILE* stream, const fpos_t* pos)
{
	static int (*fsetposp)(FILE* stream, const fpos_t* pos);

	BEFORE_ORIGINAL_FILE(fsetpos, libc.so.6);
	ret = fsetposp(stream, pos);
	AFTER_ORIGINAL_FILEP(0, stream, FD_API_OTHER, "%p, %p", stream, pos);
	return ret;
}

long int ftell(FILE* stream)
{
	static long int (*ftellp)(FILE* stream);
	long int lret;

	BEFORE_ORIGINAL_FILE(ftell, libc.so.6);
	
	lret = ftellp(stream);

	AFTER_ORIGINAL_FILEP_RET(VT_LONG, lret, 0, stream, FD_API_OTHER, "%p", stream);

	return lret;
}

void rewind(FILE* stream)
{
	static void (*rewindp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(rewind, libc.so.6);

	rewindp(stream);
	
	AFTER_ORIGINAL_FILEP_RET(VT_NULL, NULL, 0, stream, FD_API_OTHER, "%p", stream);
}

void clearerr(FILE* stream)
{
	static void (*clearerrp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(clearerr, libc.so.6);

	clearerrp(stream);
	
	AFTER_ORIGINAL_FILEP_RET(VT_NULL, NULL, 0, stream, FD_API_OTHER, "%p", stream);
}

int feof(FILE* stream)
{
	static int (*feofp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(feof, libc.so.6);
	ret = feofp(stream);
	AFTER_ORIGINAL_FILEP(0, stream, FD_API_OTHER, "%p", stream);
	return ret;
}

int ferror(FILE* stream)
{
	static int (*ferrorp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(ferror, libc.so.6);
	ret = ferrorp(stream);
	AFTER_ORIGINAL_FILEP(0, stream, FD_API_OTHER, "%p", stream);
	return ret;
}

int fileno(FILE* stream)
{
	static int (*filenop)(FILE* stream);

	BEFORE_ORIGINAL_FILE(fileno, libc.so.6);
	ret = filenop(stream);
	AFTER_ORIGINAL_FILEP(0, stream, FD_API_OTHER, "%p", stream);
	return ret;
}

void perror(const char* string)
{
	static void (*perrorp)(const char* string);

	BEFORE_ORIGINAL_FILE(perror, libc.so.6);

	perrorp(string);

	AFTER_ORIGINAL_NOFD_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%s", string);
}

// *******************************************************************
// File read / write APIs
// *******************************************************************

int vfprintf(FILE* stream, const char* format, va_list arg)
{
	static int (*vfprintfp)(FILE* stream, const char* format, va_list arg);

	BEFORE_ORIGINAL_FILE(vfprintf, libc.so.6);
	ret = vfprintfp(stream, format, arg);
	AFTER_ORIGINAL_FILEP(ret, stream, FD_API_WRITE, "%p, %s", stream, format);
	return ret;
}

int vfscanf(FILE* stream, const char* format, va_list arg)
{
	static int (*vfscanfp)(FILE* stream, const char* format, va_list arg);

	BEFORE_ORIGINAL_FILE(vfscanf, libc.so.6);
	ret = vfscanfp(stream, format, arg);
	AFTER_ORIGINAL_FILEP(ret, stream, FD_API_READ, "%p, %s", stream, format);
	return ret;
}

int fgetc(FILE* stream)
{
	static int (*fgetcp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(fgetc, libc.so.6);
	ret = fgetcp(stream);
	AFTER_ORIGINAL_FILEP((ret == EOF ? 0 : 1), stream, FD_API_READ, "%p", stream);
	return ret;
}

#if 0	// why is this commented?
char* fgets(char* str, int size, FILE* stream)
{
	static char* (*fgetsp)(char* str, int num, FILE* stream);
	char* cret;

	BEFORE_ORIGINAL_FILE(fgets, libc.so.6);
	
	cret = fgetsp(str, size, stream);

	AFTER_ORIGINAL_FILEP_RET(VT_STR, cret, (ret == NULL ? 0 : strlen(cret)),
			stream, FD_API_READ, "%s, %d, %p", str, size, stream);

	return cret;
}
#endif

int fputc(int character, FILE* stream)
{
	static int (*fputcp)(int character, FILE* stream);

	BEFORE_ORIGINAL_FILE(fputc, libc.so.6);
	ret = fputcp(character, stream);
	AFTER_ORIGINAL_FILEP((ret == EOF ? 0 : 1), stream, FD_API_WRITE,
			"%d, %p", character, stream);
	return ret;
}

int fputs(const char* str, FILE* stream)
{
	static int (*fputsp)(const char* str, FILE* stream);

	BEFORE_ORIGINAL_FILE(fputs, libc.so.6);
	ret = fputsp(str, stream);
	AFTER_ORIGINAL_FILEP(ret, stream, FD_API_WRITE, "%s, %p", str, stream);
	return ret;
}

int getc(FILE* stream)
{
	static int (*getcp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(getc, libc.so.6);
	ret = getcp(stream);
	AFTER_ORIGINAL_FILEP((ret == EOF ? 0 : 1), stream, FD_API_READ, "%p", stream);
	return ret;
}

int putc(int character, FILE* stream)
{
	static int (*putcp)(int character, FILE* stream);

	BEFORE_ORIGINAL_FILE(putc, libc.so.6);
	ret = putcp(character, stream);
	AFTER_ORIGINAL_FILEP((ret == EOF ? 0 : 1), stream, FD_API_WRITE,
			"%d, %p", character, stream);
	return ret;
}

int ungetc(int character, FILE* stream)
{
	static int (*ungetcp)(int character, FILE* stream);

	BEFORE_ORIGINAL_FILE(ungetc, libc.so.6);
	ret = ungetcp(character, stream);
	AFTER_ORIGINAL_FILEP(0, stream, FD_API_OTHER, "%d, %p", character, stream);
	return ret;
}

size_t fread(void* ptr, size_t size, size_t count, FILE* stream)
{
	static size_t (*freadp)(void* ptr, size_t size, size_t count, FILE* stream);
	size_t tret;

	BEFORE_ORIGINAL_FILE(fread, libc.so.6);

	tret = freadp(ptr, size, count, stream);
	
	AFTER_ORIGINAL_FILEP_RET(VT_SIZE_T, tret, 0, stream, FD_API_READ,
			"%p, %u, %u, %p", ptr, size, count, stream);
	
	return tret;
}

size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream)
{
	static size_t (*fwritep)(const void* ptr, size_t size, size_t count, FILE* stream);
	size_t tret;

	BEFORE_ORIGINAL_FILE(fwrite, libc.so.6);

	tret = fwritep(ptr, size, count, stream);

	AFTER_ORIGINAL_FILEP_RET(VT_SIZE_T, tret, 0, stream, FD_API_WRITE,
			"%p, %u, %u, %p", ptr, size, count, stream);

	return tret;
}

// *********************************************************
// variable parameter function
// *********************************************************
int fprintf(FILE* stream, const char* format, ...)
{
	static int (*vfprintfp)(FILE* stream, const char* format, ...);

	BEFORE_ORIGINAL_FILE(vfprintf, libc.so.6);

	va_list arg;
	va_start(arg, format);
	ret = vfprintfp(stream, format, arg);

	AFTER_ORIGINAL_FILEP(ret, stream, FD_API_WRITE, "%p, %s, ...", stream, format);
	va_end(arg);

	return ret;
}

int fscanf(FILE* stream, const char* format, ...)
{
	static int (*vfscanfp)(FILE* stream, const char* format, ...);
	
	BEFORE_ORIGINAL_FILE(vfscanf, libc.so.6);

	va_list arg;
	va_start(arg, format);
	ret = vfscanfp(stream, format, arg);

	AFTER_ORIGINAL_FILEP(ret, stream, FD_API_READ, "%p, %s, ...", stream, format);
	va_end(arg);

	return ret;
}

#if !defined(DA_DEBUG_LOG) && !defined(PRINT_STDOUT)
int printf(const char* format, ...)
{
	static int (*vprintfp)(const char* format, ...);
	
	BEFORE_ORIGINAL_FILE(vprintf, libc.so.6);

	va_list arg;
	va_start(arg, format);
	ret = vprintfp(format, arg);

	AFTER_ORIGINAL_NOFD(ret, FD_API_WRITE, "%s,...", format);
	va_end(arg);

	return ret;
}
#endif

int scanf(const char* format, ...)
{
	static int (*vscanfp)(const char* format, ...);

	BEFORE_ORIGINAL_FILE(vscanf, libc.so.6);

	va_list arg;
	va_start(arg, format);
	ret = vscanfp(format, arg);

	AFTER_ORIGINAL_NOFD(ret, FD_API_READ, "%s,...", format);
	va_end(arg);

	return ret;
}

int getchar()
{
	static int (*getcharp)();

	BEFORE_ORIGINAL_FILE(getchar, libc.so.6);
	ret = getcharp();
	AFTER_ORIGINAL_NOFD((ret == EOF ? 0 : 1), FD_API_READ, "%s", "");
	return ret;
}

int putchar(int c)
{
	static int (*putcharp)(int c);

	BEFORE_ORIGINAL_FILE(putchar, libc.so.6);
	ret = putcharp(c);
	AFTER_ORIGINAL_NOFD((ret == EOF ? 0 : 1), FD_API_WRITE, "%d", c);
	return ret;
}

char* gets(char* str)
{
	static char* (*getsp)(char* str);
	char* cret;

	BEFORE_ORIGINAL_FILE(gets, libc.so.6);

	cret = getsp(str);
	
	AFTER_ORIGINAL_NOFD_RET(VT_STR, cret, strlen(cret), FD_API_READ, "%s", str);

	return cret;
}

#if !defined(DA_DEBUG_LOG) && !defined(PRINT_STDOUT)
int puts(const char* str)
{
	static int (*putsp)(const char* str);

	BEFORE_ORIGINAL_FILE(puts, libc.so.6);
	ret = putsp(str);
	AFTER_ORIGINAL_NOFD(ret, FD_API_WRITE, "%s", str);
	return ret;
}
#endif

char* tmpnam(char* str)
{
	static char* (*tmpnamp)(char* str);
	char* cret;

	BEFORE_ORIGINAL_FILE(tmpnam, libc.so.6);

	cret = tmpnamp(str);

	AFTER_ORIGINAL_NOFD_RET(VT_STR, cret, 0, FD_API_OTHER, "%s", str);

	return cret;
}

void setbuf(FILE* stream, char* buf)
{
	static void (*setbufp)(FILE* stream, char* buf);

	BEFORE_ORIGINAL_FILE(setbuf, libc.so.6);

	setbufp(stream, buf);

	AFTER_ORIGINAL_FILEP_RET(VT_NULL, NULL, 0, stream, FD_API_OTHER,
			"%p, %p", stream, buf);
}

void setbuffer(FILE* stream, char* buf, size_t size)
{
	static void (*setbufferp)(FILE* stream, char* buf, size_t size);

	BEFORE_ORIGINAL_FILE(setbuffer, libc.so.6);

	setbufferp(stream, buf, size);

	AFTER_ORIGINAL_FILEP_RET(VT_NULL, NULL, size, stream, FD_API_OTHER,
			"%p, %p, %u", stream, buf, size);
}

void setlinebuf(FILE* stream)
{
	static int (*setlinebufp)(FILE* stream);

	BEFORE_ORIGINAL_FILE(setlinebuf, libc.so.6);

	setlinebufp(stream);

	AFTER_ORIGINAL_FILEP_RET(VT_NULL, NULL, 0, stream, FD_API_OTHER, "%p", stream);
}

int setvbuf(FILE* stream, char* buf, int mode, size_t size)
{
	static int (*setvbufp)(FILE* stream, char* buf, int mode, size_t size);

	BEFORE_ORIGINAL_FILE(setvbuf, libc.so.6);
	ret = setvbufp(stream,buf,mode,size);
	AFTER_ORIGINAL_FILEP(size, stream, FD_API_OTHER,
			"%p, %p, %d, %u", stream, buf, mode, size);
	return ret;
}

