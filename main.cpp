#include "net/socket.h"

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>		// bzero
#include <sys/socket.h>	// basic socket definitions
#include <netinet/in.h>	// sockaddr_in{} and other Internet defns
#include <arpa/inet.h>	// inet(3) functions
#include <sys/epoll.h>	// epoll function
#include <fcntl.h>		// nonblocking

#include <iostream>
using namespace std;

const short LISTEN_PORT = 30606;
const int MAX_LISTEN = 10;
const int MAX_EPOLL_SIZE = 10;
const int BUFFER_SIZE = 256;

int main(int argc, char* argv[])
{
	InetAddr srvaddr(30606);
	InetAddr cliaddr;
	
	Socket listenSock;
	Socket connSock;
	listenSock.create();
	listenSock.bind(srvaddr);
	listenSock.listen();
	
	int epfd;
	int readsize = 0;
	char buf[BUFFER_SIZE];
	
	struct epoll_event ev;
	struct epoll_event events[MAX_EPOLL_SIZE];
	
	epfd = ::epoll_create(MAX_EPOLL_SIZE);
	if (0 > epfd) {
		fprintf(stderr, "epoll create error: fd=%d\n", epfd);
		exit(0);
	}
	ev.events = EPOLLIN | EPOLLET;	// 设置epoll模式
	ev.data.fd = listenSock.fd();
	if (0 > epoll_ctl(epfd, EPOLL_CTL_ADD, listenSock.fd(), &ev)) {
		fprintf(stderr, "epoll set insertion error: fd=%d\n", listenSock.fd());
		exit(0);
	}
	
	for (;;) {
		int nfds = ::epoll_wait(epfd, events, MAX_EPOLL_SIZE, -1);
		
		for (int i=0; i<nfds; ++i) {
			if (events[i].data.fd == listenSock.fd()) {
				connSock = listenSock.accept(&cliaddr);
				
				cout << "new connection from "
					<< "[" << cliaddr.toIP()
					<< ":" << cliaddr.toPort() << "]"
					<< " accept socket fd:" << connSock.fd()
					<< endl;
				connSock.setNonBlock();
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = connSock.fd();
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, connSock.fd(), &ev) < 0) {
					fprintf(stderr, "epoll set insertion error: fd=%d\n", connSock.fd());
					return -1;
				}
			} else if (events[i].events & EPOLLIN) {
				// 有读事件
				if (0 > (connSock.fd() == events[i].data.fd)) {
					cout << "epoll read fd error" << endl;
					continue;
				}
				memset(&buf, 0, BUFFER_SIZE);
				readsize = ::read(connSock.fd(), buf, BUFFER_SIZE);
				if (0 >= readsize) {
					connSock.close();
				} else {
					if (::write(connSock.fd(), buf, readsize) != readsize) {
						cout << "error: not finished one time" << endl;
                    }
				}
			}
		}
	}
	
	return 0;
}
