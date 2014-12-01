#ifndef __NET_EPOLLER_H__
#define __NET_EPOLLER_H__

#include "net/channel.h"

#include <vector>
#include <sys/epoll.h>

class EPoller
{
public:
	typedef std::vector<Channel *> ChannelList;
	
	EPoller();
	~EPoller();
	
	void poll(ChannelList& channelList);
	void update(Channel* channel);
	
private:
	int _epfd;
	struct epoll_event _events[128];
};

#endif	// __NET_EPOLLER_H__
