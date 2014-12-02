#ifndef __NET_TIMER_QUEUE_H__
#define __NET_TIMER_QUEUE_H__

#include "base/timestamp.h"
#include "net/callbacks.h"

#include <set>

class Timer;
class EventLoop;
class TimerQueue
{
public:
	TimerQueue(EventLoop* loop, int timerfd);
	~TimerQueue();
	
	long addTimer(const TimerCallback& cb, Timestamp when, double interval);
	void cancelTimer(long timerId);
	
private:
	typedef std::pair<Timestamp, Timer *> Entry;
	typedef std::set<Entry> TimerList;
	
	void handleRead();
	
private:
	EventLoop* _loop;
	const int _timerfd;
	
	TimerList _timers;
};

#endif	// __NET_TIMER_QUEUE_H__
