#ifndef __NET_TCP_SERVER_H__
#define __NET_TCP_SERVER_H__

#include "net/callbacks.h"
#include "net/acceptor.h"

class EventLoop;
class TCPServer
{
public:
	TCPServer(EventLoop* loop, const InetAddr& addr);
	~TCPServer();
	
	void start();
	
	// ���ⲿ�ص�ʹ��
	void setConnectionCallback(const ConnectionCallback& cb);
	void setMessageCallback(const MessageCallback& cb);
	
private:
	void newConnection(int sockfd, const InetAddr& peerAddr);
	
private:
	EventLoop* _loop;
	Acceptor _acceptor;	// ������
	
	ConnectionCallback _connectionCallback;
	MessageCallback _messageCallback;
	WriteCompleteCallback _writeCompleteCallback;
};

#endif	// __NET_TCP_SERVER_H__
