#include "net/tcpserver.h"
#include "net/eventloop.h"

const short LISTEN_PORT = 30606;

int main(int argc, char* argv[])
{
	EventLoop loop;
	InetAddr srvaddr(LISTEN_PORT);
	
	TCPServer s(&loop, srvaddr);
	s.start();
	loop.loop();
	
	return 0;
}
