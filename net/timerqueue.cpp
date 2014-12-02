#include "net/timerqueue.h"
#include "net/timer.h"

TimerQueue::TimerQueue(EventLoop* loop, int timerfd)
	: _loop(loop),
	_timerfd(timerfd)
{
}

TimerQueue::~TimerQueue()
{
}

void TimerQueue::handleRead()
{
}

long TimerQueue::addTimer(const TimerCallback& cb, Timestamp when, double interval)
{
	return 0;
}

void TimerQueue::cancelTimer(long timerId)
{
}
