#ifndef __NET_TIMER_H__
#define __NET_TIMER_H__

#include "base/timestamp.h"
#include "net/callbacks.h"


class Timer
{
public:
	Timer(const TimerCallback& cb, Timestamp when, double interval);
	~Timer();
	
	void run();
	
	Timestamp getExpiration() const;
	bool isRepeat() const;
	int64_t getSequence() const;
	
//	void restart(Timestamp now);
	
private:
	const TimerCallback _cb;
	Timestamp _expiration;
	const double _interval;
	
	bool _repeat;
	const int64_t _sequence;
};

#endif	// __NET_TIMER_H__
