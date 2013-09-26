/*
 *  DA probe
 *
 * Copyright (File::*c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Hyunjong Park        <phjwithyou.park@samsung.com>
 * Juyoung Kim          <j0.kim@samsung.com>
 * Vitaliy Cherepanov   <v.cherepanov@samsung.com>
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

#include <FBase.h>
#include <FBaseObject.h>
#include <FBaseResult.h>
#include <arpa/inet.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dahelper.h"
#include "dacollection.h"
#include "tizen_probe.h"
#include "../probe_socket/da_socket.h"
#include "binproto.h"
#include "api_id_mapping.h"

static enum DaOptions _sopt = OPT_NETWORK;

namespace Tizen {
namespace Net {

typedef int NetAccountId;

enum NetAddressFamily {

	NET_AF_IPV4 = 1, /**< The default @n An IPv4 address [RFC791] is represented in decimal format with dots as delimiters. */
	NET_AF_IPV6, /**< An IPv6 address [RFC2373] is generally represented in hexadecimal numbers with colons as delimiters @n It can also be a combination of hexadecimal and decimal numbers with dots and colons as delimiters. */
	NET_AF_ALPHANUMERIC, /**< A generic alphanumeric address (as defined by alphanum in [RFC2396]) */
	NET_AF_APN, /**< A GPRS APN as defined in [GENFORM] */
	NET_AF_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application. Network Address Family not set
};

enum NetConnectionState {
	NET_CONNECTION_STATE_NONE, /**< The initial state */
	NET_CONNECTION_STATE_STARTING, /**< The network connection is being established */
	NET_CONNECTION_STATE_STARTED, /**< The network connection has been established */
	NET_CONNECTION_STATE_STOPPING, /**< The network connection is being disconnected */
	NET_CONNECTION_STATE_STOPPED, /**< The network connection has been disconnected */
	NET_CONNECTION_STATE_SUSPENDED, /**< The network connection has been suspended */
	NET_CONNECTION_STATE_SUSPEND = 5, // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
	NET_CONNECTION_STATE_RESUMED, /**< The network connection is established, and recovered from SUSPEND state - enable to send/receive packet data */
	NET_CONNECTION_STATE_AVAILABLE, // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
	NET_CONNECTION_STATE_UNAVAILABLE, // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
};
enum NetBearerType {
	NET_BEARER_PS = 1, /**< The bearer type for the PS domain */
	NET_BEARER_CS, // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
	NET_BEARER_WIFI, /**< The bearer type for Wi-Fi */
	NET_BEARER_WIFI_DIRECT, /**< The bearer type for Wi-Fi Direct @n
	 *	This bearer type cannot be tested on the Emulator. */
	NET_BEARER_USB, /**< The bearer type for USB NDIS mode @n
	 *	Can use this bearer type via USB Tethering mode. @n
	 *	This bearer type cannot be tested on the Emulator. */
	NET_BEARER_MMS, /**< The bearer type for MMS */
	NET_BEARER_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application. Network Bearer Type not set
};

enum NetAddressScheme {
	NET_ADDRESS_SCHEME_DYNAMIC = 1, /**< The Dynamic IP or DNS Address */
	NET_ADDRESS_SCHEME_STATIC, /**< The Static IP or DNS Address */
	NET_ADDRESS_SCHEME_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application. Network Address Scheme not set
};

enum NetProtocolType {
	NET_PROTO_TYPE_IPV4 = 1, /**< The IPV4 type */
	NET_PROTO_TYPE_PPP, /**< The PPP type */
	NET_PROTO_TYPE_IPV6, /**< The IPV6 type, currently NOT supported */
	NET_PROTO_TYPE_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application. Network Protocol type not set
};

class _IpAddressImpl;
class IpAddress {
public:
	virtual ~IpAddress(void);
	static unsigned short HostToNetworkOrder(unsigned short host);
	static unsigned long HostToNetworkOrder(unsigned long host);
	static unsigned short NetworkToHostOrder(unsigned short network);
	static unsigned long NetworkToHostOrder(unsigned long network);
	virtual NetAddressFamily GetNetAddressFamily(void) const = 0;
	virtual result GetAddress(Tizen::Base::ByteBuffer& ipAddr) const = 0;
	virtual Tizen::Base::String ToString(void) const = 0;
	virtual IpAddress* CloneN(void) const = 0;
protected:
	IpAddress(void);
protected:
	virtual void IpAddress_Reserved1(void) {
	}
	virtual void IpAddress_Reserved2(void) {
	}
	virtual void IpAddress_Reserved3(void) {
	}
	virtual void IpAddress_Reserved4(void) {
	}
	virtual void IpAddress_Reserved5(void) {
	}
private:
	IpAddress(const IpAddress& rhs);
	IpAddress& operator =(const IpAddress& rhs);
protected:
	_IpAddressImpl* _pIpAddressImpl;
	friend class _IpAddressImpl;
};
// IpAddress

class Ip4Address {
public:
	Tizen::Base::String ToString(void) const;
	//unsigned long GetAddress(unsigned long& ipAddr) const;
	unsigned long GetAddress(uint32_t *ipAddr) const;
};

Tizen::Base::String Ip4Address::ToString(void) const {
	typedef Tizen::Base::String (Ip4Address::*methodType)(void) const;
	static methodType ToStringp;
	CALL_ORIGINAL_TIZEN_NET( _ZNK5Tizen3Net10Ip4Address8ToStringEv, ToStringp);
	return (this->*ToStringp)();
}

/* INFO this is not original Ip4Address function */
unsigned long Ip4Address::GetAddress( uint32_t *ipAddr) const {
	unsigned long IP, res;
	typedef unsigned long (Ip4Address::*methodType)(unsigned long& ipAddr) const;
	static methodType GetAddresgp;
	CALL_ORIGINAL_TIZEN_NET(_ZNK5Tizen3Net10Ip4Address10GetAddressERm,
				GetAddresgp);
	res = (this->*GetAddresgp)(IP);
	*ipAddr = ntohl(IP);
	return res;
}

class NetEndPoint {
public:
	unsigned short GetPort(void) const;
	IpAddress* GetAddress(void) const;
};

unsigned short NetEndPoint::GetPort(void) const {
	typedef unsigned short (NetEndPoint::*methodType)(void) const;
	static methodType GetPortp;
	CALL_ORIGINAL_TIZEN_NET( _ZNK5Tizen3Net11NetEndPoint7GetPortEv, GetPortp);
	return (this->*GetPortp)();
}

IpAddress* NetEndPoint::GetAddress(void) const {
	typedef IpAddress *(NetEndPoint::*methodType)(void) const;
	static methodType GetAddressp;
	CALL_ORIGINAL_TIZEN_NET( _ZNK5Tizen3Net11NetEndPoint10GetAddressEv,
			GetAddressp);
	return (this->*GetAddressp)();
}

class NetConnectionInfo;
class INetConnectionEventListener;

class _NetConnectionImpl;
class NetConnection {
public:
	NetConnection(void);
	virtual ~NetConnection(void);
	result Construct(NetAccountId netAccountId);
public:
	result AddNetConnectionListener(INetConnectionEventListener& listener);
	result RemoveNetConnectionListener(INetConnectionEventListener& listener);
	result Start(void);
	result Stop(void);
	result Close(void);
	NetAccountId GetNetAccountId(void) const;
	const NetConnectionInfo* GetNetConnectionInfo(void) const;
	static NetConnectionInfo* GetNetConnectionInfoN(NetAccountId netAccountId);
	static Tizen::Base::Collection::IList* GetAllNetConnectionInfoN(void);
	NetConnectionState GetConnectionState(void) const;
private:
	NetConnection(const NetConnection& rhs);
	NetConnection& operator =(const NetConnection& rhs);
private:
	_NetConnectionImpl* __pNetConnectionImpl;
	friend class _NetConnectionImpl;
};

class INetConnectionEventListener {
public:
	virtual ~INetConnectionEventListener(void) {
	}
public:
	virtual void OnNetConnectionStarted(NetConnection& netConnection,
			result r) = 0;
	virtual void OnNetConnectionStopped(NetConnection& netConnection,
			result r) = 0;
	virtual void OnNetConnectionSuspended(NetConnection& netConnection) = 0;
	virtual void OnNetConnectionResumed(NetConnection& netConnection) = 0;
protected:
	virtual void INetConnectionEventListener_Reserved1(void) {
	}
	virtual void INetConnectionEventListener_Reserved2(void) {
	}
};

class _NetConnectionInfoImpl;
class NetConnectionInfo {
public:
	NetConnectionInfo(void);
	virtual ~NetConnectionInfo(void);
	NetBearerType GetBearerType(void) const;
	NetProtocolType GetProtocolType(void) const;
	Tizen::Base::String GetAccessPointName(void) const;
	NetAddressScheme GetLocalAddressScheme(void) const;
	const IpAddress* GetLocalAddress(void) const;
	NetAddressScheme GetDnsAddressScheme(void) const;
	const IpAddress* GetPrimaryDnsAddress(void) const;
	const IpAddress* GetSecondaryDnsAddress(void) const;
	const IpAddress* GetSubnetMaskAddress(void) const;
	const IpAddress* GetDefaultGatewayAddress(void) const;
	NetConnectionInfo(const NetConnectionInfo& rhs);
	NetConnectionInfo& operator =(const NetConnectionInfo& rhs);
	virtual bool Equals(const Tizen::Base::Object& rhs) const;
	virtual int GetHashCode(void) const;
private:
	_NetConnectionInfoImpl* __pNetConnectionInfoImpl;
	friend class _NetConnectionInfoImpl;
};

}
} //namespace Tizen { namespace Net {

namespace Tizen {
namespace Net {
namespace Sockets {

enum NetSocketAddressFamily {
	//NET_SOCKET_AF_LOCAL = 1,                            /*< The local socket for loopback*/
	NET_SOCKET_AF_IPV4 = 2, /**< An IP version 4 address family */
	//NET_SOCKET_AF_IPV6,						/*< An IP version 6 address family */
	NET_SOCKET_AF_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
};
//
enum NetSocketType {
	NET_SOCKET_TYPE_STREAM = 1, /**< The stream socket */
	NET_SOCKET_TYPE_DATAGRAM, /**< The datagram socket */
	NET_SOCKET_TYPE_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
};
//
enum NetSocketProtocol {
	NET_SOCKET_PROTOCOL_TCP = 1, /**< The TCP protocol */
	NET_SOCKET_PROTOCOL_UDP, /**< The UDP protocol */
	NET_SOCKET_PROTOCOL_SSL, /**< The SSL protocol */
	NET_SOCKET_PROTOCOL_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
};

enum NetSocketIoctlCmd {
	NET_SOCKET_FIONBIO = 1, /**< The command to set the non-blocking/blocking mode on a socket */
	NET_SOCKET_FIONREAD, /**< The command to determine the amount of data pending in the network input buffer */
	NET_SOCKET_FIONWRITE, /**< The functionality is not supported in this SDK */
	NET_SOCKET_SIOCATMARK, /**< The command to determine whether all out-of-band data is read */
	NET_SOCKET_IOCTLCMD_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
};

enum NetSocketOptLevel {
	NET_SOCKET_IPPROTO_TCP = 1, /**< The socket options apply only to the TCP sockets */
	NET_SOCKET_IPPROTO_IP, /**< The socket options apply only to the IP sockets */
	NET_SOCKET_SOL_SOCKET, /**< The socket options apply to all the sockets */
	NET_SOCKET_OPTLEVEL_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
};
//
enum NetSocketOptName {
	NET_SOCKET_TCP_NODELAY = 1, /**< The option name is Level: NET_SOCKET_IPPROTO_TCP - Set/Get is possible */
	NET_SOCKET_TCP_MAXSEG, /**< The option name is Level: NET_SOCKET_IPPROTO_TCP - Set/Get is possible */
	NET_SOCKET_IP_TTL, /**< The option name is Level: NET_SOCKET_IPPROTO_IP - Set/Get is possible */
	NET_SOCKET_IP_TOS, /**< The option name is Level: NET_SOCKET_IPPROTO_IP - Set/Get is possible */
	NET_SOCKET_IP_ADD_MEMBERSHIP, /**< The option name is Level: NET_SOCKET_IPPROTO_IP - Only Set is possible */
	NET_SOCKET_IP_DROP_MEMBERSHIP, /**< The option name is Level: NET_SOCKET_IPPROTO_IP - Only Set is possible */
	NET_SOCKET_SO_ACCEPTCONN, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Only Get is possible */
	NET_SOCKET_SO_BROADCAST, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_DEBUG, // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application. The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible (Currently, not supported)
	NET_SOCKET_SO_DONTROUTE, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_ERROR, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Only Get is possible */
	NET_SOCKET_SO_KEEPALIVE, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_LINGER, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_OOBINLINE, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_RCVBUF, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_RCVLOWAT, // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application. The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible (Currently, not supported)
	NET_SOCKET_SO_RCVTIMEO, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_REUSEADDR, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_SNDBUF, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_SNDLOWAT, // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application. The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible (Currently, not supported)
	NET_SOCKET_SO_SNDTIMEO, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_TYPE, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Only Get is possible */

	//Secure Socket only
	NET_SOCKET_SO_SSLVERSION, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_SSLCIPHERSUITEID, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Set/Get is possible */
	NET_SOCKET_SO_SSLCERTVERIFY, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Only Set is possible */
	NET_SOCKET_SO_SSLCERTID, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Only Set is possible */
	NET_SOCKET_SO_SSLCLIENTCERTVERIFY, /**< The option name is Level: NET_SOCKET_SOL_SOCKET - Only Set is possible */
	NET_SOCKET_OPTNAME_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
};

enum NetSocketClosedReason {
	NET_SOCKET_CLOSED_REASON_NORMAL = 1, /**< A normal closed status by peer */
	NET_SOCKET_CLOSED_REASON_TIMEOUT, /**< The connection attempt failed due to timeout */
	NET_SOCKET_CLOSED_REASON_NETWORK_UNAVAILABLE, /**< The network is unavailable */
	NET_SOCKET_CLOSED_REASON_SYSTEM, /**< An internal error has occurred */
	NET_SOCKET_CLOSED_REASON_NO_CERTIFICATE, /**< The reason is there is no client's SSL certificate */
	NET_SOCKET_CLOSED_REASON_BY_USER, /**< The connection closed by user */
	NET_SOCKET_CLOSED_REASON_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application.
};

class Socket;

class _LingerOptionImpl;
class LingerOption {
public:
	LingerOption(bool enable, int seconds);
	virtual ~LingerOption(void);
	LingerOption(const LingerOption& rhs);
	LingerOption& operator =(const LingerOption& rhs);
	virtual bool Equals(const Tizen::Base::Object& obj) const;
	virtual int GetHashCode(void) const;
public:
	void SetEnabled(bool on);
	void SetLingerTime(int seconds);
	int GetLingerTime(void) const;
	bool IsEnabled(void) const;
private:
	LingerOption(void);
private:
	_LingerOptionImpl* __pLingerOptionImpl;
	friend class _LingerOptionImpl;
};

class _IpMulticastRequestOptionImpl;
class IpMulticastRequestOption {
public:
	IpMulticastRequestOption(const NetEndPoint& multicastAddress,
			const NetEndPoint& interfaceAddress);
	virtual ~IpMulticastRequestOption(void);
	IpMulticastRequestOption(const IpMulticastRequestOption& rhs);
	IpMulticastRequestOption& operator =(const IpMulticastRequestOption& rhs);
public:
	result SetMulticastEndPoint(NetEndPoint& multicastAddress);
	result SetInterfaceEndPoint(NetEndPoint& interfaceAddress);
	const NetEndPoint* GetMulticastEndPoint(void) const;
	const NetEndPoint* GetInterfaceEndPoint(void) const;
	virtual bool Equals(const Tizen::Base::Object& obj) const;
	virtual int GetHashCode(void) const;
private:
	IpMulticastRequestOption(void);
private:
	_IpMulticastRequestOptionImpl* __pIpMulticastRequestOptionImpl;
	friend class _IpMulticastRequestOptionImpl;
};

class ISocketEventListener {
public:
	virtual ~ISocketEventListener(void) {
	}
public:
	virtual void OnSocketConnected(Socket& socket) = 0;
	virtual void OnSocketClosed(Socket& socket,
			NetSocketClosedReason reason) = 0;
	virtual void OnSocketReadyToReceive(Socket& socket) = 0;
	virtual void OnSocketReadyToSend(Socket& socket) = 0;
	virtual void OnSocketAccept(Socket& socket) = 0;
protected:
	virtual void ISocketEventListener_Reserved1(void) {
	}
	virtual void ISocketEventListener_Reserved2(void) {
	}
};

class Socket {
public:
	result Construct(NetSocketAddressFamily addressFamily,
			NetSocketType socketType, NetSocketProtocol protocol);
	result Construct(const NetConnection& netConnection,
			NetSocketAddressFamily addressFamily, NetSocketType socketType,
			NetSocketProtocol protocol);
	result Bind(const Tizen::Net::NetEndPoint& localEndPoint);
	result Listen(int backLog);
	result Connect(const Tizen::Net::NetEndPoint& remoteEndPoint);
	Socket* AcceptN(void) const;
	result Receive(Tizen::Base::ByteBuffer& buffer) const;
	result Receive(void* pBuffer, int length, int& rcvdLength) const;
	result ReceiveFrom(Tizen::Base::ByteBuffer& buffer,
			Tizen::Net::NetEndPoint& remoteEndPoint) const;
	result ReceiveFrom(void* pBuffer, int length,
			Tizen::Net::NetEndPoint& remoteEndPoint, int& rcvdLength) const;
	result Send(Tizen::Base::ByteBuffer& buffer);
	result Send(void* pBuffer, int length, int& sentLength);
	result SendTo(Tizen::Base::ByteBuffer& buffer,
			const Tizen::Net::NetEndPoint& remoteEndPoint);
	result SendTo(void* pBuffer, int length,
			const Tizen::Net::NetEndPoint& remoteEndPoint, int& sentLength);
	result Close(void);

	result SetSockOpt(NetSocketOptLevel optionLevel,
			NetSocketOptName optionName, int optionValue);
	result SetSockOpt(NetSocketOptLevel optionLevel,
			NetSocketOptName optionName, const LingerOption& optionValue);
	result SetSockOpt(NetSocketOptLevel optionLevel,
			NetSocketOptName optionName,
			const IpMulticastRequestOption& optionValue);
	result Ioctl(NetSocketIoctlCmd cmd, unsigned long& value) const;
	result AddSocketListener(ISocketEventListener& listener);
	result AsyncSelectByListener(unsigned long socketEventType);
	result RemoveSocketListener(ISocketEventListener& listener);
	result GetSockOpt(NetSocketOptLevel optionLevel,
			NetSocketOptName optionName, int& optionValue) const;
	result GetSockOpt(NetSocketOptLevel optionLevel,
			NetSocketOptName optionName, LingerOption& optionValue) const;
};

result Socket::Construct(NetSocketAddressFamily addressFamily,
		NetSocketType socketType, NetSocketProtocol protocol) {
	typedef result (Socket::*methodType)(NetSocketAddressFamily addressFamily,
			NetSocketType socketType, NetSocketProtocol protocol);
	static methodType Constructp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket9ConstructENS1_22NetSocketAddressFamilyENS1_13NetSocketTypeENS1_17NetSocketProtocolE,
			Constructp);

	retVal = (this->*Constructp)(addressFamily, socketType, protocol);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::Construct", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_FD_OPEN, "", 0,
			hostinfo, "ddd", addressFamily, socketType, protocol);

	return retVal;
}

result Socket::Construct(const NetConnection& netConnection,
		NetSocketAddressFamily addressFamily, NetSocketType socketType,
		NetSocketProtocol protocol) {
	typedef result (Socket::*methodType)(const NetConnection& netConnection,
			NetSocketAddressFamily addressFamily, NetSocketType socketType,
			NetSocketProtocol protocol);
	static methodType Constructp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket9ConstructERKNS0_13NetConnectionENS1_22NetSocketAddressFamilyENS1_13NetSocketTypeENS1_17NetSocketProtocolE,
			Constructp);

	retVal = (this->*Constructp)(netConnection, addressFamily, socketType,
			protocol);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::Construct", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_FD_OPEN, "", 0,
			hostinfo, "xddd",
			(unsigned int)&netConnection, addressFamily, socketType, protocol);

	return retVal;
}

Socket* Socket::AcceptN(void) const {
	typedef Socket* (Socket::*methodType)(void) const;
	static methodType AcceptNp;
	Socket* pret;

	BEFORE_ORIGINAL_TIZEN_NET( _ZNK5Tizen3Net7Sockets6Socket7AcceptNEv,
			AcceptNp);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_START("Socket::AcceptN", VT_NULL, NULL,
			(unsigned int)this, (unsigned int)this, SOCKET_API_ACCEPT_START, "",
			0, hostinfo, "s", "void");

	pret = (this->*AcceptNp)();
	if (pret == NULL)
		newerrno = 1;
	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_END("Socket::AcceptN", VT_PTR, pret,
			(unsigned int)this, (unsigned int)this, SOCKET_API_ACCEPT_END, "",
			0, hostinfo, "s", "void");

	return pret;
}

result Socket::Connect(const Tizen::Net::NetEndPoint& remoteEndPoint) {
	typedef result (Socket::*methodType)(
			const Tizen::Net::NetEndPoint& remoteEndPoint);
	static methodType Connectp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket7ConnectERKNS0_11NetEndPointE,
			Connectp);

	Ip4Address* iv4PeerAddr = (Ip4Address*) remoteEndPoint.GetAddress();

	char temp[64];
	WcharToChar(temp, iv4PeerAddr->ToString().GetPointer());
	char addressInfo[64];
	sprintf(addressInfo, "%s:%d", temp, remoteEndPoint.GetPort());
	hostinfo.port = remoteEndPoint.GetPort();
	iv4PeerAddr->GetAddress(&hostinfo.ip);
	retVal = (this->*Connectp)(remoteEndPoint);


	AFTER_ORIGINAL_TIZEN_SOCK("Socket::Connect", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_CONNECT, "", 0,
			hostinfo, "s", addressInfo);

	return retVal;
}

result Socket::Close(void) {
	typedef result (Socket::*methodType)(void);
	static methodType Closep;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZN5Tizen3Net7Sockets6Socket5CloseEv, Closep);

	retVal = (this->*Closep)();

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::Close", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_FD_CLOSE, "", 0,
			hostinfo, "s", "void");

	return retVal;
}

result Socket::Bind(const Tizen::Net::NetEndPoint& localEndPoint) {
	typedef result (Socket::*methodType)(
			const Tizen::Net::NetEndPoint& localEndPoint);
	static methodType Bindp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket4BindERKNS0_11NetEndPointE, Bindp);

	retVal = (this->*Bindp)(localEndPoint);

	Ip4Address* iv4PeerAddr = (Ip4Address*) localEndPoint.GetAddress();
	char temp[64];
	WcharToChar(temp, iv4PeerAddr->ToString().GetPointer());
	char addressInfo[64];
	sprintf(addressInfo, "%s:%d", temp, localEndPoint.GetPort());
	hostinfo.port = localEndPoint.GetPort();
	iv4PeerAddr->GetAddress(&hostinfo.ip);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::Bind", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_BIND, "", 0,
			hostinfo, "s", addressInfo);
	return retVal;
}

result Socket::Listen(int backLog) {
	typedef result (Socket::*methodType)(int backLog);
	static methodType Listenp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZN5Tizen3Net7Sockets6Socket6ListenEi, Listenp);

	retVal = (this->*Listenp)(backLog);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::Listen", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_LISTEN, "", 0,
			hostinfo, "d", backLog);

	return retVal;
}

result Socket::Receive(Tizen::Base::ByteBuffer& buffer) const {
	typedef result (Socket::*methodType)(Tizen::Base::ByteBuffer& buffer) const;
	static methodType Receivep;
	result pret;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net7Sockets6Socket7ReceiveERNS_4Base10ByteBufferE,
			Receivep);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_START("Socket::Receive", VT_NULL, NULL,
			(unsigned int)this, (unsigned int)this, SOCKET_API_RECV_START, "",
			0, hostinfo, "x", (unsigned int)&buffer);

	pret = (this->*Receivep)(buffer);

	int bufferSize = buffer.GetLimit();
	char* pBuffer = (char*) (buffer.GetPointer());
	Tizen::Base::String strData;
	strData.SetCapacity(bufferSize);
	strData.Append(pBuffer);
	char* out = new char[bufferSize];
	WcharToChar(out, strData.GetPointer());
	int socketSendSize = SOCKET_SEND_SIZE;
	if (socketSendSize != 0 && socketSendSize < bufferSize) {
		out[socketSendSize + 1] = '.';
		out[socketSendSize + 2] = '.';
		out[socketSendSize + 3] = '.';
		out[socketSendSize + 4] = '\0';
	}

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_END("Socket::Receive", VT_ULONG, pret,
			(unsigned int)this, (unsigned int)this, SOCKET_API_RECV_END, out,
			buffer.GetLimit(), hostinfo, "x", (unsigned int)&buffer);
	delete out;
	return pret;
}

result Socket::Receive(void* pBuffer, int length, int& rcvdLength) const {
	typedef result (Socket::*methodType)(void* pBuffer, int length,
			int& rcvdLength) const;
	static methodType Receivep;
	result pret;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net7Sockets6Socket11ReceiveFromEPviRNS0_11NetEndPointERi,
			Receivep);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_START("Socket::Receive", VT_NULL, NULL,
			(unsigned int)this, (unsigned int)this, SOCKET_API_RECV_START, "",
			0, hostinfo, "xdx",
			(unsigned int)&pBuffer, length, rcvdLength);

	pret = (this->*Receivep)(pBuffer, length, rcvdLength);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_END("Socket::Receive", VT_ULONG, pret,
			(unsigned int)this, (unsigned int)this, SOCKET_API_RECV_END,
			(char*)pBuffer, length, hostinfo, "xdx",
			(unsigned int)&pBuffer, length, rcvdLength);

	return pret;
}

result Socket::ReceiveFrom(Tizen::Base::ByteBuffer& buffer,
		Tizen::Net::NetEndPoint& remoteEndPoint) const {
	typedef result (Socket::*methodType)(Tizen::Base::ByteBuffer& buffer,
			Tizen::Net::NetEndPoint& remoteEndPoint) const;
	static methodType ReceiveFromp;
	result pret;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net7Sockets6Socket11ReceiveFromERNS_4Base10ByteBufferERNS0_11NetEndPointE,
			ReceiveFromp);

	Ip4Address* iv4PeerAddr = (Ip4Address*) remoteEndPoint.GetAddress();
	char temp[64];
	WcharToChar(temp, iv4PeerAddr->ToString().GetPointer());
	char addressInfo[64];
	sprintf(addressInfo, "%s:%d", temp, remoteEndPoint.GetPort());
	hostinfo.port = remoteEndPoint.GetPort();
	iv4PeerAddr->GetAddress(&hostinfo.ip);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_START("Socket::ReceiveFrom", VT_NULL,
			NULL, (unsigned int)this, (unsigned int)this, SOCKET_API_RECV_START,
			"", 0,
			hostinfo, "xs", (unsigned int)&buffer, temp);

	pret = (this->*ReceiveFromp)(buffer, remoteEndPoint);

	int bufferSize = buffer.GetLimit();
	char* pBuffer = (char*) (buffer.GetPointer());
	Tizen::Base::String strData;
	strData.SetCapacity(bufferSize);
	strData.Append(pBuffer);
	char* out = new char[bufferSize];
	WcharToChar(out, strData.GetPointer());
	int socketSendSize = SOCKET_SEND_SIZE;
	if (socketSendSize != 0 && socketSendSize < bufferSize) {
		out[socketSendSize + 1] = '.';
		out[socketSendSize + 2] = '.';
		out[socketSendSize + 3] = '.';
		out[socketSendSize + 4] = '\0';
	}

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_END("Socket::ReceiveFrom", VT_ULONG,
			pret, (unsigned int)this, (unsigned int)this, SOCKET_API_RECV_END,
			out, buffer.GetLimit(), hostinfo, "xs",
			(unsigned int)&buffer, temp);
	delete out;
	return pret;
}
result Socket::ReceiveFrom(void* pBuffer, int length,
		Tizen::Net::NetEndPoint& remoteEndPoint, int& rcvdLength) const {
	typedef result (Socket::*methodType)(void* pBuffer, int length,
			Tizen::Net::NetEndPoint& remoteEndPoint, int& rcvdLength) const;
	static methodType ReceiveFromp;
	result pret;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net7Sockets6Socket11ReceiveFromEPviRNS0_11NetEndPointERi,
			ReceiveFromp);

	Ip4Address* iv4PeerAddr = (Ip4Address*) remoteEndPoint.GetAddress();
	char temp[64];
	WcharToChar(temp, iv4PeerAddr->ToString().GetPointer());
	char addressInfo[64];
	sprintf(addressInfo, "%s:%d", temp, remoteEndPoint.GetPort());
	hostinfo.port = remoteEndPoint.GetPort();
	iv4PeerAddr->GetAddress(&hostinfo.ip);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_START("Socket::ReceiveFrom", VT_NULL,
			NULL, (unsigned int)this, (unsigned int)this, SOCKET_API_RECV_START,
			"", 0, hostinfo, "xdsx",
			(unsigned int)&pBuffer, length, temp, rcvdLength);

	pret = (this->*ReceiveFromp)(pBuffer, length, remoteEndPoint, rcvdLength);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_END("Socket::ReceiveFrom", VT_ULONG,
			pret, (unsigned int)this, (unsigned int)this, SOCKET_API_RECV_END,
			(char*)pBuffer, length, hostinfo, "xdsx",
			(unsigned int)&pBuffer, length, temp, rcvdLength);

	return pret;
}

result Socket::Send(Tizen::Base::ByteBuffer& buffer) {
	typedef result (Socket::*methodType)(Tizen::Base::ByteBuffer& buffer);
	static methodType Sendp;
	result pret;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket4SendERNS_4Base10ByteBufferE, Sendp);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_START("Socket::Send", VT_NULL, NULL,
			(unsigned int)this, (unsigned int)this, SOCKET_API_SEND_START, "",
			0, hostinfo, "x", (unsigned int)&buffer);

	pret = (this->*Sendp)(buffer);
	int bufferSize = buffer.GetLimit();
	char* pBuffer = (char*) (buffer.GetPointer());
	Tizen::Base::String strData;
	strData.SetCapacity(bufferSize);
	strData.Append(pBuffer);
	char* out = new char[bufferSize];
	WcharToChar(out, strData.GetPointer());
	int socketSendSize = SOCKET_SEND_SIZE;
	if (socketSendSize != 0 && socketSendSize < bufferSize) {
		out[socketSendSize + 1] = '.';
		out[socketSendSize + 2] = '.';
		out[socketSendSize + 3] = '.';
		out[socketSendSize + 4] = '\0';
	}
	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_END("Socket::Send", VT_ULONG, pret,
			(unsigned int)this, (unsigned int)this, SOCKET_API_SEND_END, out,
			buffer.GetLimit(), hostinfo, "x", (unsigned int)&buffer);
	delete out;
	return pret;
}

result Socket::Send(void* pBuffer, int length, int& sentLength) {
	typedef result (Socket::*methodType)(void* pBuffer, int length,
			int& sentLength);
	static methodType Sendp;
	result pret;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZZN5Tizen3Net7Sockets6Socket4SendEPviRiE19__PRETTY_FUNCTION__,
			Sendp);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_START("Socket::Send", VT_NULL, NULL,
			(unsigned int)this, (unsigned int)this, SOCKET_API_SEND_START, "",
			0, hostinfo, "xdx",
			(unsigned int)&pBuffer, length, (unsigned int)&sentLength);

	pret = (this->*Sendp)(pBuffer, length, sentLength);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_END("Socket::Send", VT_ULONG, pret,
			(unsigned int)this, (unsigned int)this, SOCKET_API_SEND_END,
			(char*)pBuffer, length, hostinfo, "xdx",
			(unsigned int)&pBuffer, length, (unsigned int)&sentLength);

	return pret;

}

result Socket::SendTo(Tizen::Base::ByteBuffer& buffer,
		const Tizen::Net::NetEndPoint& remoteEndPoint) {
	typedef result (Socket::*methodType)(Tizen::Base::ByteBuffer& buffer,
			const Tizen::Net::NetEndPoint& remoteEndPoint);
	static methodType SendTop;
	result pret;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket6SendToERNS_4Base10ByteBufferERKNS0_11NetEndPointE,
			SendTop);

	Ip4Address* iv4PeerAddr = (Ip4Address*) remoteEndPoint.GetAddress();
	char temp[64];
	WcharToChar(temp, iv4PeerAddr->ToString().GetPointer());
	char addressInfo[64];
	sprintf(addressInfo, "%s:%d", temp, remoteEndPoint.GetPort());
	hostinfo.port = remoteEndPoint.GetPort();
	iv4PeerAddr->GetAddress(&hostinfo.ip);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_START("Socket::SendTo", VT_NULL, NULL,
			(unsigned int)this, (unsigned int)this, SOCKET_API_SEND_START, "",
			0, hostinfo, "xs", (unsigned int)&buffer, temp);

	pret = (this->*SendTop)(buffer, remoteEndPoint);

	int bufferSize = buffer.GetLimit();
	char* pBuffer = (char*) (buffer.GetPointer());
	Tizen::Base::String strData;
	strData.SetCapacity(bufferSize);
	strData.Append(pBuffer);
	char* out = new char[bufferSize];
	WcharToChar(out, strData.GetPointer());
	int socketSendSize = SOCKET_SEND_SIZE;
	if (socketSendSize != 0 && socketSendSize < bufferSize) {
		out[socketSendSize + 1] = '.';
		out[socketSendSize + 2] = '.';
		out[socketSendSize + 3] = '.';
		out[socketSendSize + 4] = '\0';
	}

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_END("Socket::SendTo", VT_ULONG, pret,
			(unsigned int)this, (unsigned int)this, SOCKET_API_SEND_END, out,
			buffer.GetLimit(), hostinfo, "xs", (unsigned int)&buffer, temp);
	delete out;
	return pret;
}

result Socket::SendTo(void* pBuffer, int length,
		const Tizen::Net::NetEndPoint& remoteEndPoint, int& sentLength) {
	typedef result (Socket::*methodType)(void* pBuffer, int length,
			const Tizen::Net::NetEndPoint& remoteEndPoint, int& sentLength);
	static methodType SendTop;
	result pret;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket6SendToEPviRKNS0_11NetEndPointERi,
			SendTop);

	Ip4Address* iv4PeerAddr = (Ip4Address*) remoteEndPoint.GetAddress();
	char temp[64];
	WcharToChar(temp, iv4PeerAddr->ToString().GetPointer());
	char addressInfo[64];
	sprintf(addressInfo, "%s:%d", temp, remoteEndPoint.GetPort());
	hostinfo.port = remoteEndPoint.GetPort();
	iv4PeerAddr->GetAddress(&hostinfo.ip);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_START("Socket::SendTo", VT_NULL, NULL,
			(unsigned int)this, (unsigned int)this, SOCKET_API_SEND_START, "",
			0, hostinfo, "xdsx,",
			(unsigned int)&pBuffer, length, temp, (unsigned int)&sentLength);

	pret = (this->*SendTop)(pBuffer, length, remoteEndPoint, sentLength);

	AFTER_ORIGINAL_TIZEN_SOCK_WAIT_FUNC_END("Socket::SendTo", VT_ULONG, pret,
			(unsigned int)this, (unsigned int)this, SOCKET_API_SEND_END,
			(char*)pBuffer, length, hostinfo, "xdsx,",
			(unsigned int)&pBuffer, length, temp, (unsigned int)&sentLength);

	return pret;

}

//Option
result Socket::AddSocketListener(ISocketEventListener& listener) {
	typedef result (Socket::*methodType)(ISocketEventListener& listener);
	static methodType AddSocketListenerp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket17AddSocketListenerERNS1_20ISocketEventListenerE,
			AddSocketListenerp);

	retVal = (this->*AddSocketListenerp)(listener);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::AddSocketListener", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_OTHER, "", 0, hostinfo,
			"x", (unsigned int) &listener);

	return retVal;
}

result Socket::AsyncSelectByListener(unsigned long socketEventType) {
	typedef result (Socket::*methodType)(unsigned long socketEventType);
	static methodType AsyncSelectByListenerp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket21AsyncSelectByListenerEm,
			AsyncSelectByListenerp);

	retVal = (this->*AsyncSelectByListenerp)(socketEventType);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::AsyncSelectByListener", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_OTHER, "", 0, hostinfo,
			"x", socketEventType);

	return retVal;
}

result Socket::Ioctl(NetSocketIoctlCmd cmd, unsigned long& value) const {
	typedef result (Socket::*methodType)(NetSocketIoctlCmd cmd,
			unsigned long& value) const;
	static methodType Ioctlp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net7Sockets6Socket5IoctlENS1_17NetSocketIoctlCmdERm,
			Ioctlp);

	retVal = (this->*Ioctlp)(cmd, value);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::Ioctl", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_OTHER, "", 0, hostinfo,
			"dx", cmd, (unsigned int) &value);

	return retVal;
}

result Socket::SetSockOpt(NetSocketOptLevel optionLevel,
		NetSocketOptName optionName, int optionValue) {
	typedef result (Socket::*methodType)(NetSocketOptLevel optionLevel,
			NetSocketOptName optionName, int optionValue);
	static methodType SetSockOptp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket10SetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameEi,
			SetSockOptp);

	retVal = (this->*SetSockOptp)(optionLevel, optionName, optionValue);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::SetSockOpt", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_OTHER, "", 0, hostinfo,
			"ddd", optionLevel, optionName, optionValue);

	return retVal;
}
result Socket::SetSockOpt(NetSocketOptLevel optionLevel,
		NetSocketOptName optionName, const LingerOption& optionValue) {
	typedef result (Socket::*methodType)(NetSocketOptLevel optionLevel,
			NetSocketOptName optionName, const LingerOption& optionValue);
	static methodType SetSockOptp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket10SetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameERKNS1_12LingerOptionE,
			SetSockOptp);

	retVal = (this->*SetSockOptp)(optionLevel, optionName, optionValue);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::SetSockOpt", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_OTHER, "", 0, hostinfo,
			"ddx",
			optionLevel, optionName, (unsigned int) &optionValue);

	return retVal;
}
result Socket::SetSockOpt(NetSocketOptLevel optionLevel,
		NetSocketOptName optionName,
		const IpMulticastRequestOption& optionValue) {
	typedef result (Socket::*methodType)(NetSocketOptLevel optionLevel,
			NetSocketOptName optionName,
			const IpMulticastRequestOption& optionValue);
	static methodType SetSockOptp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket10SetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameERKNS1_12LingerOptionE,
			SetSockOptp);

	retVal = (this->*SetSockOptp)(optionLevel, optionName, optionValue);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::SetSockOpt", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_OTHER, "", 0, hostinfo,
			"ddx",
			optionLevel, optionName, (unsigned int) &optionValue);

	return retVal;
}

result Socket::GetSockOpt(NetSocketOptLevel optionLevel,
		NetSocketOptName optionName, int& optionValue) const {
	typedef result (Socket::*methodType)(NetSocketOptLevel optionLevel,
			NetSocketOptName optionName, int& optionValue) const;
	static methodType GetSockOptp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net7Sockets6Socket10GetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameERi,
			GetSockOptp);

	retVal = (this->*GetSockOptp)(optionLevel, optionName, optionValue);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::GetSockOpt", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_OTHER, "", 0, hostinfo,
			"ddx",
			optionLevel, optionName, (unsigned int) &optionValue);

	return retVal;
}

result Socket::GetSockOpt(NetSocketOptLevel optionLevel,
		NetSocketOptName optionName, LingerOption& optionValue) const {
	typedef result (Socket::*methodType)(NetSocketOptLevel optionLevel,
			NetSocketOptName optionName, LingerOption& optionValue) const;
	static methodType GetSockOptp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net7Sockets6Socket10GetSockOptENS1_17NetSocketOptLevelENS1_16NetSocketOptNameERNS1_12LingerOptionE,
			GetSockOptp);

	retVal = (this->*GetSockOptp)(optionLevel, optionName, optionValue);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::GetSockOpt", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_OTHER, "", 0, hostinfo,
			"ddx",
			optionLevel, optionName, (unsigned int) &optionValue);

	return retVal;
}

result Socket::RemoveSocketListener(ISocketEventListener& listener) {
	typedef result (Socket::*methodType)(ISocketEventListener& listener);
	static methodType RemoveSocketListenerp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net7Sockets6Socket20RemoveSocketListenerERNS1_20ISocketEventListenerE,
			RemoveSocketListenerp);

	retVal = (this->*RemoveSocketListenerp)(listener);

	AFTER_ORIGINAL_TIZEN_SOCK("Socket::RemoveSocketListener", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, SOCKET_API_OTHER, "", 0, hostinfo,
			"x", (unsigned int) &listener);

	return retVal;
}

}
}
} //namespace Tizen {namespace Net {namespace Sockets {

