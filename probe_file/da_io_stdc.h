#ifndef __DA_IO_STDC_H__
#define __DA_IO_STDC_H__

HANDLER_WRAPPERS_DEF(FILE*, fopen, const char*, filename, const char*, mode);
HANDLER_WRAPPERS_DEF(FILE*, freopen, const char *, filename, const char *, mode, FILE *, stream);
HANDLER_WRAPPERS_DEF(FILE*, fdopen, int, fildes, const char *, mode);
HANDLER_WRAPPERS_DEF(int, fflush, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, fclose, FILE*, stream);
HANDLER_WRAPPERS_DEF(FILE *, tmpfile, void);
HANDLER_WRAPPERS_DEF(int, fgetpos, FILE*, stream, fpos_t*, position);
HANDLER_WRAPPERS_DEF(int, fseek, FILE*, stream, long int, offset, int, origin);
HANDLER_WRAPPERS_DEF(int, fsetpos, FILE*, stream, const fpos_t*, pos);
HANDLER_WRAPPERS_DEF(long int, ftell, FILE*, stream);
HANDLER_WRAPPERS_DEF(void, rewind, FILE*, stream);
HANDLER_WRAPPERS_DEF(void, clearerr, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, feof, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, ferror, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, fileno, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, vfprintf, FILE*, stream, const char*, format, va_list, arg);
HANDLER_WRAPPERS_DEF(int, vfscanf, FILE*, stream, const char*, format, va_list, arg);
HANDLER_WRAPPERS_DEF(int, fgetc, FILE*, stream);
//HANDLER_WRAPPERS_DEF(char*, fgets, char*, str, int, size, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, fputc, int, character, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, fputs, const char*, str, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, getc, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, putc, int, character, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, ungetc, int, character, FILE*, stream);
HANDLER_WRAPPERS_DEF(size_t, fread, void*, ptr, size_t, size, size_t, count, FILE*, stream);
HANDLER_WRAPPERS_DEF(size_t, fwrite, const void*, ptr, size_t, size, size_t, count, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, fprintf, FILE*, stream, const char*, format, ...);
HANDLER_WRAPPERS_DEF(int, fscanf, FILE*, stream, const char*, format, ...);
HANDLER_WRAPPERS_DEF(int, printf, const char*, format, ...);
HANDLER_WRAPPERS_DEF(int, scanf, const char*, format, ...);
HANDLER_WRAPPERS_DEF(int, getchar, void);
HANDLER_WRAPPERS_DEF(int, putchar, int, c);
HANDLER_WRAPPERS_DEF(char*, gets, char*, str);
HANDLER_WRAPPERS_DEF(int, puts, const char*, str);
HANDLER_WRAPPERS_DEF(void, setbuf, FILE*, stream, char*, buf);
HANDLER_WRAPPERS_DEF(void, setbuffer, FILE*, stream, char*, buf, size_t, size);
HANDLER_WRAPPERS_DEF(void, setlinebuf, FILE*, stream);
HANDLER_WRAPPERS_DEF(int, setvbuf, FILE*, stream, char*, buf, int, mode, size_t, size);


#endif /* __DA_IO_STDC_H__ */
