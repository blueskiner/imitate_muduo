#include "net/epoller.h"

#include "glog/logging.h"

const int MAX_EPOLL_SIZE = 10;

EPoller::EPoller()
{
	_epfd = ::epoll_create(MAX_EPOLL_SIZE);
	if (0 >= _epfd) {
		LOG(ERROR) << "epoll_create error";
	}
}

EPoller::~EPoller()
{
}

void EPoller::poll(ChannelList& channelList)
{
	int nfds = ::epoll_wait(_epfd, _events, MAX_EPOLL_SIZE, -1);
	if (0 > nfds) {
		LOG(ERROR) << "EPoller::poll";
		return;
	}
	for (int i=0; i<nfds; ++i) {
		Channel* pChannel = static_cast<Channel *>(_events[i].data.ptr);
        pChannel->setRevents(_events[i].events);
        channelList.push_back(pChannel);
	}
}

void EPoller::update(Channel* channel)
{
	struct epoll_event ev;
	ev.data.ptr = channel;
	ev.events = channel->getEvents();
	int fd = channel->fd();
	if (0 > ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev)) {
		LOG(ERROR) << "epoll set insertion error: fd=" << fd;
	}
}
