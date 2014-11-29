#include "net/channel.h"

Channel::Channel(int epollfd, int fd)
	: _epfd(epollfd),
	_sockfd(fd),
	_revents(0)
{
}

Channel::~Channel()
{
}

void Channel::handleEvent()
{
	if(_revents & EPOLLIN) {
		_callBack->OnIn(_sockfd);
	}
}

void Channel::setRevents(int revents)
{
	_revents = revents;
}
