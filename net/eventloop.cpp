#include "net/eventloop.h"
#include "net/epoller.h"

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
	while (!_quit) {
		_activeChannels.clear();
		_poller->poll(_activeChannels);
		
		ChannelList::iterator it;
		for (it=_activeChannels.begin(); it!=_activeChannels.end(); ++it) {
			(*it)->handleEvent();
		}
	}
	_quit = true;
}

void EventLoop::updateChannel(Channel* channel)
{
	_poller->update(channel);
}
