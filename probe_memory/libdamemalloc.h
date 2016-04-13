#ifndef __LIBDAMEMALLOC_H__
#define __LIBDAMEMALLOC_H__

void *PROBE_NAME(malloc)(size_t size);
void PROBE_NAME(free)(void *ptr);
void *PROBE_NAME(realloc)(void *memblock, size_t size);
void *PROBE_NAME(calloc)(size_t nelem, size_t elsize);

#endif /* __LIBDAMEMALLOC_H__ */
