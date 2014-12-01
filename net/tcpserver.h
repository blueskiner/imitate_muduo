#ifndef __NET_TCP_SERVER_H__
#define __NET_TCP_SERVER_H__

#include "net/acceptor.h"

class EventLoop;
class TCPServer
{
public:
	TCPServer(EventLoop* loop, const InetAddr& addr);
	~TCPServer();
	
	void start();
	
private:
	void newConnection(int sockfd, const InetAddr& peerAddr);
	
private:
	EventLoop* _loop;
	Acceptor _acceptor;	// ½ÓÊÕÆ÷
};

#endif	// __NET_TCP_SERVER_H__
