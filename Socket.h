#ifndef SOCKET_H__
#define SOCKET_H__

#include "SocketHandle.h"
#include "CResult.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndpoint.h"

namespace cid
{
	class Socket
	{
		IPVersion ipVersion{ IPVersion::IPv4 };
		SocketHandle handle{ INVALID_SOCKET };
		CResult SetSocketOption(SocketOption option_, BOOL value_);
	public:
		Socket(IPVersion ipVersion_ = IPVersion::IPv4, SocketHandle handle_ = INVALID_SOCKET);

		CResult Create();
		CResult Close();
		CResult Bind(IPEndpoint endpoint_);
		CResult Listen(IPEndpoint endpoint_, int backlog_ = 5);

		SocketHandle GetHandle();
		IPVersion GetIPVersion();
		CResult Accept(Socket& outSocket);
		CResult Connect(IPEndpoint endpoint_);
		CResult Recv(void* dest_, int numBytes_, int& bytesReceived_);
		CResult RecvAll(void* dest_, int numBytes_);
		CResult SendAll(void* data, int numBytes);

		CResult Send(void* data, int numBytes, int& bytesSent);
	};
}

#endif