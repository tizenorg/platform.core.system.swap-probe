#ifndef __LIBDAMEMMANAGE_H__
#define __LIBDAMEMMANAGE_H__

HANDLER_WRAPPERS_DEF(void *, memset, void *, memblock, int, c, size_t, n);
HANDLER_WRAPPERS_DEF(int, memcmp, const void *, ptr1, const void *, ptr2, size_t, num);
HANDLER_WRAPPERS_DEF(void *, memcpy, void *, destination, const void *, source, size_t, num);


#endif /* __LIBDAMEMMANAGE_H__ */
