#include "net/eventloop.h"
#include "net/epoller.h"

#include "glog/logging.h"

EventLoop::EventLoop()
	: _poller(new EPoller())
{
}

EventLoop::~EventLoop()
{
}

void EventLoop::loop()
{
	_quit = false;
	// 下面是epoll经典模式
	while (!_quit) {
		LOG(INFO) << "EventLoop::loop";
		_activeChannels.clear();
		_poller->poll(_activeChannels);
		
		ChannelList::iterator it;
		for (it=_activeChannels.begin(); it!=_activeChannels.end(); ++it) {
			// 事件分发处理 channel::handlEvent ==>>
			(*it)->handleEvent();
		}
	}
	_quit = true;
}

void EventLoop::updateChannel(Channel* channel)
{
	_poller->update(channel);
}
