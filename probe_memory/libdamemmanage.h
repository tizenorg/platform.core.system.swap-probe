#ifndef __LIBDAMEMMANAGE_H__
#define __LIBDAMEMMANAGE_H__

void *PROBE_NAME(memset)(void *memblock, int c, size_t n);
int PROBE_NAME(memcmp)(const void * ptr1, const void * ptr2, size_t num);
void *PROBE_NAME(memcpy)(void * destination, const void * source, size_t num );


#endif /* __LIBDAMEMMANAGE_H__ */
