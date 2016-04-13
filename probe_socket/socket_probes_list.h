#ifndef __SOCKET_PROBES_LIST_H__
#define __SOCKET_PROBES_LIST_H__

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

#define X(func_name, orig_name)         \
	GET_FUNC_ID(func_name),

enum network_funcs_id_t {
	PROBES_LIST
};
#undef X

enum network_funcs_id_t get_network_feature_id(void *ptr);

#endif /* __SOCKET_PROBES_LIST_H__ */
