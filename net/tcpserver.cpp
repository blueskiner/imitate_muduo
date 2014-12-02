#include "net/tcpserver.h"
#include "net/tcpconnection.h"

#include "glog/logging.h"

#include <boost/bind.hpp>

TCPServer::TCPServer(EventLoop* loop, const InetAddr& addr)
	: _loop(loop),
	_acceptor(loop, addr)
{
	_acceptor.setNewConnectionCallback(boost::bind(&TCPServer::newConnection, this, _1, _2));
}

TCPServer::~TCPServer()
{
}

void TCPServer::start()
{
	_acceptor.start();
}

void TCPServer::setConnectionCallback(const ConnectionCallback& cb)
{
	_connectionCallback = cb;
}

void TCPServer::setMessageCallback(const MessageCallback& cb)
{
	_messageCallback = cb;
}

////////////////////////////////////////////////////////////

void TCPServer::newConnection(int sockfd, const InetAddr& peerAddr)
{
	LOG(INFO) << "TCPServer::newConnection"
		<< " from " << peerAddr.toIP()
		<< ":" << peerAddr.toPort();
	
	TCPConnectionPtr conn(new TCPConnection(_loop, sockfd));
	conn->setConnectionCallback(_connectionCallback);
	conn->setMessageCallback(_messageCallback);
	conn->setWriteCompleteCallback(_writeCompleteCallback);
}
