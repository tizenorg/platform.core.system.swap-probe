#include <stddef.h>
#include "api_names_global.h"
#include "binproto.h"
//#include "libdasocket.h"

#define PROBES_LIST \
	X(PROBE_NAME(socket), "socket") \
	X(PROBE_NAME(accept), "accept") \
	X(PROBE_NAME(accept4), "accept4") \
	X(PROBE_NAME(connect), "connect") \
	X(PROBE_NAME(shutdown), "shutdown") \
	X(PROBE_NAME(bind), "bind") \
	X(PROBE_NAME(listen), "listen") \
	X(PROBE_NAME(send), "send") \
	X(PROBE_NAME(recv), "recv") \
	X(PROBE_NAME(sendto), "sendto") \
	X(PROBE_NAME(recvfrom), "recvfrom") \
	X(PROBE_NAME(recvmsg), "recvmsg") \
	X(PROBE_NAME(sendmsg), "sendmsg") \
	X(PROBE_NAME(getsockopt), "getsockopt") \
	X(PROBE_NAME(setsockopt), "setsockopt") \
	X(PROBE_NAME(getpeername), "getpeername") \
	X(PROBE_NAME(getsockname), "getsockname") \
	X(PROBE_NAME(socketpair), "socketpair") \
	X(PROBE_NAME(sockatmark), "sockatmark") \
	X(PROBE_NAME(isfdtype), "isfdtype") \
	X(PROBE_NAME(select), "select") \
	X(PROBE_NAME(pselect), "pselect") \
	X(PROBE_NAME(poll), "poll") \
	X(PROBE_NAME(ppoll), "ppoll") \
	X(PROBE_NAME(epoll_create), "epoll_create") \
	X(PROBE_NAME(epoll_create1), "epoll_create1") \
	X(PROBE_NAME(epoll_wait), "epoll_wait") \
	X(PROBE_NAME(epoll_pwait), "epoll_pwait") \
	X(PROBE_NAME(epoll_ctl), "epoll_ctl")



/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X

#define X(func_name, orig_name) \
	void CONCAT(func_name, _always)(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
/* For target binaries probes */
#define X(func_name, orig_name) { & func_name, orig_name, GT_TARGET_PROBE },

static struct probe_desc_t network_probes[] = {
	PROBES_LIST
};

#undef X

/* For all binaries probes */
#define X(func_name, orig_name)     \
    { & CONCAT(func_name, _always), orig_name, GT_ALWAYS_PROBE },

static struct probe_desc_t network_always_probes[] = {
	PROBES_LIST
};

#undef X


/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

struct feature_desc_t network_feature = {
	.feature = FL_NETWORK_API_PROBING,
	.cnt = (PROBES_LIST),
	.probes = network_probes
};

struct feature_desc_t network_feature_always = {
	.feature = FL_NETWORK_API_ALWAYS_PROBING,
	.cnt = (PROBES_LIST),
	.probes = network_always_probes
};

#undef X

#undef PROBES_LIST
