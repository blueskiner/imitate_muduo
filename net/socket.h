#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__

#include "net/inetaddr.h"

// 可以拷贝
class Socket
{
public:
	Socket();
	Socket(int fd);
	Socket(const Socket& other);
	
	~Socket();
	
	Socket& operator = (const Socket& other);
	Socket& operator = (const int sockfd);
	
	int fd() const;
	
	// 创建socket
	void create();
	
	void bind(const InetAddr& addr);
	void listen();
	void close();
	
	int accept(InetAddr* peeraddr);
	
public:
	///
	/// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
	///
	void setTcpNoDelay(bool on);
	
	///
	/// Enable/disable SO_REUSEADDR
	///
	void setReuseAddr(bool on);
	
	///
	/// Enable/disable SO_REUSEPORT
	///
	void setReusePort(bool on);
	
	///
	/// Enable/disable SO_KEEPALIVE
	///
	void setKeepAlive(bool on);
	
	void setNonBlock();
	
private:
	int _fd;
};

#endif	// __NET_SOCKET_H__
