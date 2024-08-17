#ifndef SOCKETOPTION_H__
#define SOCKETOPTION_H__

namespace cid
{
	enum class SocketOption : int
	{
		TCP_NoDelay //disable Nagle's algorithm
	};
}

#endif
