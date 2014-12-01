#include "net/channel.h"
#include "net/callbacks.h"

#include "glog/logging.h"

#include <sys/epoll.h>

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
	if(_revents & EPOLLIN) {
		if (_readEventCallback) {
			_readEventCallback();
		}
	}
}

void Channel::setReadCallback(const ReadEventCallback& cb)
{
	_readEventCallback = cb;
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
	_events |= EPOLLIN;
	update();
}

int Channel::fd() const
{
	return _sockfd;
}

////////////////////////////////////////////////////////////

void Channel::update()
{
	struct epoll_event ev;
	ev.events = _events;	// ÉèÖÃepollÄ£Ê½
	ev.data.fd = _sockfd;
	if (0 > ::epoll_ctl(_epfd, EPOLL_CTL_ADD, _sockfd, &ev)) {
		LOG(ERROR) << "epoll set insertion error: fd=" << _sockfd;
	}
}
