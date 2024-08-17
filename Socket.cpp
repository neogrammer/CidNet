#include "Socket.h"
#include <assert.h>
#include <iostream>

cid::CResult cid::Socket::SetSocketOption(SocketOption option_, BOOL value_)
{
	int result = 0;
	switch (option_)
	{
	case SocketOption::TCP_NoDelay:
		result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char*)&value_, sizeof(value_));
		break;
	default:
		CResult::C_NotYetImplemented;
		break;
	}

	if (result != 0)
	{
		int error = WSAGetLastError();
		return CResult::C_NotYetImplemented;
	}

	return CResult::C_Success;
}

cid::Socket::Socket(IPVersion ipVersion_, SocketHandle handle_)
	: ipVersion{ipVersion_}
	, handle{handle_}
{
	assert(ipVersion == IPVersion::IPv4);
}

cid::CResult cid::Socket::Create()
{
	assert(ipVersion == IPVersion::IPv4);

	if (handle != INVALID_SOCKET)
	{
		return CResult::C_NotYetImplemented;
	}

	handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (handle == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		return CResult::C_NotYetImplemented;
	}

	if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != CResult::C_Success)
	{
		return CResult::C_NotYetImplemented;
	}

	return CResult::C_Success;



}

cid::CResult cid::Socket::Close()
{
	if (handle == INVALID_SOCKET)
	{
		return cid::CResult::C_NotYetImplemented;
	}

	int result = closesocket(handle);
	if (result != 0)
	{
		int error = WSAGetLastError();
		return cid::CResult::C_NotYetImplemented;
	}

	handle = INVALID_SOCKET;

	return cid::CResult::C_Success;
}

cid::CResult cid::Socket::Bind(IPEndpoint endpoint_)
{
	sockaddr_in addr = endpoint_.GetIPv4();
	int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
	if (result != 0)  // if an error occurred
	{
		int error = WSAGetLastError();
		return CResult::C_NotYetImplemented;
	}
	return CResult::C_Success;
}

cid::CResult cid::Socket::Listen(IPEndpoint endpoint_, int backlog_)
{
	if (Bind(endpoint_) != CResult::C_Success)
	{
		return CResult::C_NotYetImplemented;
	}

	int result = listen(handle, backlog_);
	if (result != 0)
	{
		int error = WSAGetLastError();
		return CResult::C_NotYetImplemented;
	}

	return CResult::C_Success;
}

cid::SocketHandle cid::Socket::GetHandle()
{
	return handle;
}

cid::IPVersion cid::Socket::GetIPVersion()
{
	return ipVersion;
}

cid::CResult cid::Socket::Accept(Socket& outSocket)
{
	sockaddr_in addr = {};
	int len = sizeof(sockaddr_in);
	SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len);
	if (acceptedConnectionHandle == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		return CResult::C_NotYetImplemented;
	}
	IPEndpoint newConnectionEndpoint((sockaddr*)&addr);
	std::cout << "New connection accepted" << std::endl;
	newConnectionEndpoint.Print();
	outSocket = Socket(IPVersion::IPv4, acceptedConnectionHandle);

	return CResult::C_Success;
}

cid::CResult cid::Socket::Connect(IPEndpoint endpoint_)
{
	sockaddr_in addr = endpoint_.GetIPv4();
	int result = connect(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
	if (result != 0) // if an error occurred
	{
		int error = WSAGetLastError();
		return CResult::C_NotYetImplemented;
	}
	return CResult::C_Success;
}

cid::CResult cid::Socket::Recv(void* dest_, int numBytes_, int& bytesReceived_)
{
	bytesReceived_ = recv(handle, (char*)dest_, numBytes_, NULL);
	if (bytesReceived_ == 0)
	{
		return CResult::C_NotYetImplemented;
	}
	if (bytesReceived_ == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		return CResult::C_NotYetImplemented;
	}
	
	return CResult::C_Success;
}

cid::CResult cid::Socket::RecvAll(void* dest_, int numBytes_)
{
	int totalReceived = 0;
	while (totalReceived < numBytes_)
	{
		int bytesRemaining = numBytes_ - totalReceived;
		int bytesReceived = 0;
		char* bufferOffset = (char*)dest_ + totalReceived;
		CResult result = Recv(bufferOffset, bytesRemaining, bytesReceived);
		if (result != CResult::C_Success)
		{
			return CResult::C_NotYetImplemented;
		}
		totalReceived += bytesReceived;
	}

	return CResult::C_Success;
}

cid::CResult cid::Socket::SendAll(void* data, int numBytes)
{
	int totalSent = 0;
	while (totalSent < numBytes)
	{
		int bytesRemaining = numBytes - totalSent;
		int bytesSent = 0;
		char* bufferOffset = (char*)data + totalSent;
		CResult result = Send(bufferOffset, bytesRemaining, bytesSent);
		if (result != CResult::C_Success)
		{
			return CResult::C_NotYetImplemented;
		}
		totalSent += bytesSent;
	}
	return CResult::C_Success;
}

cid::CResult cid::Socket::Send(void* data, int numBytes, int& bytesSent)
{
	//int totalSent = 0;
	//while (totalSent < numBytes)
	//{
		bytesSent = send(handle, (const char*)data, numBytes, NULL);
		if (bytesSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return CResult::C_NotYetImplemented;
		}
//		totalSent += bytesSent;
	//}

	return CResult::C_Success;
}
