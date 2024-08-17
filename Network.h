#ifndef NETWORK_H__
#define NETWORK_H__
#define WIN32_LEAN_AND_MEAN
#include <Winsock2.h>
#include <stdexcept>
#include <iostream>

namespace cid
{
	namespace net
	{
		static bool initialize()
		{
			WSADATA version;
			int result = WSAStartup(MAKEWORD(2, 2), &version);
			if (result != 0)
			{
				//startup failure
				std::cout << "startup failure: ERROR: " << WSAGetLastError() << std::endl;

			}
			else if (LOBYTE(version.wVersion) != 2 || HIBYTE(version.wVersion) != 2)
			{
				// wrong version obtained
				std::cout << "wrong version of Winsock oobtained" << std::endl;
			}
			else
			{
				// startup success
				std::cout << "startup success\n" << std::endl;
				return true;
			}

			return false;
		};

		static void shutdown()
		{
			WSACleanup();
		};
	}

}

#endif
