#ifndef __LIBDASOCKET_H__
#define __LIBDASOCKET_H__

int PROBE_NAME(socket)(int domain, int type, int protocol);
int PROBE_NAME(accept)(int socket, struct sockaddr *address, socklen_t *address_len);
int PROBE_NAME(accept4)(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);
int PROBE_NAME(connect)(int socket, const struct sockaddr *address, socklen_t address_len);
int PROBE_NAME(shutdown)(int socket, int how);
int PROBE_NAME(bind)(int socket, const struct sockaddr *address, socklen_t address_len);
int PROBE_NAME(listen)(int socket, int backlog);
ssize_t PROBE_NAME(send)(int socket, const void *message, size_t length, int flags);
ssize_t PROBE_NAME(recv)(int socket, void *buffer, size_t length, int flags);
ssize_t PROBE_NAME(sendto)(int socket, const void *message, size_t length, int flags, const struct sockaddr *dest_addr, socklen_t dest_len);
ssize_t PROBE_NAME(recvfrom)(int socket, void *buffer, size_t length, int flags, struct sockaddr *address, socklen_t *address_len);
ssize_t PROBE_NAME(recvmsg)(int socket, struct msghdr *message, int flags);
ssize_t PROBE_NAME(sendmsg)(int socket, const struct msghdr *message, int flags);
int PROBE_NAME(getsockopt)(int socket, int level, int option_name, void *option_value, socklen_t *option_len);
int PROBE_NAME(setsockopt)(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
int PROBE_NAME(getpeername)(int fd, struct sockaddr *addr, socklen_t *len);
int PROBE_NAME(getsockname)(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int PROBE_NAME(socketpair)(int domain, int type, int protocol, int socket_vector[2]);
int PROBE_NAME(sockatmark)(int __fd);
int PROBE_NAME(isfdtype)(int __fd, int __fdtype);
int PROBE_NAME(select)(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int PROBE_NAME(pselect)(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timespec *ntimeout, const sigset_t *sigmask);
int PROBE_NAME(poll)(struct pollfd *fds, nfds_t nfds, int timeout);
int PROBE_NAME(ppoll)(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts, const sigset_t *sigmask);
int PROBE_NAME(epoll_create)(int __size);
int PROBE_NAME(epoll_create1)(int __flags);
int PROBE_NAME(epoll_wait)(int __epfd, struct epoll_event *__events, int __maxevents, int __timeout);
int PROBE_NAME(epoll_pwait)(int __epfd, struct epoll_event *__events, int __maxevents, int __timeout, __const                          __sigset_t *__ss);
int PROBE_NAME(epoll_ctl)(int __epfd, int __op, int __fd, struct epoll_event *__event);
//uint32_t PROBE_NAME(htonl)(uint32_t hostlong);
//uint16_t PROBE_NAME(htons)(uint16_t hostshort);
//int PROBE_NAME(inet_aton)(const char *cp, struct in_addr *inp);
//in_addr_t PROBE_NAME(inet_addr)(const char *cp);
//in_addr_t PROBE_NAME(inet_network)(const char *cp);
//char *PROBE_NAME(inet_ntoa)(struct in_addr in);
//uint32_t PROBE_NAME(ntohl)(uint32_t netlong);
//uint16_t PROBE_NAME(ntohs)(uint16_t netshort);
//in_addr_t PROBE_NAME(inet_lnaof)(struct in_addr in);
//in_addr_t PROBE_NAME(inet_netof)(struct in_addr in);
//const char *PROBE_NAME(inet_ntop)(int af, const void *src, char *dst, socklen_t size);
//int PROBE_NAME(inet_pton)(int af, const char *src, void *dst);
//int PROBE_NAME(getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
//void PROBE_NAME(freeaddrinfo)(struct addrinfo *res);
//const char *PROBE_NAME(gai_strerror)(int errcode);
//int PROBE_NAME(gai_suspend)(const struct gaicb* const list[], int nitems, const struct timespec *timeout);
//int PROBE_NAME(gai_error)(struct gaicb *req);
//int PROBE_NAME(gai_cancel)(struct gaicb *req);
//int PROBE_NAME(getaddrinfo_a)(int mode, struct gaicb *list[], int nitems, struct sigevent *sevp);
//int PROBE_NAME(getdomainname)(char *name, size_t len);
//int PROBE_NAME(setdomainname)(const char *name, size_t len);
//int PROBE_NAME(gethostname)(char *name, size_t len);
//int PROBE_NAME(sethostname)(const char *name, size_t len);
//int PROBE_NAME(getnameinfo)(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, unsigned int flags);
//struct hostent *PROBE_NAME(gethostbyname)(const char *name);
//struct hostent *PROBE_NAME(gethostbyaddr)(const void *addr, socklen_t len, int type);
//void PROBE_NAME(sethostent)(int stayopen);
//void PROBE_NAME(endhostent)(void);
//void PROBE_NAME(herror)(const char *s);
//const char *PROBE_NAME(hstrerror)(int err);
//struct hostent *PROBE_NAME(gethostent)(void);
//struct hostent *PROBE_NAME(gethostbyname2)(const char *name, int af);
//int PROBE_NAME(gethostent_r)(struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
//int PROBE_NAME(gethostbyaddr_r)(const void *addr, socklen_t len, int type, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
//int PROBE_NAME(gethostbyname_r)(const char *name, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
//int PROBE_NAME(gethostbyname2_r)(const char *name, int af, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
//struct servent *PROBE_NAME(getservbyname)(const char *name, const char *proto);
//void PROBE_NAME(setservent)(int stayopen);
//void PROBE_NAME(endservent)(void);
//struct servent *PROBE_NAME(getservent)(void);
//struct servent *PROBE_NAME(getservbyport)(int port, const char *proto);
//int PROBE_NAME(getservent_r)(struct servent *result_buf, char *buf, size_t buflen, struct servent **result);
//int PROBE_NAME(getservbyname_r)(const char *name, const char *proto, struct servent *result_buf, char *buf, size_t buflen, struct servent **result);
//int PROBE_NAME(getservbyport_r)(int port, const char *proto, struct servent *result_buf, char *buf, size_t buflen, struct servent **result);
//struct netent* PROBE_NAME(getnetent)(void);
//struct netent *PROBE_NAME(getnetbyname)(const char *name);
//struct netent *PROBE_NAME(getnetbyaddr)(uint32_t net, int type);
//void PROBE_NAME(setnetent)(int stayopen);
//void PROBE_NAME(endnetent)(void);
//int PROBE_NAME(getnetent_r)(struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop);
//int PROBE_NAME(getnetbyname_r)(const char *name, struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop);
//int PROBE_NAME(getnetbyaddr_r)(uint32_t net, int type, struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop);
//struct protoent *PROBE_NAME(getprotoent)(void);
//struct protoent *PROBE_NAME(getprotobyname)(const char *name);
//struct protoent *PROBE_NAME(getprotobynumber)(int proto);
//void PROBE_NAME(setprotoent)(int stayopen);
//void PROBE_NAME(endprotoent)(void);
//int PROBE_NAME(getprotoent_r)(struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result);
//int PROBE_NAME(getprotobyname_r)(const char *name, struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result);
//int PROBE_NAME(getprotobynumber_r)(int proto, struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result);
//unsigned int PROBE_NAME(if_nametoindex)(__const char *__ifname);
//char *PROBE_NAME(if_indextoname)(unsigned int __ifindex, char *__ifname);
//struct if_nameindex *PROBE_NAME(if_nameindex)(void);
//void PROBE_NAME(if_freenameindex)(struct if_nameindex *__ptr);
//int PROBE_NAME(getifaddrs)(struct ifaddrs **ifap);
//void PROBE_NAME(freeifaddrs)(struct ifaddrs *ifa);
//uint16_t PROBE_NAME(htobe16)(uint16_t host_16bits);
//uint16_t PROBE_NAME(htole16)(uint16_t host_16bits);
//uint16_t PROBE_NAME(be16toh)(uint16_t big_endian_16bits);
//uint16_t PROBE_NAME(le16toh)(uint16_t little_endian_16bits);
//uint32_t PROBE_NAME(htobe32)(uint32_t host_32bits);
//uint32_t PROBE_NAME(htole32)(uint32_t host_32bits);
//uint32_t PROBE_NAME(be32toh)(uint32_t big_endian_32bits);
//uint32_t PROBE_NAME(le32toh)(uint32_t little_endian_32bits);
//uint64_t PROBE_NAME(htobe64)(uint64_t host_64bits);
//uint64_t PROBE_NAME(htole64)(uint64_t host_64bits);
//uint64_t PROBE_NAME(be64toh)(uint64_t big_endian_64bits);
//uint64_t PROBE_NAME(le64toh)(uint64_t little_endian_64bits);
//struct in_addr PROBE_NAME(inet_makeaddr)(int net, int host);

#endif /* __LIBDASOCKET_H__ */
