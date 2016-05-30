#ifndef __LIBDAMEMALLOC_H__
#define __LIBDAMEMALLOC_H__

HANDLER_WRAPPERS_DEF(void *, malloc, size_t, size);
HANDLER_WRAPPERS_DEF(void, free, void *, ptr);
HANDLER_WRAPPERS_DEF(void *, realloc, void *, memblock, size_t, size);
HANDLER_WRAPPERS_DEF(void *, calloc, size_t, nelem, size_t, elsize);

#endif /* __LIBDAMEMALLOC_H__ */
