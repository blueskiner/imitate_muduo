#ifndef __NET_EVENT_LOOP_H__
#define __NET_EVENT_LOOP_H__

#include <vector>

#include <boost/scoped_ptr.hpp>

class EPoller;
class Channel;
class EventLoop
{
public:
	EventLoop();
	~EventLoop();
	
	void loop();
	void updateChannel(Channel* channel);
	
private:
	typedef std::vector<Channel *> ChannelList;
	
	bool _quit;
	boost::scoped_ptr<EPoller> _poller;
	
	ChannelList _activeChannels;
};

#endif	// __NET_EVENT_LOOP_H__
