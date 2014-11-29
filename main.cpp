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

int setNonBlocking(int sockfd)
{
	if (0 > ::fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK)) {
		return -1;
	}
	return 0;
}

int createListenSocket()
{
	int listen = ::socket(AF_INET, SOCK_STREAM, 0);
	if (0 > listen) {
		return listen;
	}
	struct sockaddr_in srvaddr;
	bzero(&srvaddr, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(LISTEN_PORT);
	
	if (0 > ::bind(listen, (struct sockaddr *)(&srvaddr), sizeof(srvaddr))) {
		return -1;
	}
	if (0 > ::listen(listen, MAX_LISTEN)) {
		return -1;
	}
	
	return listen;
}

int main(int argc, char* argv[])
{
	int listener, connfd, epfd, sockfd;
	int readsize = 0;
	char buf[BUFFER_SIZE];
	struct sockaddr_in local;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	struct epoll_event ev;
	struct epoll_event events[MAX_EPOLL_SIZE];
	
	listener = createListenSocket();
	epfd = ::epoll_create(MAX_EPOLL_SIZE);
	if (0 > epfd) {
		fprintf(stderr, "epoll create error: fd=%d\n", epfd);
		exit(0);
	}
	ev.events = EPOLLIN | EPOLLET;	// 设置epoll模式
	ev.data.fd = listener;
	if (0 > epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &ev)) {
		fprintf(stderr, "epoll set insertion error: fd=%d\n", listener);
		exit(0);
	}
	
	for (;;) {
		int nfds = ::epoll_wait(epfd, events, MAX_EPOLL_SIZE, -1);
		
		for (int i=0; i<nfds; ++i) {
			if (events[i].data.fd == listener) {
				connfd = ::accept(listener, (struct sockaddr *)&local, &addrlen);
				if (0 > connfd) {
					perror("accept");
					continue;
				}
				cout << "new connection from " 
					<< "[" << inet_ntoa(local.sin_addr) 
					<< ":" << ntohs(local.sin_port) << "]" 
					<< " accept socket fd:" << connfd 
					<< endl;
				setNonBlocking(connfd);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = connfd;
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev) < 0) {
					fprintf(stderr, "epoll set insertion error: fd=%d\n", connfd);
					return -1;
				}
			} else if (events[i].events & EPOLLIN) {
				// 有读事件
				if (0 > (sockfd = events[i].data.fd)) {
					cout << "epoll read fd error" << endl;
					continue;
				}
				memset(&buf, 0, BUFFER_SIZE);
				readsize = ::read(sockfd, buf, BUFFER_SIZE);
				if (0 >= readsize) {
					::close(sockfd);
				} else {
					if (::write(sockfd, buf, readsize) != readsize) {
						cout << "error: not finished one time" << endl;
                    }
				}
			}
		}
   }
	
	return 0;
}
