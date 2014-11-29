#ifndef __NET_INET_ADDR_H__
#define __NET_INET_ADDR_H__

#include <netinet/in.h>	// sockaddr_in{} and other Internet defns

class InetAddr
{
public:
	explicit InetAddr(uint16_t port = 0);
	// "1.2.3.4"
	InetAddr(const char* ip, uint16_t port);
	InetAddr(const struct sockaddr_in& addr) : _addr(addr) { }
	
	~InetAddr();
	
	const char* toIP() const;
	const char* toIPPort() const;
	uint16_t toPort() const;
	
	const struct sockaddr_in& getSockAddrInet() const;
	void setSockAddrInet(const struct sockaddr_in& addr);
	
private:
	struct sockaddr_in _addr;
};

#endif	// __NET_INET_ADDR_H__
