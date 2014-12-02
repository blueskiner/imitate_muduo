#include "net/tcpserver.h"
#include "net/tcpconnection.h"
#include "net/eventloop.h"

#include "glog/logging.h"

#include <boost/bind.hpp>

const short LISTEN_PORT = 30606;


class TestServer
{
public:
	TestServer(EventLoop* loop, const InetAddr& listenAddr)
		: _server(loop, listenAddr),
		numConnected_(0)
	{
		_server.setConnectionCallback(boost::bind(&TestServer::onConnection, this, _1));
		_server.setMessageCallback(boost::bind(&TestServer::onMessage, this, _1, _2));
	}
	
	void start() {
		_server.start();
	}
	
private:
	void onConnection(const TCPConnectionPtr& conn) {
		
//		if (conn->connected()) {
//			++numConnected_;
//			if (numConnected_ > kMaxConnections_) {
//				conn->shutdown();
//				conn->forceCloseWithDelay(3.0);  // > round trip of the whole Internet.
//			}
//		} else {
//			--numConnected_;
//		}
		LOG(INFO) << "numConnected = " << numConnected_;
	}
	
	void onMessage(const TCPConnectionPtr& conn, Buffer* buf) {
		LOG(INFO) << "TestServer::onMessage";
//		string msg(buf->retrieveAllAsString());
//		LOG(INFO) << conn->name() << " echo " << msg.size() << " bytes at " << time.toString();
//		conn->send(msg);
	}
	
	TCPServer _server;
	int numConnected_; // should be atomic_int
//	const int kMaxConnections_;
};

int main(int argc, char* argv[])
{
	EventLoop loop;
	InetAddr srvaddr(LISTEN_PORT);
	
	TestServer s(&loop, srvaddr);
	s.start();
	loop.loop();
	
	return 0;
}
