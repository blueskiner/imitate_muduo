#include "net/channel.h"
#include "net/callbacks.h"
#include "net/eventloop.h"

#include "glog/logging.h"

#include <sys/epoll.h>
#include <errno.h>

Channel::Channel(EventLoop* loop, int fd)
	: _loop(loop),
	_sockfd(fd),
	_events(0),
	_revents(0)
{
}

Channel::~Channel()
{
}

void Channel::handleEvent()
{
	// 事件分发处理
	if(_revents & EPOLLIN) {
		if (_readEventCallback) {
			_readEventCallback();
		}
	}
	
	if (_revents & EPOLLOUT) {
	}
}

void Channel::setReadCallback(const ReadEventCallback& cb)
{
	_readEventCallback = cb;
}
void Channel::setWriteCallback(const EventCallback& cb)
{
	_writeEventCallback = cb;
}
void Channel::setCloseCallback(const EventCallback& cb)
{
	_closeEventCallback = cb;
}
void Channel::setErrorCallback(const EventCallback& cb)
{
	_errorCallback = cb;
}

int Channel::getEvents() const
{
	return _events;
}

void Channel::setRevents(int revents)
{
	_revents = revents;
}

void Channel::enableReading()
{
	LOG(INFO) << "Channel::enableReading";
	_events |= EPOLLIN;
	update();
}

void Channel::disableReading()
{
	_events &= ~EPOLLIN;
	update();
}

void Channel::enableWriting()
{
	LOG(INFO) << "Channel::enableWriting";
	_events |= EPOLLOUT;
	update();
}

void Channel::disableWriting()
{
}

int Channel::fd() const
{
	return _sockfd;
}

////////////////////////////////////////////////////////////

void Channel::update()
{
	_loop->updateChannel(this);
	/*
	struct epoll_event ev;
	ev.events = _events;	// 设置epoll模式
	ev.data.fd = _sockfd;
	int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, _sockfd, &ev);
	if (0 > ret) {
		int e = errno;
		switch (e) {
		case EBADF:LOG(ERROR)<<"A";break;
		case EEXIST:LOG(ERROR)<<"B";break;
		case EINVAL:LOG(ERROR)<<"C";break;
		case ENOENT:LOG(ERROR)<<"D";break;
		case ENOMEM:LOG(ERROR)<<"E";break;
		case ENOSPC:LOG(ERROR)<<"F";break;
		case EPERM:LOG(ERROR)<<"G";break;
		}
		//LOG(ERROR) << "epoll set insertion error: fd==" << _sockfd << " error:" << errno;
	}*/
}
