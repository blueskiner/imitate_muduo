#include "net/tcpserver.h"

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

////////////////////////////////////////////////////////////

void TCPServer::newConnection(int sockfd, const InetAddr& peerAddr)
{
	LOG(INFO) << "TCPServer::newConnection"
		<< " from " << peerAddr.toIP()
		<< ":" << peerAddr.toPort();
}
