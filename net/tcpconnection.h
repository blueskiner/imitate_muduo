#ifndef __NET_TCP_CONNECTION_H__
#define __NET_TCP_CONNECTION_H__

#include "net/callbacks.h"
#include "net/buffer.h"

#include <string>

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

class Socket;
class Channel;
class EventLoop;
class TCPConnection : boost::noncopyable, public boost::enable_shared_from_this<TCPConnection>
{
public:
	TCPConnection(EventLoop* loop, int sockfd);
	~TCPConnection();
	
	// ·¢ËÍÊý¾Ý
	void send(const void* message, int len);
	void send(const std::string& message);
	void send(Buffer* message);
	
	
	void setConnectionCallback(const ConnectionCallback& cb);
	void setMessageCallback(const MessageCallback& cb);
	void setWriteCompleteCallback(const WriteCompleteCallback& cb);
	void setCloseCallback(const CloseCallback& cb);
	
private:
	void handleRead();
	void handleWrite();
	void handleClose();
	void handleError();
	
private:
	EventLoop* _loop;
	
	boost::scoped_ptr<Socket> _socket;
	boost::scoped_ptr<Channel> _channel;
	
	ConnectionCallback _connectionCallback;
	MessageCallback _messageCallback;
	WriteCompleteCallback _writeCompleteCallback;
	CloseCallback _closeCallback;
	
	Buffer _iBuffer;
	Buffer _oBuffer;
};

typedef boost::shared_ptr<TCPConnection> TCPConnectionPtr;

#endif	// __NET_TCP_CONNECTION_H__
