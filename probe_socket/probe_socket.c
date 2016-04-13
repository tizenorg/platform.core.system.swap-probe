#include <stddef.h>
#include "api_names_global.h"
#include "probeinfo.h"
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
	X(PROBE_NAME(epoll_ctl), "epoll_ctl") \
	X(PROBE_NAME(htonl), "htonl") \
	X(PROBE_NAME(htons), "htons") \
	X(PROBE_NAME(inet_aton), "inet_aton") \
	X(PROBE_NAME(inet_addr), "inet_addr") \
	X(PROBE_NAME(inet_network), "inet_network") \
	X(PROBE_NAME(inet_ntoa), "inet_ntoa") \
	X(PROBE_NAME(ntohl), "ntohl") \
	X(PROBE_NAME(ntohs), "ntohs") \
	X(PROBE_NAME(inet_lnaof), "inet_lnaof") \
	X(PROBE_NAME(inet_netof), "inet_netof") \
	X(PROBE_NAME(inet_ntop), "inet_ntop") \
	X(PROBE_NAME(inet_pton), "inet_pton") \
	X(PROBE_NAME(getaddrinfo), "getaddrinfo") \
	X(PROBE_NAME(freeaddrinfo), "freeaddrinfo") \
	X(PROBE_NAME(gai_strerror), "gai_strerror") \
	X(PROBE_NAME(gai_suspend), "gai_suspend") \
	X(PROBE_NAME(gai_error), "gai_error") \
	X(PROBE_NAME(gai_cancel), "gai_cancel") \
	X(PROBE_NAME(getaddrinfo_a), "getaddrinfo_a") \
	X(PROBE_NAME(getdomainname), "getdomainname") \
	X(PROBE_NAME(setdomainname), "setdomainname") \
	X(PROBE_NAME(gethostname), "gethostname") \
	X(PROBE_NAME(sethostname), "sethostname") \
	X(PROBE_NAME(getnameinfo), "getnameinfo") \
	X(PROBE_NAME(gethostbyname), "gethostbyname") \
	X(PROBE_NAME(gethostbyaddr), "gethostbyaddr") \
	X(PROBE_NAME(sethostent), "sethostent") \
	X(PROBE_NAME(endhostent), "endhostent") \
	X(PROBE_NAME(herror), "herror") \
	X(PROBE_NAME(hstrerror), "hstrerror") \
	X(PROBE_NAME(gethostent), "gethostent") \
	X(PROBE_NAME(gethostbyname2), "gethostbyname2") \
	X(PROBE_NAME(gethostent_r), "gethostent_r") \
	X(PROBE_NAME(gethostbyaddr_r), "gethostbyaddr_r") \
	X(PROBE_NAME(gethostbyname_r), "gethostbyname_r") \
	X(PROBE_NAME(gethostbyname2_r), "gethostbyname2_r") \
	X(PROBE_NAME(getservbyname), "getservbyname") \
	X(PROBE_NAME(setservent), "setservent") \
	X(PROBE_NAME(endservent), "endservent") \
	X(PROBE_NAME(getservent), "getservent") \
	X(PROBE_NAME(getservbyport), "getservbyport") \
	X(PROBE_NAME(getservent_r), "getservent_r") \
	X(PROBE_NAME(getservbyname_r), "getservbyname_r") \
	X(PROBE_NAME(getservbyport_r), "getservbyport_r") \
	X(PROBE_NAME(getnetent), "getnetent") \
	X(PROBE_NAME(getnetbyname), "getnetbyname") \
	X(PROBE_NAME(getnetbyaddr), "getnetbyaddr") \
	X(PROBE_NAME(setnetent), "setnetent") \
	X(PROBE_NAME(endnetent), "endnetent") \
	X(PROBE_NAME(getnetent_r), "getnetent_r") \
	X(PROBE_NAME(getnetbyname_r), "getnetbyname_r") \
	X(PROBE_NAME(getnetbyaddr_r), "getnetbyaddr_r") \
	X(PROBE_NAME(getprotoent), "getprotoent") \
	X(PROBE_NAME(getprotobyname), "getprotobyname") \
	X(PROBE_NAME(getprotobynumber), "getprotobynumber") \
	X(PROBE_NAME(setprotoent), "setprotoent") \
	X(PROBE_NAME(endprotoent), "endprotoent") \
	X(PROBE_NAME(getprotoent_r), "getprotoent_r") \
	X(PROBE_NAME(getprotobyname_r), "getprotobyname_r") \
	X(PROBE_NAME(getprotobynumber_r), "getprotobynumber_r") \
	X(PROBE_NAME(if_nametoindex), "if_nametoindex") \
	X(PROBE_NAME(if_indextoname), "if_indextoname") \
	X(PROBE_NAME(if_nameindex), "if_nameindex") \
	X(PROBE_NAME(if_freenameindex), "if_freenameindex") \
	X(PROBE_NAME(getifaddrs), "getifaddrs") \
	X(PROBE_NAME(freeifaddrs), "freeifaddrs") \
	X(PROBE_NAME(htobe16), "htobe16") \
	X(PROBE_NAME(htole16), "htole16") \
	X(PROBE_NAME(be16toh), "be16toh") \
	X(PROBE_NAME(le16toh), "le16toh") \
	X(PROBE_NAME(htobe32), "htobe32") \
	X(PROBE_NAME(htole32), "htole32") \
	X(PROBE_NAME(be32toh), "be32toh") \
	X(PROBE_NAME(le32toh), "le32toh") \
	X(PROBE_NAME(htobe64), "htobe64") \
	X(PROBE_NAME(htole64), "htole64") \
	X(PROBE_NAME(be64toh), "be64toh") \
	X(PROBE_NAME(le64toh), "le64toh") \
	X(PROBE_NAME(inet_makeaddr), "inet_makeaddr")



/* X-macros replaced by function defenitions */
/* We need this only to get symbol address - so don't care
 * about real prototype */
#define X(func_name, orig_name) \
	void func_name(void);

PROBES_LIST

#undef X


/* X-macros replaced by structures defenitions */
#define X(func_name, orig_name) { & func_name, orig_name },

static struct probe_desc_t network_probes[] = {
	PROBES_LIST
};

#undef X

/* X-macros replaced by counted additions */
#define X(func_name, orig_name) +1

static struct feature_desc_t feature = {
	.feature = FL_NETWORK_API_PROBING,
	.cnt = (PROBES_LIST),
	.probes = network_probes
};

static struct feature_desc_t feature_always = {
	.feature = FL_NETWORK_API_ALWAYS_PROBING,
	.cnt = (PROBES_LIST),
	.probes = network_probes
};

#undef X

#undef PROBES_LIST

struct feature_desc_t *network_feature = &feature;
struct feature_desc_t *network_feature_always = &feature_always;
