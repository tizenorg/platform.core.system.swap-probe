
#ifndef __PACK_ARGS_H__
#define __PACK_ARGS_H__

#include <stdint.h>

#define PACK_CALL_ARRAY()\
pack_add(A_INT32,   'd', pack_int32),\
pack_add(A_VOIDP64, 'p', pack_p64), \
pack_add(A_VOID,    'v', pack_void), \
pack_add(A_SHADER,  's', pack_shader),\
pack_add(A_FILEPATH,'s', pack_filepath), \
/* A_TAIL is last */ pack_add(A_TAIL,    '0', 0)

#define pack_add(id, ret, func) id
enum handler {
	PACK_CALL_ARRAY()
};
#undef pack_add

typedef void *(*pack_handler_t)(char *to, void *);
extern pack_handler_t pack_handler[];
extern char pack_type_char[];

struct packable_t{
	const enum handler h;
	const void *arg;
};

#endif /* __PACK_ARGS_H__ */
