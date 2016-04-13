#ifndef __DA_IO_STDC_H__
#define __DA_IO_STDC_H__

FILE* PROBE_NAME(fopen)(const char* filename, const char* mode);
FILE* PROBE_NAME(freopen)(const char * filename, const char * mode, FILE * stream);
FILE* PROBE_NAME(fdopen)(int fildes, const char *mode);
int PROBE_NAME(fflush)(FILE* stream);
int PROBE_NAME(fclose)(FILE* stream);
FILE * PROBE_NAME(tmpfile )( void );
int PROBE_NAME(fgetpos)(FILE* stream, fpos_t* position);
int PROBE_NAME(fseek)(FILE* stream, long int offset, int origin);
int PROBE_NAME(fsetpos)(FILE* stream, const fpos_t* pos);
long int PROBE_NAME(ftell)(FILE* stream);
void PROBE_NAME(rewind)(FILE* stream);
void PROBE_NAME(clearerr)(FILE* stream);
int PROBE_NAME(feof)(FILE* stream);
int PROBE_NAME(ferror)(FILE* stream);
int PROBE_NAME(fileno)(FILE* stream);
int PROBE_NAME(vfprintf)(FILE* stream, const char* format, va_list arg);
int PROBE_NAME(vfscanf)(FILE* stream, const char* format, va_list arg);
int PROBE_NAME(fgetc)(FILE* stream);
//char* PROBE_NAME(fgets)(char* str, int size, FILE* stream);
int PROBE_NAME(fputc)(int character, FILE* stream);
int PROBE_NAME(fputs)(const char* str, FILE* stream);
int PROBE_NAME(getc)(FILE* stream);
int PROBE_NAME(putc)(int character, FILE* stream);
int PROBE_NAME(ungetc)(int character, FILE* stream);
size_t PROBE_NAME(fread)(void* ptr, size_t size, size_t count, FILE* stream);
size_t PROBE_NAME(fwrite)(const void* ptr, size_t size, size_t count, FILE* stream);
int PROBE_NAME(fprintf)(FILE* stream, const char* format, ...);
int PROBE_NAME(fscanf)(FILE* stream, const char* format, ...);
int PROBE_NAME(printf)(const char* format, ...);
int PROBE_NAME(scanf)(const char* format, ...);
int PROBE_NAME(getchar)();
int PROBE_NAME(putchar)(int c);
char* PROBE_NAME(gets)(char* str);
int PROBE_NAME(puts)(const char* str);
void PROBE_NAME(setbuf)(FILE* stream, char* buf);
void PROBE_NAME(setbuffer)(FILE* stream, char* buf, size_t size);
void PROBE_NAME(setlinebuf)(FILE* stream);
int PROBE_NAME(setvbuf)(FILE* stream, char* buf, int mode, size_t size);


#endif /* __DA_IO_STDC_H__ */
