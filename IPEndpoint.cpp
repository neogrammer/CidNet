#include "IPEndpoint.h"
#include <assert.h>

cid::IPEndpoint::IPEndpoint(const char* ip_, unsigned short port_)
	: hostname{ ip_ }
	, port{ port_ }
{
	in_addr addr;
	int result = inet_pton(AF_INET, ip_, &addr);
	if (result == 1)
	{
		if (addr.S_un.S_addr != INADDR_NONE)
		{
			ipStr = ip_;
			hostname = ip_;

			bytes.resize(sizeof(ULONG));
			memcpy(&bytes[0], &addr.S_un.S_addr, sizeof(ULONG));

			ipVersion = IPVersion::IPv4;
			return;
		}
	}

	// attempt to resolve hostname to ipaddress
	addrinfo hints = {};
	hints.ai_family = AF_INET; //ipv4 only
	addrinfo* hostinfo = nullptr;
	result = getaddrinfo(ip_, NULL, &hints, &hostinfo);
	if (result == 0)
	{
		sockaddr_in* hostAddr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);

		//hostAddr->sin_addr.S_un.S_addr
		ipStr.resize(16);
		inet_ntop(AF_INET, &hostAddr->sin_addr, &ipStr[0], 16);

		hostname = ip_;

		ULONG ipLong = hostAddr->sin_addr.S_un.S_addr;
		bytes.resize(sizeof(ULONG));
		memcpy(&bytes[0], &ipLong, sizeof(ULONG));

		ipVersion = IPVersion::IPv4;
		freeaddrinfo(hostinfo);
		return;
	}
}

cid::IPEndpoint::IPEndpoint(sockaddr* addr)
{
	assert(addr->sa_family == AF_INET);
	sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(addr);
	ipVersion = IPVersion::IPv4;
	port = ntohs(addrv4->sin_port);
	bytes.resize(sizeof(ULONG));
	memcpy(&bytes[0], &addrv4->sin_addr, sizeof(ULONG));
	ipStr.resize(16);
	inet_ntop(AF_INET, &addrv4->sin_addr, &ipStr[0], 16);
	hostname = ipStr;
}

std::vector<uint8_t> cid::IPEndpoint::GetBytes()
{
	return bytes;
}

std::string cid::IPEndpoint::GetHostname()
{
	return hostname;
}

std::string cid::IPEndpoint::GetIPStr()
{
	return ipStr;
}

cid::IPVersion cid::IPEndpoint::GetIPVer()
{
	return ipVersion;
}

sockaddr_in cid::IPEndpoint::GetIPv4()
{
	assert(ipVersion == IPVersion::IPv4);
	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	memcpy(&addr.sin_addr, &bytes[0], sizeof(ULONG));
	addr.sin_port = htons(this->port);
	return addr;
}
unsigned short cid::IPEndpoint::GetPort()
{
	return port;
}

void cid::IPEndpoint::Print()
{
	switch (ipVersion)
	{
	case IPVersion::IPv4:
		std::cout << "IP Version: IPv4" << std::endl;
		break;
	case IPVersion::IPv6:
		std::cout << "IP Version: IPv6" << std::endl;
		break;
	default:
		std::cout << "IP Version: Unknown" << std::endl;
		break;
	}
	std::cout << "Hostname: " << hostname << std::endl;
	std::cout << "IP: " << ipStr << std::endl;
	std::cout << "Port: " << port << std::endl;
	std::cout << "IP bytes... "<< std::endl;
	for (auto& digit : bytes)
	{
		std::cout << (int)digit << std::endl;
	}
}
