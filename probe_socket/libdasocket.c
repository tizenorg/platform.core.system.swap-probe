/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 * Anastasia Lyupa <a.lyupa@samsung.com>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Contributors:
 * - S-Core Co., Ltd
 * - Samsung RnD Institute Russia
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

#include <errno.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dautil.h"
#include "dahelper.h"
#include "da_socket.h"

#include "binproto.h"

static enum DaOptions _sopt = OPT_FILE;

int socket(int domain, int type, int protocol)
{
	static int (*socketp)(int domain, int type, int protocol);
	
	BEFORE_ORIGINAL_NOFILTER(socket, LIBC);

	ret = socketp(domain, type, protocol);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_socket,
				 ret, 0, ret, FD_API_OPEN, "ddd", domain, type, protocol);

	return ret;
}

int socketpair(int domain, int type, int protocol,int socket_vector[2])
{
	static int (*socketpairp)(int domain, int type, int protocol,int socket_vector[2]);
	
	BEFORE_ORIGINAL_NOFILTER(socketpair, LIBC);
	
	ret = socketpairp(domain, type, protocol, socket_vector);

	//TODO: socket pair: socket_vector[0]/socket_vector[1], FD - ?
	AFTER_PACK_ORIGINAL_SOCK(API_ID_socketpair,
				 ret, 0, socket_vector[0], FD_API_OPEN, 
				 "dddp", domain, type, protocol, socket_vector);
	
	return ret;
}

int shutdown(int socket, int how)
{
	static int (*shutdownp)(int socket, int how);
	
	BEFORE_ORIGINAL_NOFILTER(shutdown, LIBC);
	
	ret = shutdownp(socket, how);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_shutdown,
				 ret, 0, socket, FD_API_OTHER, "dd", socket, how);

	return ret;
}

int bind(int socket, const struct sockaddr *address, socklen_t address_len)
{
	static int (*bindp)(int socket, const struct sockaddr *address, socklen_t address_len);
	
	BEFORE_ORIGINAL(bind, LIBC);
	
	ret = bindp(socket, address, address_len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_bind,
				 ret, 0, socket, FD_API_MANAGE,
				 "dpd", socket, address, address_len);
	
	return ret;
}

int listen(int socket, int backlog)
{
	static int (*listenp)(int socket, int backlog);
	
	BEFORE_ORIGINAL(listen, LIBC);
	
	ret = listenp(socket, backlog);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_listen,
				 ret, 0, socket, FD_API_MANAGE, "dd", socket, backlog);

	return ret;
}

int accept(int socket, struct sockaddr *address, socklen_t *address_len)
{
	static int (*acceptp)(int socket, struct sockaddr *address, socklen_t *address_len);
	
	BEFORE_ORIGINAL_NOFILTER(accept, LIBC);
	
	ret = acceptp(socket, address, address_len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_accept,
				 ret, 0, socket, FD_API_MANAGE,
				 "dpp", socket, address, address_len);
	
	return ret;
}

int accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags)
{
	static int (*accept4p)(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);
	
	BEFORE_ORIGINAL_NOFILTER(accept4, LIBC);
	
	ret = accept4p(sockfd, addr, addrlen, flags);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_accept4,
				 ret, 0, sockfd, FD_API_MANAGE,
				 "dppd", sockfd, addr, addrlen, flags);

	return ret;
}

int connect(int socket, const struct sockaddr *address, socklen_t address_len)
{
	static int (*connectp)(int socket, const struct sockaddr *address, socklen_t address_len);
	
	BEFORE_ORIGINAL(connect, LIBC);
	
	ret = connectp(socket, address, address_len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_connect,
				 ret, address_len, socket, FD_API_MANAGE,
				 "dpd", socket, address, address_len);
	
	return ret;
}

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	static int (*selectp)(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
	
	BEFORE_ORIGINAL(select, LIBC);
	
	ret = selectp(nfds, readfds, writefds,exceptfds, timeout);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_select,
				 ret, 0, nfds, FD_API_MANAGE,
				 "dpppp", nfds, readfds, writefds, exceptfds, timeout);
			
	return ret;
}

int pselect(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, const struct timespec *ntimeout, const sigset_t *sigmask)
{
	static int (*pselectp)(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, const struct timespec *ntimeout, const sigset_t *sigmask);

	BEFORE_ORIGINAL(pselect, LIBC);
	
	ret = pselectp(nfds, readfds, writefds,exceptfds, ntimeout, sigmask);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_pselect,
				 ret, 0, nfds, FD_API_MANAGE,	"dppppp",
				 nfds, readfds, writefds, exceptfds, ntimeout, sigmask);
 
	return ret;
}

ssize_t send(int socket, const void *message, size_t length, int flags)
{
	static ssize_t (*sendp)(int socket, const void *message, size_t length, int flags);
	ssize_t sret;
 
	BEFORE_ORIGINAL(send, LIBC);
	
	sret = sendp(socket, message, length, flags);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_send,
				 sret, sret, socket, FD_API_SEND,
				 "dpxd", socket, message, length, flags);
	
	return sret;
}

ssize_t sendmsg(int socket, const struct msghdr *message, int flags)
{
	static ssize_t (*sendmsgp)(int socket, const struct msghdr *message, int flags);
	ssize_t sret;
 
	BEFORE_ORIGINAL(sendmsg, LIBC);
	
	sret = sendmsgp(socket, message, flags);

 	AFTER_PACK_ORIGINAL_SOCK(API_ID_sendmsg,
				 sret, sret, socket, FD_API_SEND,
				 "dpd", socket, message, flags);
 
	return sret;
}

ssize_t sendto(int socket, const void *message, size_t length, int flags,const struct sockaddr *dest_addr, socklen_t dest_len)
{
	static ssize_t (*sendtop)(int socket, const void *message, size_t length, int flags,const struct sockaddr *dest_addr, socklen_t dest_len);
	ssize_t sret;
 
	BEFORE_ORIGINAL(sendto, LIBC);
	
	sret = sendtop(socket, message, length, flags, dest_addr, dest_len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_sendto,
				 sret, sret, socket, FD_API_SEND,
				 "dpxdpd", socket, message, length, flags, dest_addr, dest_len);
 
	return sret;
}

ssize_t recv(int socket, void *buffer, size_t length, int flags)
{
	static ssize_t (*recvp)(int socket, void *buffer, size_t length, int flags);
	ssize_t sret;
 
	BEFORE_ORIGINAL(recv, LIBC);

	sret = recvp(socket, buffer, length, flags);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_recv,
				 sret, sret, socket, FD_API_RECEIVE,
				 "dpxd", socket, buffer, length, flags);
	
	return sret;
}

ssize_t recvfrom(int socket, void *buffer, size_t length, int flags, struct sockaddr *address, socklen_t *address_len)
{
	static ssize_t (*recvfromp)(int socket, void *buffer, size_t length, int flags, struct sockaddr *address, socklen_t *address_len);
	ssize_t sret;

	BEFORE_ORIGINAL(recvfrom, LIBC);
	
	sret = recvfromp(socket, buffer, length, flags, address, address_len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_recvfrom,
				 sret, sret, socket, FD_API_RECEIVE,
				 "dpxdpp", socket, buffer, length, flags, address, address_len);

	return sret;
}

ssize_t recvmsg(int socket, struct msghdr *message, int flags)
{
	static ssize_t (*recvmsgp)(int socket, struct msghdr *message, int flags);
	ssize_t sret;
 
	BEFORE_ORIGINAL(recvmsg, LIBC);
	
	sret = recvmsgp(socket, message, flags);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_recvmsg,
				 sret, sret, socket, FD_API_RECEIVE,
				 "dpd", socket, message, flags);

	return sret;
}

uint32_t htonl(uint32_t hostlong)
{
	static uint32_t (*htonlp)(uint32_t hostlong);
	uint32_t uret;
 
	BEFORE_ORIGINAL(htonl, LIBC);
	
	uret = htonlp(hostlong);

 	AFTER_PACK_ORIGINAL_SOCK(API_ID_htonl,
				 uret, 0, 0, FD_API_OTHER, "d", hostlong); 
 
	return uret;
}

uint16_t htons(uint16_t hostshort)
{
	static uint16_t (*htonsp)(uint16_t hostshort);
	uint16_t uret;
 
	BEFORE_ORIGINAL(htons, LIBC);
	
	uret = htonsp(hostshort);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_htons,
				 uret, 0, 0, FD_API_OTHER, "d", hostshort); 
 
	return uret;
}

uint32_t ntohl(uint32_t netlong)
{
	static uint32_t (*ntohlp)(uint32_t netlong);
	uint32_t uret;
 
	BEFORE_ORIGINAL(ntohl, LIBC);
	
	uret = ntohlp(netlong);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_ntohl,
				 uret, 0, 0, FD_API_OTHER, "d", netlong); 

	return uret;
}

uint16_t ntohs(uint16_t netshort)
{
	static uint16_t (*ntohsp)(uint16_t netshort);
	uint16_t uret;
 
	BEFORE_ORIGINAL(ntohs, LIBC);
	
	uret = ntohsp(netshort);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_ntohs,
				 uret, 0, 0, FD_API_OTHER, "d", netshort); 

	return uret;
}

#if 0
uint16_t htobe16(uint16_t host_16bits)
{
	static uint16_t (*htobe16p)(uint16_t host_16bits);
	uint16_t uret;
 
	BEFORE_ORIGINAL(htobe16, LIBC);
	
	uret = htobe16p(host_16bits);
 
	AFTER_PACK_ORIGINAL_SOCK(uret, 0, 0, FD_API_OTHER, "d", host_16bits); 

	return uret;
}

uint16_t htole16(uint16_t host_16bits)
{
	static uint16_t (*htole16p)(uint16_t host_16bits);
	uint16_t uret;
 
	BEFORE_ORIGINAL(htole16, LIBC);
	
	uret = htole16p(host_16bits);
 
	AFTER_PACK_ORIGINAL_SOCK(uret, 0, 0, FD_API_OTHER, "d", host_16bits);

	return uret;
}

uint16_t be16toh(uint16_t big_endian_16bits)
{
	static uint16_t (*be16tohp)(uint16_t big_endian_16bits);
	uint16_t uret;
 
	BEFORE_ORIGINAL(be16toh, LIBC);
	
	uret = be16tohp(big_endian_16bits);

	AFTER_PACK_ORIGINAL_SOCK(uret, 0, 0, FD_API_OTHER, "d", big_endian_16bits); 
 
	return uret;
}

uint16_t le16toh(uint16_t little_endian_16bits)
{
	static uint16_t (*le16tohp)(uint16_t little_endian_16bits);
	uint16_t uret;
 
	BEFORE_ORIGINAL(le16toh, LIBC);
	
	uret = le16tohp(little_endian_16bits);

	AFTER_PACK_ORIGINAL_SOCK(uret, 0, 0, FD_API_OTHER, "d", little_endian_16bits); 
 
	return uret;
}

uint32_t htobe32(uint32_t host_32bits)
{
	static uint32_t (*htobe32p)(uint32_t host_32bits);
	uint32_t uret;
 
	BEFORE_ORIGINAL(htobe32, LIBC);

	uret = htobe32p(host_32bits);

	AFTER_PACK_ORIGINAL_SOCK(uret, 0, 0, FD_API_OTHER, "d", host_32bits); 

	return uret;
}

uint32_t htole32(uint32_t host_32bits)
{
	static uint32_t (*htole32p)(uint32_t host_32bits);
	uint32_t uret;
 
	BEFORE_ORIGINAL(htole32, LIBC);

	uret = htole32p(host_32bits);

	AFTER_PACK_ORIGINAL_SOCK(uret, 0, 0, FD_API_OTHER, "d", host_32bits); 

	return uret;
}

uint32_t be32toh(uint32_t big_endian_32bits)
{
	static uint32_t (*be32tohp)(uint32_t big_endian_32bits);
	uint32_t uret;
 
	BEFORE_ORIGINAL(be32toh, LIBC);

	uret = be32tohp(big_endian_32bits);

	AFTER_PACK_ORIGINAL_SOCK(uret, 0, 0, FD_API_OTHER, "d", big_endian_32bits); 

	return uret;
}

uint32_t le32toh(uint32_t little_endian_32bits)
{
	static uint32_t (*le32tohp)(uint32_t little_endian_32bits);
	uint32_t uret;
 
	BEFORE_ORIGINAL(le32toh, LIBC);

	uret = le32tohp(little_endian_32bits);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_le32toh,
				 uret, 0, 0, FD_API_OTHER, "d", little_endian_32bits); 

	return uret;
}

uint64_t htobe64(uint64_t host_64bits)
{
	static uint64_t (*htobe64p)(uint64_t host_64bits);
	uint64_t uret;

	BEFORE_ORIGINAL(htobe64, LIBC);

	uret = htobe64p(host_64bits);
 
	AFTER_PACK_ORIGINAL_SOCK(uret, 0, 0, FD_API_OTHER, "x", host_64bits); 

	return uret;
}

uint64_t htole64(uint64_t host_64bits)
{
	static uint64_t (*htole64p)(uint64_t host_64bits);
	uint64_t uret;
 
	BEFORE_ORIGINAL(htole64, LIBC);

	uret = htole64p(host_64bits);

	AFTER_PACK_ORIGINAL_SOCK(uret, 0, 0, FD_API_OTHER, "x", host_64bits); 

	return uret;
}

uint64_t be64toh(uint64_t big_endian_64bits)
{
	static uint64_t (*be64tohp)(uint64_t big_endian_64bits);
	uint64_t uret;
 
	BEFORE_ORIGINAL(be64toh, LIBC);

	uret = be64tohp(big_endian_64bits);

	AFTER_PACK_ORIGINAL_SOCK(uret, 0, 0, FD_API_OTHER, "x", big_endian_64bits); 

	return uret;
}

uint64_t le64toh(uint64_t little_endian_64bits)
{
	static uint64_t (*le64tohp)(uint64_t little_endian_64bits);
	uint64_t uret;
 
	BEFORE_ORIGINAL(le64toh, LIBC);

	uret = le64tohp(little_endian_64bits);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_le64toh,
				 uret, 0, 0, FD_API_OTHER, "x", little_endian_64bits); 

	return uret;
}
#endif

#if 1
int inet_aton(const char *cp, struct in_addr *inp)
{
	static int (*inet_atonp)(const char *cp, struct in_addr *inp);

	BEFORE_ORIGINAL(inet_aton, LIBC);
	
	ret = inet_atonp(cp,inp);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_inet_aton,
				 ret, 0, 0, FD_API_OTHER, "pp", cp, inp);
 
	return ret;
}

in_addr_t inet_addr(const char *cp)
{
	static in_addr_t (*inet_addrp)(const char *cp);
	in_addr_t iret;
 
	BEFORE_ORIGINAL(inet_addr, LIBC);
 
	iret = inet_addrp(cp);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_inet_addr,
				 iret, 0, 0, FD_API_OTHER, "p", cp);
	
	return iret;
}

in_addr_t inet_network(const char *cp)
{
	static in_addr_t (*inet_networkp)(const char *cp);
	in_addr_t iret;
 
	BEFORE_ORIGINAL(inet_network, LIBC);
 
	iret = inet_networkp(cp);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_inet_network,
				 iret, 0, 0, FD_API_OTHER, "p", cp);

	return iret;
}

char *inet_ntoa(struct in_addr in)
{
	static char * (*inet_ntoap)(struct in_addr in);
	char* sret;
 
	BEFORE_ORIGINAL(inet_ntoa, LIBC);
 
	sret = inet_ntoap(in);

 	AFTER_PACK_ORIGINAL_SOCK(API_ID_inet_ntoa, sret, 0, 0, FD_API_OTHER, "d", in.s_addr);
 
	return sret;
}

#if 0
struct in_addr inet_makeaddr(int net, int host)
{
	static struct in_addr (*inet_makeaddrp)(int net, int host);
	struct in_addr iret;
 
	BEFORE_ORIGINAL(inet_makeaddr, LIBC);
 
	iret = inet_makeaddrp(net,host);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_inet_makeaddr,
				 iret.s_addr, 0, 0, FD_API_OTHER,
				 "dd", net, host);

	return iret;
}
#endif

in_addr_t inet_lnaof(struct in_addr in)
{
	static in_addr_t (*inet_lnaofp)(struct in_addr in);
	in_addr_t iret;
 
	BEFORE_ORIGINAL(inet_lnaof, LIBC);
 
	iret = inet_lnaofp(in);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_inet_lnaof,
				 iret, 0, 0, FD_API_OTHER, "d", in.s_addr);
 
	return iret;
}

in_addr_t inet_netof(struct in_addr in)
{
	static in_addr_t (*inet_netofp)(struct in_addr in);
	in_addr_t iret;
 
	BEFORE_ORIGINAL(inet_netof, LIBC);
 
	iret = inet_netofp(in);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_inet_netof,
				 iret, 0, 0, FD_API_OTHER, "d", in.s_addr);
 
	return iret;
}

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
	static const char* (*inet_ntopp)(int af, const void *src, char *dst, socklen_t size);
	const char* cret;
 
	BEFORE_ORIGINAL(inet_ntop, LIBC);
 
	cret = inet_ntopp(af, src, dst, size);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_inet_ntop,
				 cret, size, 0, FD_API_OTHER,
				 "dppd", af, src, dst, size);
 
	return cret;
}

int inet_pton(int af, const char *src, void *dst)
{
	static int (*inet_ptonp)(int af, const char *src, void *dst);
 
	BEFORE_ORIGINAL(inet_pton, LIBC);
 
	ret = inet_ptonp(af, src, dst);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_inet_pton,
				 ret, 0, 0, FD_API_OTHER, "%dpp", af, src, dst);
 
	return ret;
}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
	static int (*getaddrinfop)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
 
	BEFORE_ORIGINAL_NOFILTER(getaddrinfo, LIBC);
 
	ret = getaddrinfop(node, service, hints, res);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getaddrinfo,
				 ret, 0, 0, FD_API_OTHER, "pppp", node, service, hints, res);

	return ret;
}

void freeaddrinfo(struct addrinfo *res)
{
	static void (*freeaddrinfop)(struct addrinfo *res);

	BEFORE_ORIGINAL_NOFILTER(freeaddrinfo, LIBC);
 
 	freeaddrinfop(res);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_freeaddrinfo,
				 0, 0, 0, FD_API_OTHER, "p", res);
}

const char *gai_strerror(int errcode)
{
	static const char * (*gai_strerrorp)(int errcode);
	const char * cret;
 
	BEFORE_ORIGINAL(gai_strerror, LIBC);
 
	cret = gai_strerrorp(errcode);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gai_strerror,
				 cret, 0, 0, FD_API_OTHER, "d", errcode);

	return cret;
}

int gai_suspend(const struct gaicb* const list[], int nitems, const struct timespec *timeout)
{
	static int (*gai_suspendp)(const struct gaicb* const list[], int nitems, const struct timespec *timeout);

	BEFORE_ORIGINAL(gai_suspend, LIBC);
 
	ret = gai_suspendp(list,nitems,timeout);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gai_suspend,
				 ret, 0, 0, FD_API_OTHER, "pdp", list, nitems, timeout);

	return ret;
}

int gai_error(struct gaicb *req)
{
	static int (*gai_errorp)(struct gaicb *req);
 
	BEFORE_ORIGINAL(gai_error, LIBC);
 
	ret = gai_errorp(req);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gai_error,
				 ret, 0, 0, FD_API_OTHER, "p", req);
 
	return ret;
}

int gai_cancel(struct gaicb *req)
{
	static int (*gai_cancelp)(struct gaicb *req);
 
	BEFORE_ORIGINAL(gai_cancel, LIBC);
 
	ret = gai_cancelp(req);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gai_cancel,
				 ret, 0, 0, FD_API_OTHER, "p", req);
 
	return ret;
}

int getaddrinfo_a(int mode, struct gaicb *list[], int nitems, struct sigevent *sevp)
{
	static int (*getaddrinfo_ap)(int mode, struct gaicb *list[], int nitems, struct sigevent *sevp);
 
	BEFORE_ORIGINAL(getaddrinfo_a, LIBC);
 
	ret = getaddrinfo_ap(mode, list,nitems, sevp);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getaddrinfo_a,
				 ret, 0, 0, FD_API_OTHER, "dpdp", mode, list, nitems, sevp);
 
	return ret;
}
#endif

int getsockopt(int socket, int level, int option_name, void *option_value, socklen_t *option_len)
{
	static int (*getsockoptp)(int socket, int level, int option_name, void *option_value, socklen_t *option_len);
 
	BEFORE_ORIGINAL(getsockopt, LIBC);
 
	ret = getsockoptp(socket, level, option_name, option_value, option_len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getsockopt,
				 ret, 0, socket, FD_API_OPTION, "dddpp",
				 socket, level, option_name, option_value, option_len);
 
	return ret;
}

int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len)
{
	static int (*setsockoptp)(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
 
	BEFORE_ORIGINAL(setsockopt, LIBC);

	ret = setsockoptp(socket, level, option_name, option_value, option_len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_setsockopt,
				 ret, option_len, socket, FD_API_OPTION, "dddpd",
				 socket, level, option_name, option_value, option_len);

	return ret;
}

#if 1
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen )
{
	static int (*getsocknamep)(int sockfd, struct sockaddr *addr, socklen_t *addrlen );
 
	BEFORE_ORIGINAL(getsockname, LIBC);
 
	ret = getsocknamep(sockfd, addr, addrlen);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getsockname,
				 ret, 0, sockfd, FD_API_OTHER, "dpp", sockfd, addr, addrlen);

	return ret;
}

int getdomainname(char *name, size_t len)
{
	static int (*getdomainnamep)(char *name, size_t len);
 
	BEFORE_ORIGINAL(getdomainname, LIBC);
 
	ret = getdomainnamep(name, len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getdomainname,
				 ret, len, 0, FD_API_OTHER, "px", name, len);

	return ret;
}

int setdomainname(const char *name, size_t len)
{
	static int (*setdomainnamep)(const char *name, size_t len);
 
	BEFORE_ORIGINAL(setdomainname, LIBC);
 
	ret = setdomainnamep(name, len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_setdomainname,
				 ret, len, 0, FD_API_OTHER, "px", name, len);
 
	return ret;
}

int gethostname(char *name, size_t len)
{
	static int (*gethostnamep)(char *name, size_t len);
 
	BEFORE_ORIGINAL(gethostname, LIBC);
 
	ret = gethostnamep(name, len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gethostname,
				 ret, len, 0, FD_API_OTHER, "px", name, len);
 
	return ret;
}

int sethostname(const char *name, size_t len)
{
	static int (*sethostnamep)(const char *name, size_t len);
 
	BEFORE_ORIGINAL(sethostname, LIBC);
 
	ret = sethostnamep(name, len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_sethostname,
				 ret, len, 0, FD_API_OTHER, "px", name, len);
 
	return ret;
}

int getpeername(int s, struct sockaddr *addr, socklen_t *len)
{
	static int (*getpeernamep)(int s, struct sockaddr *addr, socklen_t *len);

	BEFORE_ORIGINAL(getpeername, LIBC);
 
	ret = getpeernamep(s, addr, len);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getpeername,
				 ret, 0, s, FD_API_OTHER, "dpp", s, addr, len);
 
	return ret;
}

int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, unsigned int flags)
{
	static int (*getnameinfop)(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, unsigned int flags);
 
	BEFORE_ORIGINAL(getnameinfo, LIBC);
 
	ret = getnameinfop(sa, salen,host, hostlen, serv, servlen, flags);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getnameinfo,
				 ret, 0, 0, FD_API_OTHER, "pdpdpdd",
				 sa, salen, host, hostlen, serv, servlen, flags);
 
	return ret;
}

struct hostent *gethostbyname(const char *name)
{
	static struct hostent * (*gethostbynamep)(const char *name);
	struct hostent* pret;
 
	BEFORE_ORIGINAL(gethostbyname, LIBC);

	pret = gethostbynamep(name);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gethostbyname,
				 pret, 0, 0, FD_API_OTHER, "p", name);

	return pret;
}

struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type)
{
	static struct hostent * (*gethostbyaddrp)(const void *addr, socklen_t len, int type);
	struct hostent* pret;
 
	BEFORE_ORIGINAL(gethostbyaddr, LIBC);
 
	pret = gethostbyaddrp(addr, len, type);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gethostbyaddr,
				 pret, 0, 0, FD_API_OTHER, "pdd", addr, len, type);

	return pret;
}

void sethostent(int stayopen)
{
	static void (*sethostentp)(int stayopen);

	BEFORE_ORIGINAL(sethostent, LIBC);
 
	sethostentp(stayopen);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_sethostent,
				 0, 0, 0, FD_API_OTHER, "d", stayopen);
}

void endhostent(void)
{
	static void (*endhostentp)(void);

	BEFORE_ORIGINAL(endhostent, LIBC);

	endhostentp();

	AFTER_PACK_ORIGINAL_SOCK(API_ID_endhostent,
				 0, 0, 0, FD_API_OTHER, "", 0);
}

void herror(const char *s)
{
	static void (*herrorp)(const char *s);

	BEFORE_ORIGINAL(herror, LIBC);
 
	herrorp(s);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_herror,
				 0, 0, 0, FD_API_OTHER, "p", s);
}

const char *hstrerror(int err)
{
	static const char* (*hstrerrorp)(int err);
	const char* cret;
 
	BEFORE_ORIGINAL(hstrerror, LIBC);
 
	cret = hstrerrorp(err);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_hstrerror,
				 cret, 0, 0, FD_API_OTHER, "d", err);

	return cret;
}

struct hostent *gethostent(void)
{
	static struct hostent* (*gethostentp)(void);
	struct hostent* pret;
 
	BEFORE_ORIGINAL(gethostent, LIBC);
 
	pret = gethostentp();

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gethostent,
				 pret, 0, 0, FD_API_OTHER, "", 0);

	return pret;
}

struct hostent *gethostbyname2(const char *name, int af)
{
	static struct hostent * (*gethostbyname2p)(const char *name, int af);
	struct hostent* pret;
 
	BEFORE_ORIGINAL(gethostbyname2, LIBC);
 
	pret = gethostbyname2p(name, af);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gethostbyname2,
				 pret, 0, 0, FD_API_OTHER, "pd", name, af);

	return pret;
}

int gethostent_r(struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop)
{
	static int (*gethostent_rp)(struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(gethostent_r, LIBC);
	
	ret = gethostent_rp(rret, buf, buflen, result, h_errnop);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gethostent_r,
				 ret, 0, 0, FD_API_OTHER, "ppxpp",
				 rret, buf, buflen, result, h_errnop);

	return ret;
}

int gethostbyaddr_r(const void *addr, socklen_t len, int type, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop)
{
	static int (*gethostbyaddr_rp)(const void *addr, socklen_t len, int type, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);

	BEFORE_ORIGINAL(gethostbyaddr_r, LIBC);

	ret = gethostbyaddr_rp(addr, len, type, rret, buf, buflen, result, h_errnop);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gethostbyaddr_r,
				 ret, 0, 0, FD_API_OTHER, "pddppxpp",
				 addr, len, type, rret, buf, buflen, result, h_errnop);

	return ret;
}

int gethostbyname_r(const char *name, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop)
{
	static int (*gethostbyname_rp)(const char *name, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(gethostbyname_r, LIBC);

	ret = gethostbyname_rp(name, rret, buf, buflen, result, h_errnop);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gethostbyname_r,
				 ret, 0, 0, FD_API_OTHER, "pppxpp",
				 name, rret, buf, buflen, result, h_errnop);

	return ret;
}

int gethostbyname2_r(const char *name, int af, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop)
{
	static int (*gethostbyname2_rp)(const char *name, int af, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(gethostbyname2_r, LIBC);
 
	ret = gethostbyname2_rp(name, af, rret, buf, buflen, result, h_errnop);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_gethostbyname2_r,
				 ret, 0, 0, FD_API_OTHER, "pdppxpp",
				 name, af, rret, buf, buflen, result, h_errnop);
 
	return ret;
}

struct servent *getservbyname(const char *name, const char *proto)
{
	static struct servent * (*getservbynamep)(const char *name, const char *proto);
	struct servent* pret;
 
	BEFORE_ORIGINAL(getservbyname, LIBC);
 
	pret = getservbynamep(name, proto);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getservbyname,
				 pret, 0, 0, FD_API_OTHER, "pp", name, proto);

	return pret;
}

void setservent(int stayopen)
{
	static void (*setserventp)(int stayopen);

	BEFORE_ORIGINAL(setservent, LIBC);
 
	setserventp(stayopen);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_setservent,
				 0, 0, 0, FD_API_OTHER, "d", stayopen);
}

void endservent(void)
{
	static void (*endserventp)(void);

	BEFORE_ORIGINAL(endservent, LIBC);
 
	endserventp();

	AFTER_PACK_ORIGINAL_SOCK(API_ID_endservent,
				 0, 0, 0, FD_API_OTHER, "", 0);
}

struct servent *getservent(void)
{
	static struct servent * (*getserventp)(void);
	struct servent* pret;
 
	BEFORE_ORIGINAL(getservent, LIBC);
 
	pret = getserventp();

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getservent,
				 pret, 0, 0, FD_API_OTHER, "", 0);

	return pret;
}

struct servent *getservbyport(int port, const char *proto)
{
	static struct servent * (*getservbyportp)(int port, const char *proto);
	struct servent* pret;
 
	BEFORE_ORIGINAL(getservbyport, LIBC);
 
	pret = getservbyportp(port, proto);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getservbyport,
				 pret, 0, 0, FD_API_OTHER, "dp", port, proto);

	return pret;
}

int getservent_r(struct servent *result_buf, char *buf, size_t buflen, struct servent **result)
{
	static int (*getservent_rp)(struct servent *result_buf, char *buf, size_t buflen, struct servent **result);
 
	BEFORE_ORIGINAL(getservent_r, LIBC);
 
	ret = getservent_rp(result_buf, buf, buflen, result);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getservent_r,
				 ret, 0, 0, FD_API_OTHER, "ppxp",
				 result_buf, buf, buflen, result);

	return ret;
}

int getservbyname_r(const char *name, const char *proto, struct servent *result_buf, char *buf, size_t buflen, struct servent **result)
{
	static int (*getservbyname_rp)(const char *name, const char *proto, struct servent *result_buf, char *buf, size_t buflen, struct servent **result);
 
	BEFORE_ORIGINAL(getservbyname_r, LIBC);
 
	ret = getservbyname_rp(name, proto, result_buf, buf, buflen, result);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getservbyname_r,
				 ret, 0, 0, FD_API_OTHER, "ppppxp",
				 name, proto, result_buf, buf, buflen, result);

	return ret;
}

int getservbyport_r(int port, const char *proto, struct servent *result_buf, char *buf, size_t buflen, struct servent **result)
{
	static int (*getservbyport_rp)(int port, const char *proto, struct servent *result_buf, char *buf, size_t buflen, struct servent **result);
 
	BEFORE_ORIGINAL(getservbyport_r, LIBC);
 
	ret = getservbyport_rp(port, proto, result_buf, buf, buflen, result);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getservbyport_r,
				 ret, 0, 0, FD_API_OTHER, "dpppxp",
				 port, proto, result_buf, buf, buflen, result);

	return ret;
}

struct netent* getnetent(void)
{
	static struct netent * (*getnetentp)(void);
	struct netent* pret;
 
	BEFORE_ORIGINAL(getnetent, LIBC);
 
	pret = getnetentp();

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getnetent,
				 pret, 0, 0, FD_API_OTHER, "", 0);

	return pret;
}

struct netent *getnetbyname(const char *name)
{
	static struct netent * (*getnetbynamep)(const char *name);
	struct netent* pret;
 
	BEFORE_ORIGINAL(getnetbyname, LIBC);
 
	pret = getnetbynamep(name);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getnetbyname,
				 pret, 0, 0, FD_API_OTHER, "p", name);
 
	return pret;
}

struct netent *getnetbyaddr(uint32_t net, int type)
{
	static struct netent * (*getnetbyaddrp)(uint32_t net, int type);
	struct netent * pret;
 
	BEFORE_ORIGINAL(getnetbyaddr, LIBC);

	pret = getnetbyaddrp(net, type);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getnetbyaddr,
				 pret, 0, 0, FD_API_OTHER, "dd", net, type);

	return pret;
}

void setnetent(int stayopen)
{
	static void (*setnetentp)(int stayopen);

	BEFORE_ORIGINAL(setnetent, LIBC);
 
	setnetentp(stayopen);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_setnetent,
				 0, 0, 0, FD_API_OTHER, "d", stayopen);
}

void endnetent(void)
{
	static void (*endnetentp)(void);

	BEFORE_ORIGINAL(endnetent, LIBC);
 
	endnetentp();

	AFTER_PACK_ORIGINAL_SOCK(API_ID_endnetent,
				 0, 0, 0, FD_API_OTHER, "", 0);
}

int getnetent_r(struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop)
{
	static int (*getnetent_rp)(struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(getnetent_r, LIBC);
 
	ret = getnetent_rp(result_buf, buf, buflen, result, h_errnop);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getnetent_r,
				 ret, 0, 0, FD_API_OTHER, "ppxpp",
				 result_buf, buf, buflen, result, h_errnop);

	return ret;
}

int getnetbyname_r(const char *name, struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop)
{
	static int (*getnetbyname_rp)(const char *name, struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(getnetbyname_r, LIBC);
 
	ret = getnetbyname_rp(name,result_buf, buf, buflen, result, h_errnop);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getnetbyname_r,
				 ret, 0, 0, FD_API_OTHER, "pppxpp",
				 name, result_buf, buf, buflen, result, h_errnop);

	return ret;
}

int getnetbyaddr_r(uint32_t net, int type, struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop)
{
	static int (*getnetbyaddr_rp)(uint32_t net, int type, struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(getnetbyaddr_r, LIBC);
 
	ret = getnetbyaddr_rp(net, type, result_buf, buf, buflen, result, h_errnop);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getnetbyaddr_r,
				 ret, 0, 0, FD_API_OTHER, "ddppxpp",
				 net, type, result_buf, buf, buflen, result, h_errnop);

	return ret;
}

struct protoent *getprotoent(void)
{
	static struct protoent * (*getprotoentp)(void);
	struct protoent * pret;
 
	BEFORE_ORIGINAL(getprotoent, LIBC);
 
	pret = getprotoentp();

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getprotoent,
				 pret, 0, 0, FD_API_OTHER, "", 0);
 
	return pret;
}

struct protoent *getprotobyname(const char *name)
{
	static struct protoent * (*getprotobynamep)(const char *name);
	struct protoent * pret;
 
	BEFORE_ORIGINAL(getprotobyname, LIBC);
 
	pret = getprotobynamep(name);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getprotobyname,
				 pret, 0, 0, FD_API_OTHER, "p", name);

	return pret;
}

struct protoent *getprotobynumber(int proto)
{
	static struct protoent * (*getprotobynumberp)(int proto);
	struct protoent * pret;
 
	BEFORE_ORIGINAL(getprotobynumber, LIBC);
 
	pret = getprotobynumberp(proto);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getprotobynumber,
				 pret, 0, 0, FD_API_OTHER, "d", proto);

	return pret;
}

void setprotoent(int stayopen)
{
	static void (*setprotoentp)(int stayopen);

	BEFORE_ORIGINAL(setprotoent, LIBC);
 
	setprotoentp(stayopen);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_setprotoent,
				 0, 0, 0, FD_API_OTHER, "d", stayopen);
}

void endprotoent(void)
{
	static void (*endprotoentp)(void);

	BEFORE_ORIGINAL(endprotoent, LIBC);
 
	endprotoentp();
 
	AFTER_PACK_ORIGINAL_SOCK(API_ID_endprotoent,
				 0, 0, 0, FD_API_OTHER, "", 0);
}

int getprotoent_r(struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result)
{
	static int (*getprotoent_rp)(struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result);
 
	BEFORE_ORIGINAL(getprotoent_r, LIBC);
 
	ret = getprotoent_rp(result_buf, buf, buflen, result);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getprotoent_r,
				 ret, 0, 0, FD_API_OTHER, "ppxp",
				 result_buf, buf, buflen, result);

	return ret;
}

int getprotobyname_r(const char *name, struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result)
{
	static int (*getprotobyname_rp)(const char *name, struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result);
 
	BEFORE_ORIGINAL(getprotobyname_r, LIBC);
 
	ret = getprotobyname_rp(name, result_buf, buf, buflen, result);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getprotobyname_r,
				 ret, 0, 0, FD_API_OTHER, "pppxp",
				 name, result_buf, buf, buflen, result);

	return ret;
}

int getprotobynumber_r(int proto, struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result)
{
	static int (*getprotobynumber_rp)(int proto, struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result);
 
	BEFORE_ORIGINAL(getprotobynumber_r, LIBC);
 
	ret = getprotobynumber_rp(proto, result_buf, buf, buflen, result);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getprotobynumber_r,
				 ret, 0, 0, FD_API_OTHER, "dppxp",
				 proto, result_buf, buf, buflen, result);

	return ret;
}

unsigned int if_nametoindex (__const char *__ifname)
{
	static unsigned int (*if_nametoindexp)(__const char *__ifname);
	unsigned int uret;
 
	BEFORE_ORIGINAL(if_nametoindex, LIBC);
 
	uret = if_nametoindexp(__ifname);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_if_nametoindex,
				 uret, 0, 0, FD_API_OTHER, "p", __ifname);
 
	return uret;
}

char *if_indextoname (unsigned int __ifindex, char *__ifname)
{
	static char * (*if_indextonamep)(unsigned int __ifindex, char *__ifname);
	char * cret;
	
	BEFORE_ORIGINAL(if_indextoname, LIBC);
 
	cret = if_indextonamep(__ifindex, __ifname);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_if_indextoname,
				 cret, 0, 0, FD_API_OTHER,
				 "dp", __ifindex, __ifname);

	return cret;
}

struct if_nameindex *if_nameindex (void)
{
	static struct if_nameindex * (*if_nameindexp)(void);
	struct if_nameindex * pret;
 
	BEFORE_ORIGINAL_NOFILTER(if_nameindex, LIBC);
	
	pret = if_nameindexp();

	AFTER_PACK_ORIGINAL_SOCK(API_ID_if_nameindex,
				 pret, 0, 0, FD_API_OTHER, "", 0);

	return pret;
}

void if_freenameindex (struct if_nameindex *__ptr)
{
	static void (*if_freenameindexp)(struct if_nameindex *__ptr);

	BEFORE_ORIGINAL_NOFILTER(if_freenameindex, LIBC);
 
	if_freenameindexp(__ptr);
 
	AFTER_PACK_ORIGINAL_SOCK(API_ID_if_freenameindex,
				 0, 0, 0, FD_API_OTHER, "p", __ptr);
}

int getifaddrs(struct ifaddrs **ifap)
{
	static int (*getifaddrsp)(struct ifaddrs **ifap);
 
	BEFORE_ORIGINAL_NOFILTER(getifaddrs, LIBC);
 
	ret = getifaddrsp(ifap);

	AFTER_PACK_ORIGINAL_SOCK(API_ID_getifaddrs,
				 ret, 0, 0, FD_API_OTHER, "p", ifap);
 
	return ret;
}

void freeifaddrs(struct ifaddrs *ifa)
{
	static void (*freeifaddrsp)(struct ifaddrs *ifa);

	BEFORE_ORIGINAL_NOFILTER(freeifaddrs, LIBC);
 
	freeifaddrsp(ifa);
 
	AFTER_PACK_ORIGINAL_SOCK(API_ID_freeifaddrs,
				 0, 0, 0, FD_API_OTHER, "p", ifa);
}

#if 0
int poll(struct pollfd *ufds, unsigned int nfds, int timeout)
{
	static int (*pollp)(struct pollfd *ufds, unsigned int nfds, int timeout);
 
	BEFORE_ORIGINAL(poll, LIBC);
 
	ret = pollp(ufds, nfds, timeout);

	AFTER_PACK_ORIGINAL_SOCK(ret, timeout, 0, FD_API_OTHER, "pdd", ufds, nfds, timeout);

	return ret;
}

int ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts, const sigset_t *sigmask)
{
	static int (*ppollp)(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts, const sigset_t *sigmask);
 
	BEFORE_ORIGINAL(ppoll, LIBC);
 
	ret = ppollp(fds, nfds, timeout_ts, sigmask);

	AFTER_PACK_ORIGINAL_SOCK(ret, 0, 0, FD_API_OTHER, "pxpp",
			fds, nfds, timeout_ts, sigmask);

	return ret;
}
#endif

#endif
