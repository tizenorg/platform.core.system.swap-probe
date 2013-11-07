/*
 *  DA probe
 *
 * Copyright (File::*c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Hyunjong Park      <phjwithyou.park@samsung.com>
 * Juyoung Kim        <j0.kim@samsung.com>
 * Vitaliy Cherepanov <v.cherepanov@samsung.com>
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

/*#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>*/

#include <stdio.h>
#include <netdb.h>

#include "binproto.h"
#include "daprobe.h"
#include "probeinfo.h"
#include "dahelper.h"
#include "dacollection.h"
#include "tizen_probe.h"
#include "../probe_socket/da_socket.h"
static enum DaOptions _sopt = OPT_NETWORK;

namespace Tizen {
namespace Net {

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

enum NetProtocolType {
	NET_PROTO_TYPE_IPV4 = 1, /**< The IPV4 type */
	NET_PROTO_TYPE_PPP, /**< The PPP type */
	NET_PROTO_TYPE_IPV6, /**< The IPV6 type, currently NOT supported */
	NET_PROTO_TYPE_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application. Network Protocol type not set
};

enum NetAddressScheme {
	NET_ADDRESS_SCHEME_DYNAMIC = 1, /**< The Dynamic IP or DNS Address */
	NET_ADDRESS_SCHEME_STATIC, /**< The Static IP or DNS Address */
	NET_ADDRESS_SCHEME_NONE = 0 // This enum value is for internal use only. Using this enum value can cause behavioral, security-related, and consistency-related issues in the application. Network Address Scheme not set
};

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
// NetConnectionInfo
}
} //Tizen::Net

namespace Tizen {
namespace Net {
namespace Http {

typedef int NetAccountId;

enum HttpVersion {
	HTTP_VERSION_1_0, /**< %Http version 1.0 */
	HTTP_VERSION_1_1 /**< %Http version 1.1 */
};

enum NetHttpSessionMode {
	NET_HTTP_SESSION_MODE_NORMAL, /**< The Normal Mode */
	NET_HTTP_SESSION_MODE_PIPELINING, /**< The Pipelining mode */
	NET_HTTP_SESSION_MODE_MULTIPLE_HOST /**< The Multiple host mode */
};

enum NetHttpCookieFlag {
	NET_HTTP_COOKIE_FLAG_NONE, /**< The mode is not defined */
	NET_HTTP_COOKIE_FLAG_ALWAYS_AUTOMATIC, /**< The cookie will be parsed and saved, and also attached to request automatically */
	NET_HTTP_COOKIE_FLAG_ALWAYS_MANUAL /**< The cookie will be handled by the user action with Tizen::Net::Http::HttpRequest::SetCookie() and Tizen::Net::Http::HttpCookieStorageManager::GetCookies() */
};

enum NetHttpStatusCode {
	NET_HTTP_STATUS_UNDEFINED = 0, /**< @if OSPDEPREC The undefined status @endif */
	NET_HTTP_STATUS_CONTINUE = 100, /**< @if OSPDEPREC The status code: 100 Continue @endif */
	NET_HTTP_STATUS_SWITCHING_PROTOCOLS = 101, /**< @if OSPDEPREC The status code: 101 Switching Protocols @endif */
	NET_HTTP_STATUS_OK = 200, /**< @if OSPDEPREC The status code: 200 OK @endif */
	NET_HTTP_STATUS_CREATED = 201, /**< @if OSPDEPREC The status code: 201 Created @endif */
	NET_HTTP_STATUS_ACCEPTED = 202, /**< @if OSPDEPREC The status code: 202 Accepted @endif */
	NET_HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION = 203, /**< @if OSPDEPREC The status code: 203 Non-Authoritative Information @endif */
	NET_HTTP_STATUS_NO_CONTENT = 204, /**< @if OSPDEPREC The status code: 204 No %Content @endif */
	NET_HTTP_STATUS_RESET_CONTENT = 205, /**< @if OSPDEPREC The status code: 205 Reset %Content @endif */
	NET_HTTP_STATUS_PARTIAL_CONTENT = 206, /**< @if OSPDEPREC The status code: 206 Partial %Content @endif */

	NET_HTTP_STATUS_MULTIPLE_CHOICE = 300, /**< @if OSPDEPREC The status code: 300 Multiple Choices @endif */
	NET_HTTP_STATUS_MOVED_PERMANENTLY = 301, /**< @if OSPDEPREC The status code: 301 Moved Permanently @endif */
	NET_HTTP_STATUS_MOVED_TEMPORARILY = 302, /**< @if OSPDEPREC The status code: 302 Found @endif */
	NET_HTTP_STATUS_SEE_OTHER = 303, /**< @if OSPDEPREC The status code: 303 See Other @endif */
	NET_HTTP_STATUS_NOT_MODIFIED = 304, /**< @if OSPDEPREC The status code: 304 Not Modified @endif */
	NET_HTTP_STATUS_USE_PROXY = 305, /**< @if OSPDEPREC The status code: 305 Use Proxy @endif */

	NET_HTTP_STATUS_BAD_REQUEST = 400, /**< @if OSPDEPREC The status code: 400 Bad Request @endif */
	NET_HTTP_STATUS_UNAUTHORIZED = 401, /**< @if OSPDEPREC The status code: 401 Unauthorized @endif */
	NET_HTTP_STATUS_PAYMENT_REQUIRED = 402, /**< @if OSPDEPREC The status code: 402 Payment Required @endif */
	NET_HTTP_STATUS_FORBIDDEN = 403, /**< @if OSPDEPREC The status code: 403 Forbidden @endif */
	NET_HTTP_STATUS_NOT_FOUND = 404, /**<@if OSPDEPREC  The status code: 404 Not Found @endif */
	NET_HTTP_STATUS_METHOD_NOT_ALLOWED = 405, /**< @if OSPDEPREC The status code: 405 Method Not Allowed @endif */
	NET_HTTP_STATUS_NOT_ACCEPTABLE = 406, /**< @if OSPDEPREC The status code: 406 Not Acceptable @endif */
	NET_HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED = 407, /**< @if OSPDEPREC The status code: 407 Proxy Authentication Required @endif */
	NET_HTTP_STATUS_REQUEST_TIME_OUT = 408, /**< @if OSPDEPREC The status code: 408 Request Timeout (not used) @endif */
	NET_HTTP_STATUS_CONFLICT = 409, /**<@if OSPDEPREC  The status code: 409 Conflict @endif */
	NET_HTTP_STATUS_GONE = 410, /**< @if OSPDEPREC The status code: 410 Gone @endif */
	NET_HTTP_STATUS_LENGTH_REQUIRED = 411, /**< @if OSPDEPREC The status code: 411 Length Required @endif */
	NET_HTTP_STATUS_PRECONDITION_FAILED = 412, /**<@if OSPDEPREC  The status code: 412 Precondition Failed @endif */
	NET_HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE = 413, /**< @if OSPDEPREC The status code: 413 Request Entity Too Large (not used) @endif */
	NET_HTTP_STATUS_REQUEST_URI_TOO_LARGE = 414, /**< @if OSPDEPREC The status code: 414 Request-URI Too Long (not used) @endif */
	NET_HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE = 415, /**< @if OSPDEPREC The status code: 415 Unsupported %Media Type @endif */

	NET_HTTP_STATUS_INTERNAL_SERVER_ERROR = 500, /**< @if OSPDEPREC The status code: 500 Internal Server Error @endif */
	NET_HTTP_STATUS_NOT_IMPLEMENTED = 501, /**< @if OSPDEPREC The status code: 501 Not Implemented @endif */
	NET_HTTP_STATUS_BAD_GATEWAY = 502, /**< @if OSPDEPREC The status code: 502 Bad Gateway @endif */
	NET_HTTP_STATUS_SERVICE_UNAVAILABLE = 503, /**< @if OSPDEPREC The status code: 503 Service Unavailable @endif */
	NET_HTTP_STATUS_GATEWAY_TIME_OUT = 504, /**< @if OSPDEPREC The status code: 504 Gateway Timeout @endif */
	NET_HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED = 505 /**< @if OSPDEPREC The status code: 505 HTTP Version Not Supported @endif */
};

enum NetHttpMethod {
	NET_HTTP_METHOD_GET = 0x40, /**< The HTTP GET Method */
	NET_HTTP_METHOD_OPTIONS = 0x41, /**< The HTTP OPTIONS Method */
	NET_HTTP_METHOD_HEAD = 0x42, /**< The HTTP HEAD Method */
	NET_HTTP_METHOD_DELETE = 0x43, /**< The HTTP DELETE Method */
	NET_HTTP_METHOD_TRACE = 0x44, /**< The HTTP TRACE Method */
	NET_HTTP_METHOD_POST = 0x60, /**< The HTTP POST Method */
	NET_HTTP_METHOD_PUT = 0x61, /**< The HTTP PUT Method */
	NET_HTTP_METHOD_CONNECT = 0x70, /**< The HTTP CONNECT Method */
};

enum NetHttpCertificateVerificationFlag {
	HTTP_CV_FLAG_AUTOMATIC, /**< The certificate is verified by the system @n If server certificate is invalid, the IHttpTransactionEventListener::OnTransactionCertVerificationRequiredN() listener is received.  */
	HTTP_CV_FLAG_MANUAL, /**< The certificate verification is handled by user action when the IHttpTransactionEventListener::OnTransactionCertVerificationRequestedN() listener is received */
	HTTP_CV_FLAG_IGNORED, /**< The certificate verification is ignored */
};

enum NetHttpAuthScheme {
	NET_HTTP_AUTH_NONE = 0, /**< The no authentication type */
	NET_HTTP_AUTH_PROXY_BASIC = 1, /**< The authentication type is Proxy Basic Authentication */
	NET_HTTP_AUTH_PROXY_MD5 = 2, /**< The authentication type is Proxy Digest Authentication */
	NET_HTTP_AUTH_WWW_BASIC = 3, /**< The authentication Type is HTTP Basic Authentication */
	NET_HTTP_AUTH_WWW_MD5 = 4, /**< The authentication type is HTTP Digest Authentication */
	NET_HTTP_AUTH_PROXY_NTLM = 5, /**< The authentication type is Proxy NTLM Authentication */
	NET_HTTP_AUTH_WWW_NTLM = 7, /**< The authentication type is NTLM Authentication */
	NET_HTTP_AUTH_WWW_NEGOTIATE = 8 /**< The authentication type is Negotiate Authentication */
};

class _HttpHeaderImpl;
class HttpHeader {
public:
	HttpHeader(void);
	HttpHeader(const HttpHeader* pHeader);
	virtual ~HttpHeader(void);
public:
	result AddField(const Tizen::Base::String& fieldName,
			const Tizen::Base::String& fieldValue);
	result RemoveField(const Tizen::Base::String& fieldName);
	result RemoveField(const Tizen::Base::String& fieldName,
			const Tizen::Base::String& fieldValue);
	void RemoveAll(void);
	Tizen::Base::String* GetRawHeaderN(void) const;
	Tizen::Base::Collection::IList* GetFieldNamesN(void) const;
	Tizen::Base::Collection::IEnumerator* GetFieldValuesN(
			const Tizen::Base::String& fieldName) const;
private:
	HttpHeader(const HttpHeader& rhs);
	HttpHeader& operator =(const HttpHeader& rhs);
private:
	friend class _HttpHeaderImpl;
	_HttpHeaderImpl* __pHttpHeaderImpl;
};

Tizen::Base::String* HttpHeader::GetRawHeaderN(void) const
{
	typedef Tizen::Base::String* (HttpHeader::*methodType)(void) const;
	static methodType GetRawHeaderNp;
	Tizen::Base::String* retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZNK5Tizen3Net4Http10HttpHeader13GetRawHeaderNEv,
			GetRawHeaderNp);

	retVal = (this->*GetRawHeaderNp)();
	if (retVal == NULL)
		newerrno = 1;

	int bufferSize = retVal->GetLength();
	Tizen::Base::String strData;
	strData.SetCapacity(bufferSize);
	strData.Append(*retVal);
	char* out = new char[bufferSize];
	WcharToChar(out, strData.GetPointer());

	info.msg_buf = out;
	info.msg_pack_size = bufferSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : bufferSize;
	info.msg_total_size = bufferSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpHeader::GetRawHeaderN", VT_NULL, NULL,
				  (unsigned int)this, (unsigned int)this,
				  HTTP_API_RESPONSE, info, "s", "void");
	delete [] out;
	return retVal;
}

// HttpHeader

class HttpTransaction;
class NetConnection;
class HttpAuthentication;
class HttpCookieStorageManager;

class _HttpSessionImpl;
class HttpSession {
public:
	HttpSession(void);
	result Construct(NetHttpSessionMode sessionMode,
			const Tizen::Base::String* pProxyAddr,
			const Tizen::Base::String& hostAddr,
			const HttpHeader* pCommonHeader, NetHttpCookieFlag flag =
					NET_HTTP_COOKIE_FLAG_ALWAYS_MANUAL);
	result Construct(const NetConnection& netConnection,
			NetHttpSessionMode sessionMode,
			const Tizen::Base::String* pProxyAddr,
			const Tizen::Base::String& hostAddr,
			const HttpHeader* pCommonHeader, NetHttpCookieFlag flag =
					NET_HTTP_COOKIE_FLAG_ALWAYS_MANUAL);
	~HttpSession(void);
public:
	HttpTransaction* OpenTransactionN(void);
	HttpTransaction* OpenTransactionN(const HttpAuthentication& auth);
	result CancelTransaction(HttpTransaction& httpTransaction);
	result CloseTransaction(HttpTransaction& httpTransaction);
	result CloseAllTransactions(void);
	result SetAutoRedirectionEnabled(bool enable);
	bool IsAutoRedirectionEnabled(void) const;
	int GetActiveTransactionCount(void) const;
	int GetMaxTransactionCount(void) const;
	HttpCookieStorageManager* GetCookieStorageManager(void) const;
private:
	HttpSession(const HttpSession& rhs);
	HttpSession& operator =(const HttpSession& rhs);
private:
	friend class _HttpSessionImpl;
	_HttpSessionImpl* __pHttpSessionImpl;
};
// HttpSession

result HttpSession::Construct(NetHttpSessionMode sessionMode,
		const Tizen::Base::String* pProxyAddr,
		const Tizen::Base::String& hostAddr, const HttpHeader* pCommonHeader,
		NetHttpCookieFlag flag) {
	typedef result (HttpSession::*methodType)(NetHttpSessionMode sessionMode,
			const Tizen::Base::String* pProxyAddr,
			const Tizen::Base::String& hostAddr,
			const HttpHeader* pCommonHeader, NetHttpCookieFlag flag);
	static methodType Constructp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpSession9ConstructENS1_18NetHttpSessionModeEPKNS_4Base6StringERS6_PKNS1_10HttpHeaderENS1_17NetHttpCookieFlagE,
			Constructp);

	retVal = (this->*Constructp)(sessionMode, pProxyAddr, hostAddr,
			pCommonHeader, flag);

	char temp1[50];
	if (NULL != pProxyAddr) {
		WcharToChar(temp1, pProxyAddr->GetPointer());
	} else {
		strcpy(temp1, "NULL");
	}
	char temp2[50];
	WcharToChar(temp2, hostAddr.GetPointer());

	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::Construct", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_SESSION_CONSTRUCT,
			info, "dssxd",
			sessionMode, temp1, temp2, (unsigned int)&pCommonHeader, flag);
	return retVal;
}

result HttpSession::Construct(const NetConnection& netConnection,
		NetHttpSessionMode sessionMode, const Tizen::Base::String* pProxyAddr,
		const Tizen::Base::String& hostAddr, const HttpHeader* pCommonHeader,
		NetHttpCookieFlag flag)
{
	typedef result (HttpSession::*methodType)(
			const NetConnection& netConnection, NetHttpSessionMode sessionMode,
			const Tizen::Base::String* pProxyAddr,
			const Tizen::Base::String& hostAddr,
			const HttpHeader* pCommonHeader, NetHttpCookieFlag flag);
	static methodType Constructp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpSession9ConstructERKNS0_13NetConnectionENS1_18NetHttpSessionModeEPKNS_4Base6StringERS9_PKNS1_10HttpHeaderENS1_17NetHttpCookieFlagE,
			Constructp);

	retVal = (this->*Constructp)(netConnection, sessionMode, pProxyAddr,
			hostAddr, pCommonHeader, flag);

	char temp1[50];
	if (NULL != pProxyAddr) {
		WcharToChar(temp1, pProxyAddr->GetPointer());
	} else {
		strcpy(temp1, "NULL");
	}
	char temp2[50];
	WcharToChar(temp2, hostAddr.GetPointer());

	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::Construct", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_SESSION_CONSTRUCT,
			info, "xdssxd",
			(unsigned int)&netConnection, sessionMode, temp1, temp2, (unsigned int)&pCommonHeader, flag);
	return retVal;
}

HttpTransaction* HttpSession::OpenTransactionN(void)
{
	typedef HttpTransaction* (HttpSession::*methodType)(void);
	static methodType OpenTransactionNp;

	HttpTransaction* retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpSession16OpenTransactionNEv,
			OpenTransactionNp);

	retVal = (this->*OpenTransactionNp)();
	if (retVal == NULL)
		newerrno = 1;
	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::OpenTransactionN", VT_PTR, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_TRANSACTION_OPEN,
			info, "s", "void");
	return retVal;
}

HttpTransaction* HttpSession::OpenTransactionN(const HttpAuthentication& auth)
{
	typedef HttpTransaction* (HttpSession::*methodType)(
			const HttpAuthentication& auth);
	static methodType OpenTransactionNp;

	HttpTransaction* retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpSession16OpenTransactionNERKNS1_18HttpAuthenticationE,
			OpenTransactionNp);

	retVal = (this->*OpenTransactionNp)(auth);

	if (retVal == NULL)
		newerrno = 1;
	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::OpenTransactionN", VT_PTR, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_TRANSACTION_OPEN,
			info, "x", (unsigned int)&auth);
	return retVal;

}

result HttpSession::CancelTransaction(HttpTransaction& httpTransaction)
{
	typedef result (HttpSession::*methodType)(HttpTransaction& httpTransaction);
	static methodType CancelTransactionp;

	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpSession17CancelTransactionERNS1_15HttpTransactionE,
			CancelTransactionp);

	retVal = (this->*CancelTransactionp)(httpTransaction);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::CancelTransaction", VT_ULONG,
			retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_TRANSACTION_CLOSE, info, "x",
			(unsigned int)&httpTransaction);

	return retVal;
}

result HttpSession::CloseTransaction(HttpTransaction& httpTransaction)
{
	typedef result (HttpSession::*methodType)(HttpTransaction& httpTransaction);
	static methodType CloseTransactionp;

	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpSession16CloseTransactionERNS1_15HttpTransactionE,
			CloseTransactionp);

	retVal = (this->*CloseTransactionp)(httpTransaction);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::CloseTransaction", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_TRANSACTION_CLOSE,
			info, "x", (unsigned int)&httpTransaction);

	return retVal;
}

result HttpSession::CloseAllTransactions(void)
{
	typedef result (HttpSession::*methodType)(void);
	static methodType CloseTransactionp;

	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpSession20CloseAllTransactionsEv,
			CloseTransactionp);

	retVal = (this->*CloseTransactionp)();

	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::CloseAllTransactions", VT_ULONG,
			retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_TRANSACTION_CLOSE, info, "s", "void");

	return retVal;
}

result HttpSession::SetAutoRedirectionEnabled(bool enable)
{
	typedef result (HttpSession::*methodType)(bool enable);
	static methodType SetAutoRedirectionEnabledp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpSession25SetAutoRedirectionEnabledEb,
			SetAutoRedirectionEnabledp);

	retVal = (this->*SetAutoRedirectionEnabledp)(enable);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::SetAutoRedirectionEnabled",
			VT_ULONG, retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_OTHER, info, "d", enable);
	return retVal;
}

bool HttpSession::IsAutoRedirectionEnabled(void) const
{
	typedef bool (HttpSession::*methodType)(void) const;
	static methodType IsAutoRedirectionEnabledp;
	bool retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http11HttpSession24IsAutoRedirectionEnabledEv,
			IsAutoRedirectionEnabledp);

	retVal = (this->*IsAutoRedirectionEnabledp)();

	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::IsAutoRedirectionEnabled", VT_INT,
				  retVal, (unsigned int)this, (unsigned int)this,
				  HTTP_API_OTHER, info, "s", "void");
	return retVal;
}

int HttpSession::GetActiveTransactionCount(void) const
{
	typedef int (HttpSession::*methodType)(void) const;
	static methodType GetActiveTransactionCountp;
	int retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http11HttpSession25GetActiveTransactionCountEv,
			GetActiveTransactionCountp);

	retVal = (this->*GetActiveTransactionCountp)();

	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::GetActiveTransactionCount", VT_INT,
				  retVal, (unsigned int)this, (unsigned int)this,
				  HTTP_API_OTHER, info, "s", "void");
	return retVal;
}

int HttpSession::GetMaxTransactionCount(void) const
{
	typedef int (HttpSession::*methodType)(void) const;
	static methodType GetMaxTransactionCountp;
	int retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http11HttpSession22GetMaxTransactionCountEv,
			GetMaxTransactionCountp);

	retVal = (this->*GetMaxTransactionCountp)();

	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::GetMaxTransactionCount", VT_INT,
				  retVal, (unsigned int)this, (unsigned int)this,
				  HTTP_API_OTHER, info, "s", "void");
	return retVal;
}

HttpCookieStorageManager* HttpSession::GetCookieStorageManager(void) const
{
	typedef HttpCookieStorageManager* (HttpSession::*methodType)(void) const;
	static methodType GetMaxTransactionCountp;
	HttpCookieStorageManager* retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http11HttpSession23GetCookieStorageManagerEv,
			GetMaxTransactionCountp);

	retVal = (this->*GetMaxTransactionCountp)();
	if (retVal == NULL)
		newerrno = 1;
	AFTER_ORIGINAL_TIZEN_SOCK("HttpSession::GetCookieStorageManager", VT_PTR,
			retVal, (unsigned int)this, (unsigned int)this, HTTP_API_ALLOCATION,
			info, "s", "void");
	return retVal;
}

HttpSession::~HttpSession(void)
{
	typedef void (HttpSession::*methodType)(void);
	static methodType HttpSessionDp = 0;

	probeInfo_t probeInfo;
	log_t log;
	int blockresult = 0;
	bool bfiltering = false;
	if (!HttpSessionDp) {
		if (lib_handle[LIBOSP_NET] == NULL) {
			lib_handle[LIBOSP_NET] = dlopen(lib_string[LIBOSP_NET], RTLD_LAZY);
			if (lib_handle[LIBOSP_NET] == NULL) {
				char perror_msg[128];
				sprintf(perror_msg, "dlopen failed : %s",
						lib_string[LIBOSP_NET]);
				perror(perror_msg);
				exit(0);
			}
		}
		void* funcp = dlsym(lib_handle[LIBOSP_NET],
				"_ZN5Tizen3Net4Http11HttpSessionD1Ev");
		if (funcp == NULL || dlerror() != NULL) {
			perror("dlsym failed : " "_ZN5Tizen3Net4Http11HttpSessionD1Ev");
			exit(0);
		}
		memcpy(&HttpSessionDp, &funcp, sizeof(void*));
	}

	if ((blockresult =
			preBlockBegin(
					((void*) __builtin_extract_return_addr(
							__builtin_return_address(0))), bfiltering, _sopt))
			!= 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	(this->*HttpSessionDp)();

	if (postBlockBegin(blockresult)) {
		log.type = 0;
		log.length = 0;
		log.data[0] = '\0';
		log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d", 15,
				probeInfo.eventIndex, "HttpSession::~HttpSession",
				probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		log.length += sprintf(log.data + log.length, "`,`,");
		log.length += sprintf(log.data + log.length,
				"`,0`,0`,2`,%u`,0x%x`,0x%x`,2`,%d`,`,",
				(unsigned int) CALLER_ADDRESS, (unsigned int) this,
				(unsigned int) this, HTTP_API_TRANSACTION_CLOSE);
		log.length += sprintf(log.data + log.length, "`,\ncallstack_start`,");
		getBacktraceString(&log, 4096 - log.length - 17);
		log.length += sprintf(log.data + log.length, "`,callstack_end");

		printLog(&log, MSG_LOG);
		postBlockEnd();
	}
}

//HttpSession;

class HttpRequest;
class HttpResponse;
class IHttpTransactionEventListener;
class IHttpProgressEventListener;

class _HttpTransactionImpl;
class HttpTransaction {
public:
	~HttpTransaction(void);
public:

	HttpAuthentication* OpenAuthenticationInfoN(void);
	HttpRequest* GetRequest(void) const;
	HttpResponse* GetResponse(void) const;
	result AddHttpTransactionListener(IHttpTransactionEventListener& listener);
	result RemoveHttpTransactionListener(
			IHttpTransactionEventListener& listener);
	result SetHttpProgressListener(IHttpProgressEventListener& listener);
	result SetUserObject(const Tizen::Base::Object* pUserData);
	Tizen::Base::Object* GetUserObject(void) const;
	bool EnableTransactionReadyToWrite(void);
	result Submit(void);
	result Resume(void);
	result Pause(void);
	result SetClientCertificate(int certificateId);
	result SetTimeout(int timeout);
	result SetServerCertificateVerification(
			NetHttpCertificateVerificationFlag flag = HTTP_CV_FLAG_AUTOMATIC);
private:
	HttpTransaction(void);
	HttpTransaction(const HttpTransaction& rhs);
	HttpTransaction& operator =(const HttpTransaction& rhs);
private:
	friend class _HttpTransactionImpl;
	_HttpTransactionImpl* __pHttpTransactionImpl;
};

HttpAuthentication* HttpTransaction::OpenAuthenticationInfoN(void)
{
	typedef HttpAuthentication* (HttpTransaction::*methodType)(void) const;
	static methodType GetRequestp;
	HttpAuthentication* retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http15HttpTransaction10GetRequestEv, GetRequestp);

	retVal = (this->*GetRequestp)();
	if (retVal == NULL)
		newerrno = 1;
	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::OpenAuthenticationInfoN",
			VT_PTR, retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_ALLOCATION, info, "s", "void");
	return retVal;
}

HttpRequest* HttpTransaction::GetRequest(void) const
{
	typedef HttpRequest* (HttpTransaction::*methodType)(void) const;
	static methodType GetRequestp;
	HttpRequest* retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http15HttpTransaction10GetRequestEv, GetRequestp);

	retVal = (this->*GetRequestp)();
	if (retVal == NULL)
		newerrno = 1;
	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::GetRequest", VT_PTR, retVal,
				  (unsigned int)this, (unsigned int)this,
				  HTTP_API_ALLOCATION, info, "s", "void");
	return retVal;
}

HttpResponse* HttpTransaction::GetResponse(void) const
{
	typedef HttpResponse* (HttpTransaction::*methodType)(void) const;
	static methodType GetResponsep;
	HttpResponse* retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http15HttpTransaction11GetResponseEv, GetResponsep);

	retVal = (this->*GetResponsep)();
	if (retVal == NULL)
		newerrno = 1;
	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::GetResponse", VT_PTR, retVal,
				  (unsigned int)this, (unsigned int)this,
				  HTTP_API_ALLOCATION, info, "s", "void");
	return retVal;
}

result HttpTransaction::AddHttpTransactionListener(
		IHttpTransactionEventListener& listener) {
	typedef result (HttpTransaction::*methodType)(
			IHttpTransactionEventListener& listener);
	static methodType AddHttpTransactionListenerp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http15HttpTransaction26AddHttpTransactionListenerERNS1_29IHttpTransactionEventListenerE,
			AddHttpTransactionListenerp);

	retVal = (this->*AddHttpTransactionListenerp)(listener);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::AddHttpTransactionListener",
			VT_ULONG, retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_OTHER, info, "x", (unsigned int)&listener);
	return retVal;
}

result HttpTransaction::RemoveHttpTransactionListener(
		IHttpTransactionEventListener& listener) {
	typedef result (HttpTransaction::*methodType)(
			IHttpTransactionEventListener& listener);
	static methodType RemoveHttpTransactionListenerp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http15HttpTransaction29RemoveHttpTransactionListenerERNS1_29IHttpTransactionEventListenerE,
			RemoveHttpTransactionListenerp);

	retVal = (this->*RemoveHttpTransactionListenerp)(listener);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::RemoveHttpTransactionListener",
			VT_ULONG, retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_OTHER, info, "x", (unsigned int)&listener);
	return retVal;
}

result HttpTransaction::SetHttpProgressListener(
		IHttpProgressEventListener& listener) {
	typedef result (HttpTransaction::*methodType)(
			IHttpProgressEventListener& listener);
	static methodType SetHttpProgressListenerp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http15HttpTransaction23SetHttpProgressListenerERNS1_26IHttpProgressEventListenerE,
			SetHttpProgressListenerp);

	retVal = (this->*SetHttpProgressListenerp)(listener);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::SetHttpProgressListener",
			VT_ULONG, retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_OTHER, info, "x", (unsigned int)&listener);
	return retVal;
}

result HttpTransaction::SetUserObject(const Tizen::Base::Object* pUserData)
{
	typedef result (HttpTransaction::*methodType)(
			const Tizen::Base::Object* pUserData);
	static methodType SetUserObjectp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http15HttpTransaction13SetUserObjectEPKNS_4Base6ObjectE,
			SetUserObjectp);

	retVal = (this->*SetUserObjectp)(pUserData);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::SetUserObject", VT_ULONG,
				  retVal, (unsigned int)this, (unsigned int)this,
				  HTTP_API_OTHER, info, "x",
				  (unsigned int)&pUserData);
	return retVal;
}

Tizen::Base::Object* HttpTransaction::GetUserObject(void) const
{
	typedef Tizen::Base::Object* (HttpTransaction::*methodType)(void) const;
	static methodType GetUserObjectp;
	Tizen::Base::Object* retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http15HttpTransaction13GetUserObjectEv,
			GetUserObjectp);

	retVal = (this->*GetUserObjectp)();

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::GetUserObject", VT_PTR, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"s", "void");
	return retVal;
}

bool HttpTransaction::EnableTransactionReadyToWrite(void)
{
	typedef bool (HttpTransaction::*methodType)(void) const;
	static methodType EnableTransactionReadyToWritep;
	bool retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http15HttpTransaction29EnableTransactionReadyToWriteEv,
			EnableTransactionReadyToWritep);

	retVal = (this->*EnableTransactionReadyToWritep)();

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::GetUserObject", VT_INT, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"s", "void");
	return retVal;
}
//
result HttpTransaction::Submit(void)
{
	typedef result (HttpTransaction::*methodType)(void);
	static methodType Submitp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZN5Tizen3Net4Http15HttpTransaction6SubmitEv,
			Submitp);

	retVal = (this->*Submitp)();

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::Submit", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_SUBMIT, info,
			"s", "void");
	return retVal;
}

result HttpTransaction::Resume(void)
{
	typedef result (HttpTransaction::*methodType)(void);
	static methodType Resumep;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZN5Tizen3Net4Http15HttpTransaction6ResumeEv,
			Resumep);

	retVal = (this->*Resumep)();

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::Resume", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"s", "void");
	return retVal;
}

result HttpTransaction::Pause(void)
{
	typedef result (HttpTransaction::*methodType)(void);
	static methodType Pausep;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZN5Tizen3Net4Http15HttpTransaction5PauseEv,
			Pausep);

	retVal = (this->*Pausep)();

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::Pause", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"s", "void");
	return retVal;
}

result HttpTransaction::SetClientCertificate(int certificateId)
{
	typedef result (HttpTransaction::*methodType)(int certificateId);
	static methodType SetClientCertificatep;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http15HttpTransaction20SetClientCertificateEi,
			SetClientCertificatep);

	retVal = (this->*SetClientCertificatep)(certificateId);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::SetClientCertificate", VT_ULONG,
				  retVal, (unsigned int)this, (unsigned int)this,
				  HTTP_API_OTHER, info, "d", certificateId);
	return retVal;
}

result HttpTransaction::SetTimeout(int timeout)
{
	typedef result (HttpTransaction::*methodType)(int timeout);
	static methodType SetTimeoutp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http15HttpTransaction10SetTimeoutEi, SetTimeoutp);

	retVal = (this->*SetTimeoutp)(timeout);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::SetTimeout", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"d", timeout);
	return retVal;
}
result HttpTransaction::SetServerCertificateVerification(
		NetHttpCertificateVerificationFlag flag) {
	typedef result (HttpTransaction::*methodType)(
			NetHttpCertificateVerificationFlag flag);
	static methodType SetServerCertificateVerificationp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http15HttpTransaction32SetServerCertificateVerificationENS1_34NetHttpCertificateVerificationFlagE,
			SetServerCertificateVerificationp);

	retVal = (this->*SetServerCertificateVerificationp)(flag);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpTransaction::SetServerCertificateVerification",
				  VT_ULONG, retVal, (unsigned int)this,
				  (unsigned int)this, HTTP_API_OTHER, info, "d",
				  flag);
	return retVal;
}

HttpTransaction::~HttpTransaction(void)
{
	typedef void (HttpTransaction::*methodType)(void);
	static methodType HttpTransactiondp = 0;

	probeInfo_t probeInfo;
	log_t log;
	int blockresult = 0;
	bool bfiltering = false;
	if (!HttpTransactiondp) {
		if (lib_handle[LIBOSP_NET] == NULL) {
			lib_handle[LIBOSP_NET] = dlopen(lib_string[LIBOSP_NET], RTLD_LAZY);
			if (lib_handle[LIBOSP_NET] == NULL) {
				char perror_msg[128];
				sprintf(perror_msg, "dlopen failed : %s",
						lib_string[LIBOSP_NET]);
				perror(perror_msg);
				exit(0);
			}
		}
		void* funcp = dlsym(lib_handle[LIBOSP_NET],
				"_ZN5Tizen3Net4Http15HttpTransactionD1Ev");
		if (funcp == NULL || dlerror() != NULL) {
			perror("dlsym failed : " "_ZN5Tizen3Net4Http15HttpTransactionD1Ev");
			exit(0);
		}
		memcpy(&HttpTransactiondp, &funcp, sizeof(void*));
	}

	if ((blockresult =
			preBlockBegin(
					((void*) __builtin_extract_return_addr(
							__builtin_return_address(0))), bfiltering, _sopt))
			!= 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}

	(this->*HttpTransactiondp)();

	if (postBlockBegin(blockresult)) {
		log.type = 0;
		log.length = 0;
		log.data[0] = '\0';
		log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d", 15,
				probeInfo.eventIndex, "HttpTransaction::~HttpTransaction",
				probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
		log.length += sprintf(log.data + log.length, "`,`,");
		log.length += sprintf(log.data + log.length,
				"`,0`,0`,2`,%u`,0x%x`,0x%x`,2`,%d`,`,",
				(unsigned int) CALLER_ADDRESS, (unsigned int) this,
				(unsigned int) this, HTTP_API_TRANSACTION_CLOSE);
		log.length += sprintf(log.data + log.length, "`,\ncallstack_start`,");
		getBacktraceString(&log, 4096 - log.length - 17);
		log.length += sprintf(log.data + log.length, "`,callstack_end");

		printLog(&log, MSG_LOG);
		postBlockEnd();
	}
}

// HttpTransaction

class HttpCredentials;

class _HttpAuthenticationImpl;
class HttpAuthentication {
public:
	~HttpAuthentication(void);
	Tizen::Base::String* GetRealmN(void) const;
	NetHttpAuthScheme GetAuthScheme(void) const;
	HttpTransaction* SetCredentials(HttpCredentials& credentials);
private:
	HttpAuthentication(void);
	HttpAuthentication(const HttpAuthentication& rhs);
	HttpAuthentication& operator =(const HttpAuthentication& rhs);
private:
	_HttpAuthenticationImpl* __pHttpAuthenticationImpl;
	friend class _HttpAuthenticationImpl;
};
// HttpAuthentication

class HttpCredentials {
public:
	HttpCredentials(void);
	HttpCredentials(const Tizen::Base::String& name,
			const Tizen::Base::String& password);
	HttpCredentials(const HttpCredentials& rhs);
	virtual ~HttpCredentials(void);
	HttpCredentials& operator =(const HttpCredentials& rhs);
	result SetName(const Tizen::Base::String& name);
	result SetPassword(const Tizen::Base::String& password);
	Tizen::Base::String GetName(void) const;
	Tizen::Base::String GetPassword(void) const;
	virtual bool Equals(const Tizen::Base::Object& rhs) const;
	virtual int GetHashCode(void) const;
private:
	Tizen::Base::String __userName;
	Tizen::Base::String __password;
	friend class HttpCredentialsImpl;
	class HttpCredentialsImpl* __pHttpCredentialsImpl;
};
// HttpCredentials

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
// NetConnection

class IHttpEntity;

class _HttpRequestImpl;
class HttpRequest {
public:
	result SetMethod(NetHttpMethod method);
	result SetCustomMethod(const Tizen::Base::String& method);
	result SetVersion(HttpVersion version);
	result SetUri(const Tizen::Base::String& uri);
	result WriteBody(const Tizen::Base::ByteBuffer& body);
	result SetEntity(IHttpEntity& entity);
	HttpHeader* GetHeader(void) const;
	result SetCookie(const Tizen::Base::String& cookieString);
	Tizen::Base::String GetCookie(void) const;
	result SetAcceptEncoding(const Tizen::Base::String& encoding);
protected:
	HttpRequest(void);
	~HttpRequest(void);
	result Construct(const HttpTransaction& httpTransaction,
			const HttpHeader* pCommonHeader);
	NetHttpMethod GetMethod(void) const;
	result GetCustomMethod(Tizen::Base::String& method) const;
	HttpVersion GetVersion(void) const;
	result GetUri(Tizen::Base::String& uri) const;
	Tizen::Base::ByteBuffer* ReadBodyN(void);
	Tizen::Base::String GetAcceptEncoding(void) const;
private:
	HttpRequest(const HttpRequest& rhs);
	HttpRequest& operator =(const HttpRequest& rhs);
private:
	friend class _HttpRequestImpl;
	_HttpRequestImpl* __pHttpRequestImpl;
};

result HttpRequest::SetMethod(NetHttpMethod method)
{
	typedef result (HttpRequest::*methodType)(NetHttpMethod method);
	static methodType SetMethodp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpRequest9SetMethodENS1_13NetHttpMethodE,
			SetMethodp);

	retVal = (this->*SetMethodp)(method);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::SetMethod", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"d", method);
	return retVal;
}

result HttpRequest::SetCustomMethod(const Tizen::Base::String& method)
{
	typedef result (HttpRequest::*methodType)(
			const Tizen::Base::String& method);
	static methodType SetCustomMethodp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpRequest15SetCustomMethodERKNS_4Base6StringE,
			SetCustomMethodp);

	retVal = (this->*SetCustomMethodp)(method);

	int nSize = method.GetLength();
	char temp[nSize];
	WcharToChar(temp, method.GetPointer());
	info.msg_buf = temp;
	info.msg_total_size = nSize;
	info.msg_pack_size = nSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::SetCustomMethod", VT_ULONG,
				  retVal, (unsigned int)this, (unsigned int)this,
				  HTTP_API_REQUEST, info, "x",
				  (unsigned int)&method);
	return retVal;
}

result HttpRequest::SetVersion(HttpVersion version)
{
	typedef result (HttpRequest::*methodType)(HttpVersion version);
	static methodType SetVersionp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpRequest6SetUriERKNS_4Base6StringE,
			SetVersionp);

	retVal = (this->*SetVersionp)(version);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::SetVersion", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"d", version);
	return retVal;
}

result HttpRequest::SetUri(const Tizen::Base::String& uri)
{
	typedef result (HttpRequest::*methodType)(const Tizen::Base::String& uri);
	static methodType SetUrip;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpRequest6SetUriERKNS_4Base6StringE, SetUrip);

	retVal = (this->*SetUrip)(uri);

	int nSize = uri.GetLength();
	char temp[nSize];
	WcharToChar(temp, uri.GetPointer());

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::SetUri", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_REQUEST, info,
			"s", temp);
	return retVal;
}

result HttpRequest::WriteBody(const Tizen::Base::ByteBuffer& body)
{
	typedef result (HttpRequest::*methodType)(
			const Tizen::Base::ByteBuffer& body);
	static methodType WriteBodyp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpRequest9WriteBodyERKNS_4Base10ByteBufferE,
			WriteBodyp);

	retVal = (this->*WriteBodyp)(body);

	int bufferSize = body.GetLimit();
	char* pBuffer = (char*) (body.GetPointer());
	Tizen::Base::String strData;
	strData.SetCapacity(bufferSize);
	strData.Append(pBuffer);
	char* out = new char[bufferSize];
	WcharToChar(out, strData.GetPointer());

	info.msg_buf = out;
	info.msg_pack_size = bufferSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : bufferSize;
	info.msg_total_size = bufferSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::WriteBody", VT_ULONG, retVal,
				  (unsigned int)this, (unsigned int)this,
				  HTTP_API_REQUEST, info, "x",
				  (unsigned int)&body);
	delete [] out;
	return retVal;
}

result HttpRequest::SetEntity(IHttpEntity& entity)
{
	typedef result (HttpRequest::*methodType)(IHttpEntity& entity);
	static methodType SetEntityp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpRequest9SetEntityERNS1_11IHttpEntityE,
			SetEntityp);

	retVal = (this->*SetEntityp)(entity);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::SetEntity", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"x", (unsigned int)& entity);
	return retVal;
}

HttpHeader* HttpRequest::GetHeader(void) const
{
	typedef HttpHeader* (HttpRequest::*methodType)(void) const;
	static methodType GetHeaderp;
	HttpHeader* retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZNK5Tizen3Net4Http11HttpRequest9GetHeaderEv,
			GetHeaderp);
	retVal = (this->*GetHeaderp)();
	if (retVal == NULL)
		newerrno = 1;
	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::GetHeader", VT_PTR, retVal,
				  (unsigned int)this, (unsigned int)this,
				  HTTP_API_ALLOCATION, info, "s", "void");
	return retVal;
}

result HttpRequest::SetCookie(const Tizen::Base::String& cookieString)
{
	typedef result (HttpRequest::*methodType)(
			const Tizen::Base::String& cookieString);
	static methodType SetCookiep;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpRequest9SetCookieERKNS_4Base6StringE,
			SetCookiep);

	retVal = (this->*SetCookiep)(cookieString);

	int nSize = cookieString.GetLength();
	char temp[nSize];
	WcharToChar(temp, cookieString.GetPointer());
	info.msg_buf = temp;
	info.msg_total_size = nSize;
	info.msg_pack_size = nSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : nSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::SetCookie", VT_ULONG, retVal,
				  (unsigned int)this, (unsigned int)this,
				  HTTP_API_REQUEST, info, "x",
				  (unsigned int)&cookieString);
	return retVal;
}

Tizen::Base::String HttpRequest::GetCookie(void) const
{
	typedef Tizen::Base::String (HttpRequest::*methodType)(void) const;
	static methodType GetCookiep;
	Tizen::Base::String retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZNK5Tizen3Net4Http11HttpRequest9GetCookieEv,
			GetCookiep);

	retVal = (this->*GetCookiep)();

	int nSize = retVal.GetLength();
	char temp[nSize];
	WcharToChar(temp, retVal.GetPointer());
	info.msg_buf = temp;
	info.msg_total_size = nSize;
	info.msg_pack_size = nSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : nSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::GetCookie", VT_PTR,
				  (unsigned int)&retVal,
				  (unsigned int)this, (unsigned int)this,
				  HTTP_API_OTHER, info, "s", "void");
	return retVal;
}

result HttpRequest::SetAcceptEncoding(const Tizen::Base::String& encoding)
{
	typedef result (HttpRequest::*methodType)(const Tizen::Base::String& uri);
	static methodType SetAcceptEncodingp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http11HttpRequest6SetUriERKNS_4Base6StringE,
			SetAcceptEncodingp);

	retVal = (this->*SetAcceptEncodingp)(encoding);

	int nSize = encoding.GetLength();
	char temp[nSize];
	WcharToChar(temp, encoding.GetPointer());
	info.msg_buf = temp;
	info.msg_total_size = nSize;
	info.msg_pack_size = nSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : nSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::SetAcceptEncoding", VT_ULONG,
				  retVal, (unsigned int)this, (unsigned int)this,
				  HTTP_API_REQUEST, info, "s", temp);
	return retVal;
}

Tizen::Base::ByteBuffer* HttpRequest::ReadBodyN(void)
{
	typedef Tizen::Base::ByteBuffer* (HttpRequest::*methodType)(void);
	static methodType ReadBodyNp;
	Tizen::Base::ByteBuffer* retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZN5Tizen3Net4Http11HttpRequest9ReadBodyNEv,
			ReadBodyNp);
	retVal = (this->*ReadBodyNp)();

	int bufferSize = retVal->GetLimit();
	Tizen::Base::ByteBuffer copybuffer;
	copybuffer.Construct(bufferSize + 4);
	copybuffer.CopyFrom(*retVal);

	info.msg_buf = (char *)(copybuffer.GetPointer());
	info.msg_total_size = bufferSize;
	info.msg_pack_size = bufferSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : bufferSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::ReadBodyN", VT_PTR,
				  (unsigned int)&retVal,
				  (unsigned int)this, (unsigned int)this,
				  HTTP_API_OTHER, info, "s", "void");
	return retVal;
}

Tizen::Base::String HttpRequest::GetAcceptEncoding(void) const
{
	typedef Tizen::Base::String (HttpRequest::*methodType)(void) const;
	static methodType HttpRequestp;
	Tizen::Base::String retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http11HttpRequest17GetAcceptEncodingEv,
			HttpRequestp);
	retVal = (this->*HttpRequestp)();

	int nSize = retVal.GetLength();
	char temp[nSize];
	WcharToChar(temp, retVal.GetPointer());
	info.msg_buf = temp;
	info.msg_total_size = nSize;
	info.msg_pack_size = nSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : nSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpRequest::GetAcceptEncoding", VT_PTR,
				  (unsigned int)&retVal,
				  (unsigned int)this, (unsigned int)this,
				  HTTP_API_OTHER, info, "s", "void");
	return retVal;
}

HttpRequest::~HttpRequest() {
}

// HttpRequest

class _HttpResponseImpl;
class HttpResponse {
public:
	NetHttpStatusCode GetStatusCode(void) const;
	int GetHttpStatusCode(void) const;
	Tizen::Base::String GetStatusText(void) const;
	Tizen::Base::String GetVersion(void) const;
	HttpHeader* GetHeader(void) const;
	Tizen::Base::ByteBuffer* ReadBodyN(void);
	Tizen::Base::Collection::IList* GetCookies(void) const;
protected:
	HttpResponse(void);
	virtual ~HttpResponse(void);
	result Construct(const HttpTransaction& httpTransaction);
protected:
	result SetStatusCode(NetHttpStatusCode statusCode);
	result SetHttpStatusCode(int statusCode);
	result SetStatusText(const Tizen::Base::String& statusText);
	result SetVersion(const Tizen::Base::String& httpVersion);
	result WriteBody(const Tizen::Base::ByteBuffer& body);
	result Read(int headerLen, int bodyLen, int& rcvHeaderLen, int& rcvBodyLen);
	result SetCookie(Tizen::Net::Http::HttpHeader* pHeader);
private:
	HttpResponse(const HttpResponse& rhs);
	HttpResponse& operator =(const HttpResponse& rhs);
private:
	friend class _HttpResponseImpl;
	_HttpResponseImpl* __pHttpResponseImpl;

};

NetHttpStatusCode HttpResponse::GetStatusCode(void) const
{
	typedef NetHttpStatusCode (HttpResponse::*methodType)(void) const;
	static methodType GetHttpStatusCodep;
	NetHttpStatusCode retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http12HttpResponse17GetHttpStatusCodeEv,
			GetHttpStatusCodep);

	retVal = (this->*GetHttpStatusCodep)();
	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::GetStatusCode", VT_INT, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"s", "void");
	return retVal;

}

int HttpResponse::GetHttpStatusCode(void) const
{
	typedef int (HttpResponse::*methodType)(void) const;
	static methodType GetHttpStatusCodep;
	int retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http12HttpResponse17GetHttpStatusCodeEv,
			GetHttpStatusCodep);

	retVal = (this->*GetHttpStatusCodep)();

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::GetHttpStatusCode", VT_INT, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"s", "void");
	return retVal;
}

Tizen::Base::String HttpResponse::GetStatusText(void) const
{
	typedef Tizen::Base::String (HttpResponse::*methodType)(void) const;
	static methodType GetStatusTextp;
	Tizen::Base::String retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZNK5Tizen3Net4Http12HttpResponse13GetStatusTextEv, GetStatusTextp);

	retVal = (this->*GetStatusTextp)();
	int nSize = retVal.GetLength();
	char temp[nSize];
	WcharToChar(temp, retVal.GetPointer());

	info.msg_buf = temp;
	info.msg_total_size = nSize;
	info.msg_pack_size = nSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : nSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::GetStatusText", VT_PTR,
			(unsigned int)&retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_RESPONSE, info, "s", "void");
	return retVal;
}

Tizen::Base::String HttpResponse::GetVersion(void) const
{
	typedef Tizen::Base::String (HttpResponse::*methodType)(void) const;
	static methodType GetVersionp;
	Tizen::Base::String retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZNK5Tizen3Net4Http12HttpResponse10GetVersionEv,
			GetVersionp);

	retVal = (this->*GetVersionp)();
	int nSize = retVal.GetLength();
	char temp[nSize];
	WcharToChar(temp, retVal.GetPointer());

	info.msg_buf = temp;
	info.msg_total_size = nSize;
	info.msg_pack_size = nSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : nSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::GetVersion", VT_PTR,
			(unsigned int)&retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_RESPONSE, info, "s", "void");
	return retVal;
}

HttpHeader* HttpResponse::GetHeader(void) const
{
	typedef HttpHeader* (HttpResponse::*methodType)(void) const;
	static methodType GetHeaderp;
	HttpHeader* retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZNK5Tizen3Net4Http12HttpResponse9GetHeaderEv,
			GetHeaderp);
	retVal = (this->*GetHeaderp)();
	if (retVal == NULL)
		newerrno = 1;
	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::GetHeader", VT_PTR, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_ALLOCATION,
			info, "s", "void");
	return retVal;
}

Tizen::Base::ByteBuffer* HttpResponse::ReadBodyN(void)
{
	typedef Tizen::Base::ByteBuffer* (HttpResponse::*methodType)(void);
	static methodType ReadBodyNp;
	Tizen::Base::ByteBuffer* retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZN5Tizen3Net4Http12HttpResponse9ReadBodyNEv,
			ReadBodyNp);
	retVal = (this->*ReadBodyNp)();
	if (retVal == NULL)
		newerrno = 1;

	int bufferSize = retVal->GetLimit();
	Tizen::Base::ByteBuffer copybuffer;
	copybuffer.Construct(bufferSize + 4);
	copybuffer.CopyFrom(*retVal);

	char* out = (char*) (copybuffer.GetPointer());

	info.msg_buf = out;
	info.msg_pack_size = bufferSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : bufferSize;
	info.msg_total_size = bufferSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::ReadBodyN", VT_PTR,
			(unsigned int)&retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_RESPONSE, info, "s", "void");
	return retVal;
}

Tizen::Base::Collection::IList* HttpResponse::GetCookies(void) const
{
	typedef Tizen::Base::Collection::IList* (HttpResponse::*methodType)(
			void) const;
	static methodType GetCookiesp;
	Tizen::Base::Collection::IList* retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZNK5Tizen3Net4Http12HttpResponse10GetCookiesEv,
			GetCookiesp);

	retVal = (this->*GetCookiesp)();
	if (retVal == NULL)
		newerrno = 1;
	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::GetCookies", VT_PTR,
			(unsigned int)&retVal, (unsigned int)this, (unsigned int)this,
			HTTP_API_OTHER, info, "s", "void");
	return retVal;
}

result HttpResponse::SetStatusCode(NetHttpStatusCode statusCode)
{
	typedef result (HttpResponse::*methodType)(NetHttpStatusCode statusCode);
	static methodType SetStatusCodep;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http12HttpResponse13SetStatusCodeENS1_17NetHttpStatusCodeE,
			SetStatusCodep);

	retVal = (this->*SetStatusCodep)(statusCode);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::SetStatusCode", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"d", statusCode);
	return retVal;
}

result HttpResponse::SetHttpStatusCode(int statusCode)
{
	typedef result (HttpResponse::*methodType)(int statusCode);
	static methodType SetHttpStatusCodep;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http12HttpResponse17SetHttpStatusCodeEi,
			SetHttpStatusCodep);

	retVal = (this->*SetHttpStatusCodep)(statusCode);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::SetHttpStatusCode", VT_ULONG,
				  retVal, (unsigned int)this, (unsigned int)this,
				  HTTP_API_OTHER, info, "d", statusCode);
	return retVal;
}

result HttpResponse::SetStatusText(const Tizen::Base::String& statusText)
{
	typedef result (HttpResponse::*methodType)(
			const Tizen::Base::String& statusText);
	static methodType SetStatusText;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http12HttpResponse13SetStatusTextERKNS_4Base6StringE,
			SetStatusText);

	retVal = (this->*SetStatusText)(statusText);

	int nSize = statusText.GetLength();
	char temp[nSize];
	WcharToChar(temp, statusText.GetPointer());

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::SetStatusText", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"s", temp);
	return retVal;
}

result HttpResponse::SetVersion(const Tizen::Base::String& httpVersion)
{
	typedef result (HttpResponse::*methodType)(
			const Tizen::Base::String& httpVersion);
	static methodType SetVersionp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http12HttpResponse10SetVersionERKNS_4Base6StringE,
			SetVersionp);

	retVal = (this->*SetVersionp)(httpVersion);

	int nSize = httpVersion.GetLength();
	char temp[nSize];
	WcharToChar(temp, httpVersion.GetPointer());

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::SetVersion", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"s", temp);
	return retVal;
}

result HttpResponse::WriteBody(const Tizen::Base::ByteBuffer& body)
{
	typedef result (HttpResponse::*methodType)(
			const Tizen::Base::ByteBuffer& body);
	static methodType WriteBodyp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http12HttpResponse9WriteBodyERKNS_4Base10ByteBufferE,
			WriteBodyp);

	retVal = (this->*WriteBodyp)(body);

	int bufferSize = body.GetLimit();
	char* pBuffer = (char*) (body.GetPointer());
	Tizen::Base::String strData;
	strData.SetCapacity(bufferSize);
	strData.Append(pBuffer);
	char* out = new char[bufferSize];
	WcharToChar(out, strData.GetPointer());

	info.msg_buf = out;
	info.msg_pack_size = bufferSize > SOCKET_SEND_SIZE ? SOCKET_SEND_SIZE : bufferSize;
	info.msg_total_size = bufferSize;

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::WriteBody", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_RESPONSE,
			info, "x", (unsigned int)&body);
	delete out;
	return retVal;
}

result HttpResponse::Read(int headerLen, int bodyLen, int& rcvHeaderLen,
		int& rcvBodyLen) {
	typedef result (HttpResponse::*methodType)(int headerLen, int bodyLen,
			int& rcvHeaderLen, int& rcvBodyLen);
	static methodType Readp;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET( _ZN5Tizen3Net4Http12HttpResponse4ReadEiiRiS3_,
			Readp);

	retVal = (this->*Readp)(headerLen, bodyLen, rcvHeaderLen, rcvBodyLen);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::Read", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"ddxx",
			headerLen, bodyLen, (unsigned int)&rcvHeaderLen, (unsigned int)&rcvBodyLen);
	return retVal;
}

result HttpResponse::SetCookie(Tizen::Net::Http::HttpHeader* pHeader)
{
	typedef result (HttpResponse::*methodType)(
			Tizen::Net::Http::HttpHeader* pHeader);
	static methodType SetCookiep;
	result retVal;

	BEFORE_ORIGINAL_TIZEN_NET(
			_ZN5Tizen3Net4Http12HttpResponse9SetCookieEPNS1_10HttpHeaderE,
			SetCookiep);

	retVal = (this->*SetCookiep)(pHeader);

	AFTER_ORIGINAL_TIZEN_SOCK("HttpResponse::SetCookie", VT_ULONG, retVal,
			(unsigned int)this, (unsigned int)this, HTTP_API_OTHER, info,
			"x", (unsigned int)&pHeader);
	return retVal;
}

//
//HttpResponse::~HttpResponse(void){
//
//}
// HttpResponse

class _HttpCookieStorageManagerImpl;
class HttpCookieStorageManager {

public:
	result GetCookies(const Tizen::Base::String& url,
			Tizen::Base::String& cookies) const;
	result RemoveCookies(const Tizen::Base::String& url);
	result RemoveAllCookies();
private:
	HttpCookieStorageManager();
	~HttpCookieStorageManager();
	HttpCookieStorageManager(const HttpCookieStorageManager& rhs);
	HttpCookieStorageManager& operator =(const HttpCookieStorageManager& rhs);
private:
	friend class _HttpCookieStorageManagerImpl;
	_HttpCookieStorageManagerImpl* __pHttpCookieStorageManagerImpl;
};
// HttpCookieStorageManager

class _HttpRequestImpl;
class IHttpEntity {
public:
	virtual ~IHttpEntity(void) {
	}
public:
	virtual long long GetContentLength(void) const = 0;
	virtual Tizen::Base::String GetContentType(void) const = 0;
protected:
	virtual bool HasNextData(void) = 0;
	virtual Tizen::Base::ByteBuffer* GetNextDataN(int recommendedSize) = 0;
protected:
	virtual void IHttpEntity_Reserved1(void) {
	}
	virtual void IHttpEntity_Reserved2(void) {
	}
	virtual void IHttpEntity_Reserved3(void) {
	}
	friend class _HttpRequestImpl;
};

class IHttpTransactionEventListener {
public:
	virtual ~IHttpTransactionEventListener(void) {
	}
public:
	virtual void OnTransactionReadyToRead(HttpSession& httpSession,
			HttpTransaction& httpTransaction, int availableBodyLen) = 0;
	virtual void OnTransactionAborted(HttpSession& httpSession,
			HttpTransaction& httpTransaction, result r) = 0;
	virtual void OnTransactionReadyToWrite(HttpSession& httpSession,
			HttpTransaction& httpTransaction, int recommendedChunkSize) = 0;
	virtual void OnTransactionHeaderCompleted(HttpSession& httpSession,
			HttpTransaction& httpTransaction, int headerLen,
			bool bAuthRequired) = 0;
	virtual void OnTransactionCompleted(HttpSession& httpSession,
			HttpTransaction& httpTransaction) = 0;
	virtual void OnTransactionCertVerificationRequiredN(
			HttpSession& httpSession, HttpTransaction& httpTransaction,
			Tizen::Base::String* pCert) = 0;
	virtual bool OnTransactionCertVerificationRequestedN(
			HttpSession&, HttpTransaction&,
			Tizen::Base::Collection::IList* pCertList) {
		std::unique_ptr < Tizen::Base::Collection::IList
				> upCertList(pCertList);
		return false;
	}
protected:
	virtual void IHttpTransactionEventListener_Reserved1(void) {
	}

};

class IHttpProgressEventListener {
public:
	virtual ~IHttpProgressEventListener(void) {
	}
public:
	virtual void OnHttpUploadInProgress(HttpSession& httpSession,
			HttpTransaction& httpTransaction, long long currentLength,
			long long totalLength) = 0;
	virtual void OnHttpDownloadInProgress(HttpSession& httpSession,
			HttpTransaction& httpTransaction, long long currentLength,
			long long totalLength) = 0;
protected:
	virtual void IHttpProgressEventListener_Reserved1(void) {
	}
	virtual void IHttpProgressEventListener_Reserved2(void) {
	}
	virtual void IHttpProgressEventListener_Reserved3(void) {
	}
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
// INetConnectionEventListener

}
}
} // namespace Tizen { namespace Net { namespace Http
