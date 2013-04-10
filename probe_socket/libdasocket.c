/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
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
#include "da_socket.h"

static enum DaOptions _sopt = OPT_FILE;

int socket(int domain, int type, int protocol)
{
	static int (*socketp)(int domain, int type, int protocol);
	
	BEFORE_ORIGINAL_NOFILTER(socket, libc.so.6);

	ret = socketp(domain, type, protocol);
	
	POST_PROBEBLOCK_BEGIN(LC_RESOURCE, VT_INT, ret,	"%d, %d, %d", domain, type, protocol);
	POST_PROBEBLOCK_MIDDLE_SOCK(0, ret, FD_API_OPEN, "");
	APPEND_LOG_CALLSTACK();
	POST_PROBEBLOCK_END();

	return ret;
}

int socketpair(int domain, int type, int protocol,int socket_vector[2])
{
	static int (*socketpairp)(int domain, int type, int protocol,int socket_vector[2]);
	
	BEFORE_ORIGINAL_NOFILTER(socketpair, libc.so.6);
	
	ret = socketpairp(domain, type, protocol, socket_vector);
	
	POST_PROBEBLOCK_BEGIN(LC_RESOURCE, VT_INT, ret,
			"%d, %d, %d, %p", domain, type, protocol, socket_vector);
	log.length += sprintf(log.data + log.length, "`,%u`,%d/%d`,%d`,%d`,0`,%s",
			0, socket_vector[0], socket_vector[1], FD_SOCKET, FD_API_OPEN, "");
	APPEND_LOG_CALLSTACK();
	POST_PROBEBLOCK_END();
	
	return ret;
}

int shutdown(int socket, int how)
{
	static int (*shutdownp)(int socket, int how);
	
	BEFORE_ORIGINAL_NOFILTER(shutdown, libc.so.6);
	
	ret = shutdownp(socket, how);
	
	AFTER_ORIGINAL_SOCK(0, socket, FD_API_OTHER, "%d, %d", socket, how);

	return ret;
}

int bind(int socket, const struct sockaddr *address, socklen_t address_len)
{
	static int (*bindp)(int socket, const struct sockaddr *address, socklen_t address_len);
	
	BEFORE_ORIGINAL(bind, libc.so.6);
	
	ret = bindp(socket, address, address_len);

	AFTER_ORIGINAL_SOCK(0, socket, FD_API_MANAGE,
			"%d, %p, %d", socket, address, address_len);
	
	return ret;
}

int listen(int socket, int backlog)
{
	static int (*listenp)(int socket, int backlog);
	
	BEFORE_ORIGINAL(listen, libc.so.6);
	
	ret = listenp(socket, backlog);
	
	AFTER_ORIGINAL_SOCK(0, socket, FD_API_MANAGE, "%d, %d", socket, backlog);

	return ret;
}

int accept(int socket, struct sockaddr *address, socklen_t *address_len)
{
	static int (*acceptp)(int socket, struct sockaddr *address, socklen_t *address_len);
	
	BEFORE_ORIGINAL_NOFILTER(accept, libc.so.6);
	
	ret = acceptp(socket, address, address_len);
	
	AFTER_ORIGINAL_SOCK(0, socket, FD_API_MANAGE,
			"%d, %p, %p", socket, address, address_len);
	
	return ret;
}

int accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags)
{
	static int (*accept4p)(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);
	
	BEFORE_ORIGINAL_NOFILTER(accept4, libc.so.6);
	
	ret = accept4p(sockfd, addr, addrlen, flags);
	
	AFTER_ORIGINAL_SOCK(0, sockfd, FD_API_MANAGE,
			"%d, %p, %p, %d", sockfd, addr, addrlen, flags);

	return ret;
}

int connect(int socket, const struct sockaddr *address, socklen_t address_len)
{
	static int (*connectp)(int socket, const struct sockaddr *address, socklen_t address_len);
	
	BEFORE_ORIGINAL(connect, libc.so.6);
	
	ret = connectp(socket, address, address_len);
	
	AFTER_ORIGINAL_SOCK(address_len, socket, FD_API_MANAGE,
			"%d, %p, %d", socket, address, address_len);
	
	return ret;
}

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	static int (*selectp)(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
	
	BEFORE_ORIGINAL(select, libc.so.6);
	
	ret = selectp(nfds, readfds, writefds,exceptfds, timeout);
	
	AFTER_ORIGINAL_SOCK(0, nfds, FD_API_MANAGE,
			"%d, %p, %p, %p, %p", nfds, readfds, writefds, exceptfds, timeout);
			
	return ret;
}

int pselect(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, const struct timespec *ntimeout, const sigset_t *sigmask)
{
	static int (*pselectp)(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, const struct timespec *ntimeout, const sigset_t *sigmask);

	BEFORE_ORIGINAL(pselect, libc.so.6);
	
	ret = pselectp(nfds, readfds, writefds,exceptfds, ntimeout, sigmask);
	
	AFTER_ORIGINAL_SOCK(0, nfds, FD_API_MANAGE,	"%d, %p, %p, %p, %p, %p",
			nfds, readfds, writefds, exceptfds, ntimeout, sigmask);
 
	return ret;
}

ssize_t send(int socket, const void *message, size_t length, int flags)
{
	static ssize_t (*sendp)(int socket, const void *message, size_t length, int flags);
	ssize_t sret;
 
	BEFORE_ORIGINAL(send, libc.so.6);
	
	sret = sendp(socket, message, length, flags);
	
	AFTER_ORIGINAL_SOCK_RET(VT_SSIZE_T, sret, sret, socket, FD_API_SEND,
			"%d, %p, %u, %d", socket, message, length, flags);
	
	return sret;
}

ssize_t sendmsg(int socket, const struct msghdr *message, int flags)
{
	static ssize_t (*sendmsgp)(int socket, const struct msghdr *message, int flags);
	ssize_t sret;
 
	BEFORE_ORIGINAL(sendmsg, libc.so.6);
	
	sret = sendmsgp(socket, message, flags);
	
	AFTER_ORIGINAL_SOCK_RET(VT_SSIZE_T, sret, sret, socket, FD_API_SEND,
			"%d, %p, %d", socket, message, flags);
 
	return sret;
}

ssize_t sendto(int socket, const void *message, size_t length, int flags,const struct sockaddr *dest_addr, socklen_t dest_len)
{
	static ssize_t (*sendtop)(int socket, const void *message, size_t length, int flags,const struct sockaddr *dest_addr, socklen_t dest_len);
	ssize_t sret;
 
	BEFORE_ORIGINAL(sendto, libc.so.6);
	
	sret = sendtop(socket, message, length, flags, dest_addr, dest_len);
	
	AFTER_ORIGINAL_SOCK_RET(VT_SSIZE_T, sret, sret, socket, FD_API_SEND,
			"%d, %p, %u, %d, %p, %d", socket, message, length, flags, dest_addr, dest_len);
 
	return sret;
}

ssize_t recv(int socket, void *buffer, size_t length, int flags)
{
	static ssize_t (*recvp)(int socket, void *buffer, size_t length, int flags);
	ssize_t sret;
 
	BEFORE_ORIGINAL(recv, libc.so.6);

	sret = recvp(socket, buffer, length, flags);
	
	AFTER_ORIGINAL_SOCK_RET(VT_SSIZE_T, sret, sret, socket, FD_API_RECEIVE,
			"%d, %p, %u, %d", socket, buffer, length, flags);
	
	return sret;
}

ssize_t recvfrom(int socket, void *buffer, size_t length, int flags, struct sockaddr *address, socklen_t *address_len)
{
	static ssize_t (*recvfromp)(int socket, void *buffer, size_t length, int flags, struct sockaddr *address, socklen_t *address_len);
	ssize_t sret;

	BEFORE_ORIGINAL(recvfrom, libc.so.6);
	
	sret = recvfromp(socket, buffer, length, flags, address, address_len);
	
	AFTER_ORIGINAL_SOCK_RET(VT_SSIZE_T, sret, sret, socket, FD_API_RECEIVE,
			"%d, %p, %u, %d, %p, %p", socket, buffer, length, flags, address, address_len);

	return sret;
}

ssize_t recvmsg(int socket, struct msghdr *message, int flags)
{
	static ssize_t (*recvmsgp)(int socket, struct msghdr *message, int flags);
	ssize_t sret;
 
	BEFORE_ORIGINAL(recvmsg, libc.so.6);
	
	sret = recvmsgp(socket, message, flags);
		
	AFTER_ORIGINAL_SOCK_RET(VT_SSIZE_T, sret, sret, socket, FD_API_RECEIVE,
			"%d, %p, %d", socket, message, flags);

	return sret;
}

uint32_t htonl(uint32_t hostlong)
{
	static uint32_t (*htonlp)(uint32_t hostlong);
	uint32_t uret;
 
	BEFORE_ORIGINAL(htonl, libc.so.6);
	
	uret = htonlp(hostlong);
			
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT32_T, uret, 0, FD_API_OTHER, "%u", hostlong); 
 
	return uret;
}

uint16_t htons(uint16_t hostshort)
{
	static uint16_t (*htonsp)(uint16_t hostshort);
	uint16_t uret;
 
	BEFORE_ORIGINAL(htons, libc.so.6);
	
	uret = htonsp(hostshort);
				
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT16_T, uret, 0, FD_API_OTHER, "%u", hostshort); 
 
	return uret;
}

uint32_t ntohl(uint32_t netlong)
{
	static uint32_t (*ntohlp)(uint32_t netlong);
	uint32_t uret;
 
	BEFORE_ORIGINAL(ntohl, libc.so.6);
	
	uret = ntohlp(netlong);
				
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT32_T, uret, 0, FD_API_OTHER, "%u", netlong); 

	return uret;
}

uint16_t ntohs(uint16_t netshort)
{
	static uint16_t (*ntohsp)(uint16_t netshort);
	uint16_t uret;
 
	BEFORE_ORIGINAL(ntohs, libc.so.6);
	
	uret = ntohsp(netshort);
				
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT16_T, uret, 0, FD_API_OTHER, "%u", netshort); 

	return uret;
}

#if 0
uint16_t htobe16(uint16_t host_16bits)
{
	static uint16_t (*htobe16p)(uint16_t host_16bits);
	uint16_t uret;
 
	BEFORE_ORIGINAL(htobe16, libc.so.6);
	
	uret = htobe16p(host_16bits);
				
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT16_T, uret, 0, FD_API_OTHER, "%u", host_16bits); 

	return uret;
}

uint16_t htole16(uint16_t host_16bits)
{
	static uint16_t (*htole16p)(uint16_t host_16bits);
	uint16_t uret;
 
	BEFORE_ORIGINAL(htole16, libc.so.6);
	
	uret = htole16p(host_16bits);
				
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT16_T, uret, 0, FD_API_OTHER, "%u", host_16bits); 

	return uret;
}

uint16_t be16toh(uint16_t big_endian_16bits)
{
	static uint16_t (*be16tohp)(uint16_t big_endian_16bits);
	uint16_t uret;
 
	BEFORE_ORIGINAL(be16toh, libc.so.6);
	
	uret = be16tohp(big_endian_16bits);
				
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT16_T, uret, 0, FD_API_OTHER,
			"%u", big_endian_16bits); 
 
	return uret;
}

uint16_t le16toh(uint16_t little_endian_16bits)
{
	static uint16_t (*le16tohp)(uint16_t little_endian_16bits);
	uint16_t uret;
 
	BEFORE_ORIGINAL(le16toh, libc.so.6);
	
	uret = le16tohp(little_endian_16bits);
				
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT16_T, uret, 0, FD_API_OTHER,
			"%u", little_endian_16bits); 
 
	return uret;
}

uint32_t htobe32(uint32_t host_32bits)
{
	static uint32_t (*htobe32p)(uint32_t host_32bits);
	uint32_t uret;
 
	BEFORE_ORIGINAL(htobe32, libc.so.6);

	uret = htobe32p(host_32bits);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT32_T, uret, 0, FD_API_OTHER,
			"%u", host_32bits); 

	return uret;
}

uint32_t htole32(uint32_t host_32bits)
{
	static uint32_t (*htole32p)(uint32_t host_32bits);
	uint32_t uret;
 
	BEFORE_ORIGINAL(htole32, libc.so.6);

	uret = htole32p(host_32bits);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT32_T, uret, 0, FD_API_OTHER,
			"%u", host_32bits); 

	return uret;
}

uint32_t be32toh(uint32_t big_endian_32bits)
{
	static uint32_t (*be32tohp)(uint32_t big_endian_32bits);
	uint32_t uret;
 
	BEFORE_ORIGINAL(be32toh, libc.so.6);

	uret = be32tohp(big_endian_32bits);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT32_T, uret, 0, FD_API_OTHER,
			"%u", big_endian_32bits); 

	return uret;
}

uint32_t le32toh(uint32_t little_endian_32bits)
{
	static uint32_t (*le32tohp)(uint32_t little_endian_32bits);
	uint32_t uret;
 
	BEFORE_ORIGINAL(le32toh, libc.so.6);

	uret = le32tohp(little_endian_32bits);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT32_T, uret, 0, FD_API_OTHER,
			"%u", little_endian_32bits); 

	return uret;
}

uint64_t htobe64(uint64_t host_64bits)
{
	static uint64_t (*htobe64p)(uint64_t host_64bits);
	uint64_t uret;

	BEFORE_ORIGINAL(htobe64, libc.so.6);

	uret = htobe64p(host_64bits);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT64_T, uret, 0, FD_API_OTHER,
			"%u", host_64bits); 

	return uret;
}

uint64_t htole64(uint64_t host_64bits)
{
	static uint64_t (*htole64p)(uint64_t host_64bits);
	uint64_t uret;
 
	BEFORE_ORIGINAL(htole64, libc.so.6);

	uret = htole64p(host_64bits);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT64_T, uret, 0, FD_API_OTHER,
			"%u", host_64bits); 

	return uret;
}

uint64_t be64toh(uint64_t big_endian_64bits)
{
	static uint64_t (*be64tohp)(uint64_t big_endian_64bits);
	uint64_t uret;
 
	BEFORE_ORIGINAL(be64toh, libc.so.6);

	uret = be64tohp(big_endian_64bits);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT64_T, uret, 0, FD_API_OTHER,
			"%u", big_endian_64bits); 

	return uret;
}

uint64_t le64toh(uint64_t little_endian_64bits)
{
	static uint64_t (*le64tohp)(uint64_t little_endian_64bits);
	uint64_t uret;
 
	BEFORE_ORIGINAL(le64toh, libc.so.6);

	uret = le64tohp(little_endian_64bits);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT64_T, uret, 0, FD_API_OTHER,
			"%u", little_endian_64bits); 

	return uret;
}
#endif

#if 1
int inet_aton(const char *cp, struct in_addr *inp)
{
	static int (*inet_atonp)(const char *cp, struct in_addr *inp);

	BEFORE_ORIGINAL(inet_aton, libc.so.6);
	
	ret = inet_atonp(cp,inp);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %p", cp, inp);
 
	return ret;
}

in_addr_t inet_addr(const char *cp)
{
	static in_addr_t (*inet_addrp)(const char *cp);
	in_addr_t iret;
 
	BEFORE_ORIGINAL(inet_addr, libc.so.6);
 
	iret = inet_addrp(cp);
	
	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, iret, 0, FD_API_OTHER, "%p", cp);
	
	return iret;
}

in_addr_t inet_network(const char *cp)
{
	static in_addr_t (*inet_networkp)(const char *cp);
	in_addr_t iret;
 
	BEFORE_ORIGINAL(inet_network, libc.so.6);
 
	iret = inet_networkp(cp);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, iret, 0, FD_API_OTHER, "%p", cp);

	return iret;
}

char *inet_ntoa(struct in_addr in)
{
	static char * (*inet_ntoap)(struct in_addr in);
	char* sret;
 
	BEFORE_ORIGINAL(inet_ntoa, libc.so.6);
 
	sret = inet_ntoap(in);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, sret, 0, FD_API_OTHER, "%u", in.s_addr);
 
	return sret;
}

#if 0
struct in_addr inet_makeaddr(int net, int host)
{
	static struct in_addr (*inet_makeaddrp)(int net, int host);
	struct in_addr iret;
 
	BEFORE_ORIGINAL(inet_makeaddr, libc.so.6);
 
	iret = inet_makeaddrp(net,host);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT32_T, iret.s_addr, 0, FD_API_OTHER,
			"%d, %d", net, host);

	return iret;
}
#endif

in_addr_t inet_lnaof(struct in_addr in)
{
	static in_addr_t (*inet_lnaofp)(struct in_addr in);
	in_addr_t iret;
 
	BEFORE_ORIGINAL(inet_lnaof, libc.so.6);
 
	iret = inet_lnaofp(in);

	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT32_T, iret, 0, FD_API_OTHER, "%u", in.s_addr);
 
	return iret;
}

in_addr_t inet_netof(struct in_addr in)
{
	static in_addr_t (*inet_netofp)(struct in_addr in);
	in_addr_t iret;
 
	BEFORE_ORIGINAL(inet_netof, libc.so.6);
 
	iret = inet_netofp(in);

	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT32_T, iret, 0, FD_API_OTHER, "%u", in.s_addr);
 
	return iret;
}

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
	static const char* (*inet_ntopp)(int af, const void *src, char *dst, socklen_t size);
	const char* cret;
 
	BEFORE_ORIGINAL(inet_ntop, libc.so.6);
 
	cret = inet_ntopp(af, src, dst, size);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, cret, size, FD_API_OTHER,
			"%d, %p, %p, %d", af, src, dst, size);
 
	return cret;
}

int inet_pton(int af, const char *src, void *dst)
{
	static int (*inet_ptonp)(int af, const char *src, void *dst);
 
	BEFORE_ORIGINAL(inet_pton, libc.so.6);
 
	ret = inet_ptonp(af, src, dst);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%d, %p, %p", af, src, dst);
 
	return ret;
}

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
	static int (*getaddrinfop)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
 
	BEFORE_ORIGINAL_NOFILTER(getaddrinfo, libc.so.6);
 
	ret = getaddrinfop(node, service, hints, res);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %p, %p, %p", node, service, hints, res);

	return ret;
}

void freeaddrinfo(struct addrinfo *res)
{
	static void (*freeaddrinfop)(struct addrinfo *res);

	BEFORE_ORIGINAL_NOFILTER(freeaddrinfo, libc.so.6);
 
 	freeaddrinfop(res);

	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%p", res);
}

const char *gai_strerror(int errcode)
{
	static const char * (*gai_strerrorp)(int errcode);
	const char * cret;
 
	BEFORE_ORIGINAL(gai_strerror, libc.so.6);
 
	cret = gai_strerrorp(errcode);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, cret, 0, FD_API_OTHER, "%d", errcode);

	return cret;
}

int gai_suspend(const struct gaicb* const list[], int nitems, const struct timespec *timeout)
{
	static int (*gai_suspendp)(const struct gaicb* const list[], int nitems, const struct timespec *timeout);

	BEFORE_ORIGINAL(gai_suspend, libc.so.6);
 
	ret = gai_suspendp(list,nitems,timeout);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %d, %p", list, nitems, timeout);

	return ret;
}

int gai_error(struct gaicb *req)
{
	static int (*gai_errorp)(struct gaicb *req);
 
	BEFORE_ORIGINAL(gai_error, libc.so.6);
 
	ret = gai_errorp(req);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p", req);
 
	return ret;
}

int gai_cancel(struct gaicb *req)
{
	static int (*gai_cancelp)(struct gaicb *req);
 
	BEFORE_ORIGINAL(gai_cancel, libc.so.6);
 
	ret = gai_cancelp(req);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p", req);
 
	return ret;
}

int getaddrinfo_a(int mode, struct gaicb *list[], int nitems, struct sigevent *sevp)
{
	static int (*getaddrinfo_ap)(int mode, struct gaicb *list[], int nitems, struct sigevent *sevp);
 
	BEFORE_ORIGINAL(getaddrinfo_a, libc.so.6);
 
	ret = getaddrinfo_ap(mode, list,nitems, sevp);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%d, %p, %d, %p", mode, list, nitems, sevp);
 
	return ret;
}
#endif

int getsockopt(int socket, int level, int option_name, void *option_value, socklen_t *option_len)
{
	static int (*getsockoptp)(int socket, int level, int option_name, void *option_value, socklen_t *option_len);
 
	BEFORE_ORIGINAL(getsockopt, libc.so.6);
 
	ret = getsockoptp(socket, level, option_name, option_value, option_len);

	AFTER_ORIGINAL_SOCK(0, socket, FD_API_OPTION, "%d, %d, %d, %p, %p",
			socket, level, option_name, option_value, option_len);
 
	return ret;
}

int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len)
{
	static int (*setsockoptp)(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
 
	BEFORE_ORIGINAL(setsockopt, libc.so.6);

	ret = setsockoptp(socket, level, option_name, option_value, option_len);

	AFTER_ORIGINAL_SOCK(option_len, socket, FD_API_OPTION, "%d, %d, %d, %p, %d",
			socket, level, option_name, option_value, option_len);

	return ret;
}

#if 1
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen )
{
	static int (*getsocknamep)(int sockfd, struct sockaddr *addr, socklen_t *addrlen );
 
	BEFORE_ORIGINAL(getsockname, libc.so.6);
 
	ret = getsocknamep(sockfd, addr, addrlen);

	AFTER_ORIGINAL_SOCK(0, sockfd, FD_API_OTHER, "%d, %p, %p", sockfd, addr, addrlen);

	return ret;
}

int getdomainname(char *name, size_t len)
{
	static int (*getdomainnamep)(char *name, size_t len);
 
	BEFORE_ORIGINAL(getdomainname, libc.so.6);
 
	ret = getdomainnamep(name, len);

	AFTER_ORIGINAL_NOSOCK(len, FD_API_OTHER, "%p, %u", name, len);

	return ret;
}

int setdomainname(const char *name, size_t len)
{
	static int (*setdomainnamep)(const char *name, size_t len);
 
	BEFORE_ORIGINAL(setdomainname, libc.so.6);
 
	ret = setdomainnamep(name, len);

	AFTER_ORIGINAL_NOSOCK(len, FD_API_OTHER, "%p, %u", name, len);
 
	return ret;
}

int gethostname(char *name, size_t len)
{
	static int (*gethostnamep)(char *name, size_t len);
 
	BEFORE_ORIGINAL(gethostname, libc.so.6);
 
	ret = gethostnamep(name, len);

	AFTER_ORIGINAL_NOSOCK(len, FD_API_OTHER, "%p, %u", name, len);
 
	return ret;
}

int sethostname(const char *name, size_t len)
{
	static int (*sethostnamep)(const char *name, size_t len);
 
	BEFORE_ORIGINAL(sethostname, libc.so.6);
 
	ret = sethostnamep(name, len);

	AFTER_ORIGINAL_NOSOCK(len, FD_API_OTHER, "%p, %u", name, len);
 
	return ret;
}

int getpeername(int s, struct sockaddr *addr, socklen_t *len)
{
	static int (*getpeernamep)(int s, struct sockaddr *addr, socklen_t *len);

	BEFORE_ORIGINAL(getpeername, libc.so.6);
 
	ret = getpeernamep(s, addr, len);

	AFTER_ORIGINAL_SOCK(0, s, FD_API_OTHER, "%d, %p, %p", s, addr, len);
 
	return ret;
}

int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, unsigned int flags)
{
	static int (*getnameinfop)(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, unsigned int flags);
 
	BEFORE_ORIGINAL(getnameinfo, libc.so.6);
 
	ret = getnameinfop(sa, salen,host, hostlen, serv, servlen, flags);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %d, %p, %u, %p, %u, %d",
			sa, salen, host, hostlen, serv, servlen, flags);
 
	return ret;
}

struct hostent *gethostbyname(const char *name)
{
	static struct hostent * (*gethostbynamep)(const char *name);
	struct hostent* pret;
 
	BEFORE_ORIGINAL(gethostbyname, libc.so.6);

	pret = gethostbynamep(name);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%p", name);

	return pret;
}

struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type)
{
	static struct hostent * (*gethostbyaddrp)(const void *addr, socklen_t len, int type);
	struct hostent* pret;
 
	BEFORE_ORIGINAL(gethostbyaddr, libc.so.6);
 
	pret = gethostbyaddrp(addr, len, type);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER,
			"%p, %d, %d", addr, len, type);

	return pret;
}

void sethostent(int stayopen)
{
	static void (*sethostentp)(int stayopen);

	BEFORE_ORIGINAL(sethostent, libc.so.6);
 
	sethostentp(stayopen);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%d", stayopen);
}

void endhostent(void)
{
	static void (*endhostentp)(void);

	BEFORE_ORIGINAL(endhostent, libc.so.6);

	endhostentp();
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%s", "");
}

void herror(const char *s)
{
	static void (*herrorp)(const char *s);

	BEFORE_ORIGINAL(herror, libc.so.6);
 
	herrorp(s);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%p", s);
}

const char *hstrerror(int err)
{
	static const char* (*hstrerrorp)(int err);
	const char* cret;
 
	BEFORE_ORIGINAL(hstrerror, libc.so.6);
 
	cret = hstrerrorp(err);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, cret, 0, FD_API_OTHER, "%d", err);

	return cret;
}

struct hostent *gethostent(void)
{
	static struct hostent* (*gethostentp)(void);
	struct hostent* pret;
 
	BEFORE_ORIGINAL(gethostent, libc.so.6);
 
	pret = gethostentp();

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%s", "");

	return pret;
}

struct hostent *gethostbyname2(const char *name, int af)
{
	static struct hostent * (*gethostbyname2p)(const char *name, int af);
	struct hostent* pret;
 
	BEFORE_ORIGINAL(gethostbyname2, libc.so.6);
 
	pret = gethostbyname2p(name, af);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%p, %d", name, af);

	return pret;
}

int gethostent_r(struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop)
{
	static int (*gethostent_rp)(struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(gethostent_r, libc.so.6);
	
	ret = gethostent_rp(rret, buf, buflen, result, h_errnop);
 
	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %p, %u, %p, %p",
			rret, buf, buflen, result, h_errnop);

	return ret;
}

int gethostbyaddr_r(const void *addr, socklen_t len, int type, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop)
{
	static int (*gethostbyaddr_rp)(const void *addr, socklen_t len, int type, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);

	BEFORE_ORIGINAL(gethostbyaddr_r, libc.so.6);

	ret = gethostbyaddr_rp(addr, len, type, rret, buf, buflen, result, h_errnop);
 
	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %d, %d, %p, %p, %u, %p, %p",
			addr, len, type, rret, buf, buflen, result, h_errnop);

	return ret;
}

int gethostbyname_r(const char *name, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop)
{
	static int (*gethostbyname_rp)(const char *name, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(gethostbyname_r, libc.so.6);

	ret = gethostbyname_rp(name, rret, buf, buflen, result, h_errnop);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %p, %p, %u, %p, %p",
			name, rret, buf, buflen, result, h_errnop);

	return ret;
}

int gethostbyname2_r(const char *name, int af, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop)
{
	static int (*gethostbyname2_rp)(const char *name, int af, struct hostent *rret, char *buf, size_t buflen, struct hostent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(gethostbyname2_r, libc.so.6);
 
	ret = gethostbyname2_rp(name, af, rret, buf, buflen, result, h_errnop);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %d, %p, %p, %u, %p, %p",
			name, af, rret, buf, buflen, result, h_errnop);
 
	return ret;
}

struct servent *getservbyname(const char *name, const char *proto)
{
	static struct servent * (*getservbynamep)(const char *name, const char *proto);
	struct servent* pret;
 
	BEFORE_ORIGINAL(getservbyname, libc.so.6);
 
	pret = getservbynamep(name, proto);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%p, %p", name, proto);

	return pret;
}

void setservent(int stayopen)
{
	static void (*setserventp)(int stayopen);

	BEFORE_ORIGINAL(setservent, libc.so.6);
 
	setserventp(stayopen);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%d", stayopen);
}

void endservent(void)
{
	static void (*endserventp)(void);

	BEFORE_ORIGINAL(endservent, libc.so.6);
 
	endserventp();
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%s", "");
}

struct servent *getservent(void)
{
	static struct servent * (*getserventp)(void);
	struct servent* pret;
 
	BEFORE_ORIGINAL(getservent, libc.so.6);
 
	pret = getserventp();
	
	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%s", "");

	return pret;
}

struct servent *getservbyport(int port, const char *proto)
{
	static struct servent * (*getservbyportp)(int port, const char *proto);
	struct servent* pret;
 
	BEFORE_ORIGINAL(getservbyport, libc.so.6);
 
	pret = getservbyportp(port, proto);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%d, %p", port, proto);

	return pret;
}

int getservent_r(struct servent *result_buf, char *buf, size_t buflen, struct servent **result)
{
	static int (*getservent_rp)(struct servent *result_buf, char *buf, size_t buflen, struct servent **result);
 
	BEFORE_ORIGINAL(getservent_r, libc.so.6);
 
	ret = getservent_rp(result_buf, buf, buflen, result);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %p, %u, %p",
			result_buf, buf, buflen, result);

	return ret;
}

int getservbyname_r(const char *name, const char *proto, struct servent *result_buf, char *buf, size_t buflen, struct servent **result)
{
	static int (*getservbyname_rp)(const char *name, const char *proto, struct servent *result_buf, char *buf, size_t buflen, struct servent **result);
 
	BEFORE_ORIGINAL(getservbyname_r, libc.so.6);
 
	ret = getservbyname_rp(name, proto, result_buf, buf, buflen, result);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %p, %p, %p, %u, %p",
			name, proto, result_buf, buf, buflen, result);

	return ret;
}

int getservbyport_r(int port, const char *proto, struct servent *result_buf, char *buf, size_t buflen, struct servent **result)
{
	static int (*getservbyport_rp)(int port, const char *proto, struct servent *result_buf, char *buf, size_t buflen, struct servent **result);
 
	BEFORE_ORIGINAL(getservbyport_r, libc.so.6);
 
	ret = getservbyport_rp(port, proto, result_buf, buf, buflen, result);
	
	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%d, %p, %p, %p, %u, %p",
			port, proto, result_buf, buf, buflen, result);

	return ret;
}

struct netent* getnetent(void)
{
	static struct netent * (*getnetentp)(void);
	struct netent* pret;
 
	BEFORE_ORIGINAL(getnetent, libc.so.6);
 
	pret = getnetentp();

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%s", "");

	return pret;
}

struct netent *getnetbyname(const char *name)
{
	static struct netent * (*getnetbynamep)(const char *name);
	struct netent* pret;
 
	BEFORE_ORIGINAL(getnetbyname, libc.so.6);
 
	pret = getnetbynamep(name);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%p", name);
 
	return pret;
}

struct netent *getnetbyaddr(uint32_t net, int type)
{
	static struct netent * (*getnetbyaddrp)(uint32_t net, int type);
	struct netent * pret;
 
	BEFORE_ORIGINAL(getnetbyaddr, libc.so.6);

	pret = getnetbyaddrp(net, type);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%u, %d", net, type);

	return pret;
}

void setnetent(int stayopen)
{
	static void (*setnetentp)(int stayopen);

	BEFORE_ORIGINAL(setnetent, libc.so.6);
 
	setnetentp(stayopen);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%d", stayopen);
}

void endnetent(void)
{
	static void (*endnetentp)(void);

	BEFORE_ORIGINAL(endnetent, libc.so.6);
 
	endnetentp();
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%s", "");
}

int getnetent_r(struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop)
{
	static int (*getnetent_rp)(struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(getnetent_r, libc.so.6);
 
	ret = getnetent_rp(result_buf, buf, buflen, result, h_errnop);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %p, %u, %p, %p",
			result_buf, buf, buflen, result, h_errnop);

	return ret;
}

int getnetbyname_r(const char *name, struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop)
{
	static int (*getnetbyname_rp)(const char *name, struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(getnetbyname_r, libc.so.6);
 
	ret = getnetbyname_rp(name,result_buf, buf, buflen, result, h_errnop);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %p, %p, %u, %p, %p",
			name, result_buf, buf, buflen, result, h_errnop);

	return ret;
}

int getnetbyaddr_r(uint32_t net, int type, struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop)
{
	static int (*getnetbyaddr_rp)(uint32_t net, int type, struct netent *result_buf, char *buf, size_t buflen, struct netent **result, int *h_errnop);
 
	BEFORE_ORIGINAL(getnetbyaddr_r, libc.so.6);
 
	ret = getnetbyaddr_rp(net, type, result_buf, buf, buflen, result, h_errnop);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%u, %d, %p, %p, %u, %p, %p",
			net, type, result_buf, buf, buflen, result, h_errnop);

	return ret;
}

struct protoent *getprotoent(void)
{
	static struct protoent * (*getprotoentp)(void);
	struct protoent * pret;
 
	BEFORE_ORIGINAL(getprotoent, libc.so.6);
 
	pret = getprotoentp();

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%s", "");
 
	return pret;
}

struct protoent *getprotobyname(const char *name)
{
	static struct protoent * (*getprotobynamep)(const char *name);
	struct protoent * pret;
 
	BEFORE_ORIGINAL(getprotobyname, libc.so.6);
 
	pret = getprotobynamep(name);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%p", name);

	return pret;
}

struct protoent *getprotobynumber(int proto)
{
	static struct protoent * (*getprotobynumberp)(int proto);
	struct protoent * pret;
 
	BEFORE_ORIGINAL(getprotobynumber, libc.so.6);
 
	pret = getprotobynumberp(proto);

	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%d", proto);

	return pret;
}

void setprotoent(int stayopen)
{
	static void (*setprotoentp)(int stayopen);

	BEFORE_ORIGINAL(setprotoent, libc.so.6);
 
	setprotoentp(stayopen);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%d", stayopen);
}

void endprotoent(void)
{
	static void (*endprotoentp)(void);

	BEFORE_ORIGINAL(endprotoent, libc.so.6);
 
	endprotoentp();
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%s", "");
}

int getprotoent_r(struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result)
{
	static int (*getprotoent_rp)(struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result);
 
	BEFORE_ORIGINAL(getprotoent_r, libc.so.6);
 
	ret = getprotoent_rp(result_buf, buf, buflen, result);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %p, %u, %p",
			result_buf, buf, buflen, result);

	return ret;
}

int getprotobyname_r(const char *name, struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result)
{
	static int (*getprotobyname_rp)(const char *name, struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result);
 
	BEFORE_ORIGINAL(getprotobyname_r, libc.so.6);
 
	ret = getprotobyname_rp(name, result_buf, buf, buflen, result);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %p, %p, %u, %p",
			name, result_buf, buf, buflen, result);

	return ret;
}

int getprotobynumber_r(int proto, struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result)
{
	static int (*getprotobynumber_rp)(int proto, struct protoent *result_buf, char *buf, size_t buflen, struct protoent **result);
 
	BEFORE_ORIGINAL(getprotobynumber_r, libc.so.6);
 
	ret = getprotobynumber_rp(proto, result_buf, buf, buflen, result);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%d, %p, %p, %u, %p",
			proto, result_buf, buf, buflen, result);

	return ret;
}

unsigned int if_nametoindex (__const char *__ifname)
{
	static unsigned int (*if_nametoindexp)(__const char *__ifname);
	unsigned int uret;
 
	BEFORE_ORIGINAL(if_nametoindex, libc.so.6);
 
	uret = if_nametoindexp(__ifname);

	AFTER_ORIGINAL_NOSOCK_RET(VT_UINT, uret, 0, FD_API_OTHER, "%p", __ifname);
 
	return uret;
}

char *if_indextoname (unsigned int __ifindex, char *__ifname)
{
	static char * (*if_indextonamep)(unsigned int __ifindex, char *__ifname);
	char * cret;
	
	BEFORE_ORIGINAL(if_indextoname, libc.so.6);
 
	cret = if_indextonamep(__ifindex, __ifname);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, cret, 0, FD_API_OTHER,
			"%u, %p", __ifindex, __ifname);

	return cret;
}

struct if_nameindex *if_nameindex (void)
{
	static struct if_nameindex * (*if_nameindexp)(void);
	struct if_nameindex * pret;
 
	BEFORE_ORIGINAL_NOFILTER(if_nameindex, libc.so.6);
	
	pret = if_nameindexp();
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_PTR, pret, 0, FD_API_OTHER, "%s", "");

	return pret;
}

void if_freenameindex (struct if_nameindex *__ptr)
{
	static void (*if_freenameindexp)(struct if_nameindex *__ptr);

	BEFORE_ORIGINAL_NOFILTER(if_freenameindex, libc.so.6);
 
	if_freenameindexp(__ptr);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%p", __ptr);
}

int getifaddrs(struct ifaddrs **ifap)
{
	static int (*getifaddrsp)(struct ifaddrs **ifap);
 
	BEFORE_ORIGINAL_NOFILTER(getifaddrs, libc.so.6);
 
	ret = getifaddrsp(ifap);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p", ifap);
 
	return ret;
}

void freeifaddrs(struct ifaddrs *ifa)
{
	static void (*freeifaddrsp)(struct ifaddrs *ifa);

	BEFORE_ORIGINAL_NOFILTER(freeifaddrs, libc.so.6);
 
	freeifaddrsp(ifa);
 
	AFTER_ORIGINAL_NOSOCK_RET(VT_NULL, NULL, 0, FD_API_OTHER, "%p", ifa);
}

#if 0
int poll(struct pollfd *ufds, unsigned int nfds, int timeout)
{
	static int (*pollp)(struct pollfd *ufds, unsigned int nfds, int timeout);
 
	BEFORE_ORIGINAL(poll, libc.so.6);
 
	ret = pollp(ufds, nfds, timeout);

	AFTER_ORIGINAL_NOSOCK(timeout, FD_API_OTHER, "%p, %u, %d", ufds, nfds, timeout);

	return ret;
}

int ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts, const sigset_t *sigmask)
{
	static int (*ppollp)(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts, const sigset_t *sigmask);
 
	BEFORE_ORIGINAL(ppoll, libc.so.6);
 
	ret = ppollp(fds, nfds, timeout_ts, sigmask);

	AFTER_ORIGINAL_NOSOCK(0, FD_API_OTHER, "%p, %lu, %p, %p",
			fds, nfds, timeout_ts, sigmask);

	return ret;
}
#endif

#endif
