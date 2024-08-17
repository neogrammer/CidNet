#ifndef IPENDPOINT_H__
#define IPENDPOINT_H__
#include <string>
#include <vector>
#include <iostream>
#include <WS2tcpip.h>
#include "IPVersion.h"


namespace cid
{

	class IPEndpoint
	{
		IPVersion ipVersion{ IPVersion::Unknown };
		std::string hostname{ "" };
		std::string ipStr{ "" };
		unsigned short port{ 55555 };
		std::vector<uint8_t> bytes{};

	public:
		IPEndpoint(const char* ip_, unsigned short port_);
		IPEndpoint(sockaddr* addr);
		std::vector<uint8_t> GetBytes();
		std::string  GetHostname();
		std::string GetIPStr();
		IPVersion GetIPVer();
		sockaddr_in GetIPv4();
		unsigned short GetPort();
		void Print();
	};
}

#endif
