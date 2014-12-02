#include "net/acceptor.h"

#include "glog/logging.h"

#include <boost/bind.hpp>

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>		// bzero
#include <sys/epoll.h>	// epoll function
#include <iostream>
using namespace std;

const int MAX_LISTEN = 10;/*
const int MAX_EPOLL_SIZE = 10;
const int BUFFER_SIZE = 256;
*/
Acceptor::Acceptor(EventLoop* loop, const InetAddr& addr)
	: _loop(loop),
	_sock(),
	_acceptChannel(loop, _sock.fd())
{
	_sock.bind(addr);
	
	_acceptChannel.setReadCallback(boost::bind(&Acceptor::handleAccept, this));
}

Acceptor::~Acceptor()
{
}

void Acceptor::setNewConnectionCallback(const NewConnectionCallback& cb)
{
	_newConnectionCallback = cb;
}

void Acceptor::start()
{
	LOG(INFO) << "Acceptor::start";
	_sock.listen(MAX_LISTEN);
	_acceptChannel.enableReading();
	/*
	InetAddr peerAddr;
	Socket connSock;
	
	int readsize = 0;
	char buf[BUFFER_SIZE];
	
	struct epoll_event ev;
	struct epoll_event events[MAX_EPOLL_SIZE];
	
	_epfd = ::epoll_create(MAX_EPOLL_SIZE);
	if (0 > _epfd) {
		LOG(ERROR) << "epoll create error: fd=" << _epfd;
		exit(0);
	}
	ev.events = EPOLLIN | EPOLLET;	// 设置epoll模式
	ev.data.fd = _sock.fd();
	if (0 > ::epoll_ctl(_epfd, EPOLL_CTL_ADD, _sock.fd(), &ev)) {
		LOG(ERROR) << "epoll set insertion error: fd=" << _sock.fd();
		exit(0);
	}
	
	for (;;) {
		int nfds = ::epoll_wait(_epfd, events, MAX_EPOLL_SIZE, -1);
		
		for (int i=0; i<nfds; ++i) {
			if (events[i].data.fd == _sock.fd()) {
				connSock = _sock.accept(&peerAddr);
				
				LOG(INFO) << "new connection from "
					<< "[" << peerAddr.toIP()
					<< ":" << peerAddr.toPort() << "]"
					<< " accept socket fd:" << connSock.fd();
				connSock.setNonBlock();
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = connSock.fd();
				if (0 > ::epoll_ctl(_epfd, EPOLL_CTL_ADD, connSock.fd(), &ev)) {
					LOG(ERROR) << "epoll set insertion error: fd=" << connSock.fd();
					return;
				}
			} else if (events[i].events & EPOLLIN) {
				// 有读事件
				if (0 > (connSock.fd() == events[i].data.fd)) {
					LOG(ERROR) << "epoll read fd error";
					continue;
				}
				memset(&buf, 0, BUFFER_SIZE);
				readsize = ::read(connSock.fd(), buf, BUFFER_SIZE);
				if (0 >= readsize) {
					connSock.close();
				} else {
					if (::write(connSock.fd(), buf, readsize) != readsize) {
						LOG(ERROR) << "error: not finished one time";
                    }
				}
			}
		}
	}*/
}

void Acceptor::handleAccept()
{
	InetAddr peerAddr;
	LOG(INFO) << "Acceptor::handleAccept";
	int connfd = _sock.accept(&peerAddr);
	if (0 <= connfd) {
		if (_newConnectionCallback) {
			_newConnectionCallback(connfd, peerAddr);
		} else {
			_sock.close();
		}
	} else {
		LOG(ERROR) << "Acceptor::handleAccept";
		if (EMFILE == errno) {
		}
	}
}
