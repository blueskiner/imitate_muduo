#include "net/inetaddr.h"

#include "glog/logging.h"

#include <arpa/inet.h>	// inet(3) functions
#include <string.h>		// bzero

InetAddr::InetAddr(uint16_t port)
{
	bzero(&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
//	in_addr_t ip = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
	_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	_addr.sin_port = htons(port);
}

InetAddr::InetAddr(const char* ip, uint16_t port)
{
	bzero(&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	// ×ª»»×Ö·û´®µ½ÍøÂçµØÖ·
	if (::inet_pton(AF_INET, ip, &_addr.sin_addr) <= 0) {
		LOG(ERROR) << "IPAddr Constructor Error";
	}
}

InetAddr::~InetAddr()
{
}

const char* InetAddr::toIP() const
{
	return ::inet_ntoa(_addr.sin_addr);
}

const char* InetAddr::toIPPort() const
{
	// TODO
	return NULL;
}

uint16_t InetAddr::toPort() const
{
	return ntohs(_addr.sin_port);
}

const struct sockaddr_in& InetAddr::getSockAddrInet() const
{
	return _addr;
}

void InetAddr::setSockAddrInet(const struct sockaddr_in& addr)
{
	_addr = addr;
}