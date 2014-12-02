#include "net/tcpconnection.h"
#include "net/socket.h"
#include "net/channel.h"

#include "glog/logging.h"

#include <boost/bind.hpp>

TCPConnection::TCPConnection(EventLoop* loop, int sockfd)
	: _loop(loop),
	_socket(new Socket(sockfd)),
	_channel(new Channel(loop, sockfd))
{
	_channel->setReadCallback(boost::bind(&TCPConnection::handleRead, this));
	_channel->setWriteCallback(boost::bind(&TCPConnection::handleWrite, this));
	_channel->setCloseCallback(boost::bind(&TCPConnection::handleClose, this));
	_channel->setErrorCallback(boost::bind(&TCPConnection::handleError, this));
}

TCPConnection::~TCPConnection()
{
}

void TCPConnection::send(const void* message, int len)
{
}

void TCPConnection::send(const std::string& message)
{
}

void TCPConnection::send(Buffer* message)
{
}

void TCPConnection::setConnectionCallback(const ConnectionCallback& cb)
{
	_connectionCallback = cb;
}

void TCPConnection::setMessageCallback(const MessageCallback& cb)
{
	_messageCallback = cb;
}

void TCPConnection::setWriteCompleteCallback(const WriteCompleteCallback& cb)
{
	_writeCompleteCallback = cb;
}

void TCPConnection::setCloseCallback(const CloseCallback& cb)
{
	_closeCallback = cb;
}

////////////////////////////////////////////////////////////

void TCPConnection::handleRead()
{
	LOG(INFO) << "TCPConnection::handleRead";
}
void TCPConnection::handleWrite()
{
}
void TCPConnection::handleClose()
{
}
void TCPConnection::handleError()
{
}
