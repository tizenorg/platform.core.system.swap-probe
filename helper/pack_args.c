
#include "pack_args.h"
#include "binproto.h"



#define pack_add(id, ch, func) (pack_handler_t)func
pack_handler_t pack_handler[] = {
	PACK_CALL_ARRAY()
};
#undef pack_add

#define pack_add(id, ch, func) ch
char pack_type_char[] = {
	PACK_CALL_ARRAY()
};
#undef pack_add


