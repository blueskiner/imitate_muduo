#ifndef __NET_ACCEPTOR_H__
#define __NET_ACCEPTOR_H__

#include "net/socket.h"
#include "net/channel.h"

#include <boost/function.hpp>

class EventLoop;
class Acceptor
{
public:
	typedef boost::function<void (int sockfd, const InetAddr&)> NewConnectionCallback;
	
	Acceptor(EventLoop* loop, const InetAddr& addr);
	~Acceptor();
	
	void setNewConnectionCallback(const NewConnectionCallback& cb);
	void start();
	
private:
	void handleAccept();
	
private:
	EventLoop* _loop;
	Socket _sock;
	Channel _acceptChannel;
	
	NewConnectionCallback _newConnectionCallback;
};


#endif	// __NET_ACCEPTOR_H__
