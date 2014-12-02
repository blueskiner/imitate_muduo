#ifndef __NET_CALL_BACKS_H__
#define __NET_CALL_BACKS_H__

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

class Buffer;
class TCPConnection;
typedef boost::shared_ptr<TCPConnection> TCPConnectionPtr;

typedef boost::function<void()> TimerCallback;

typedef boost::function<void (const TCPConnectionPtr&)> ConnectionCallback;
typedef boost::function<void (const TCPConnectionPtr&)> CloseCallback;
typedef boost::function<void (const TCPConnectionPtr&)> WriteCompleteCallback;
typedef boost::function<void (const TCPConnectionPtr&, Buffer *)> MessageCallback;

void testConnectionCallback(int sockfd);

#endif	// __NET_CALL_BACKS_H__
