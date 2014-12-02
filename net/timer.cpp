#include "net/timer.h"

Timer::Timer(const TimerCallback& cb, Timestamp when, double interval)
	: _cb(cb),
	_expiration(when),
	_interval(interval),
	_repeat(interval > 0.0),
	_sequence(0)
{
}

Timer::~Timer()
{
}

void Timer::run()
{
	_cb();
}

Timestamp Timer::getExpiration() const
{
	return _expiration;
}

bool Timer::isRepeat() const
{
	return _repeat;
}

int64_t Timer::getSequence() const
{
	return _sequence;
}
#if 0
void Timer::restart(Timestamp now)
{
	if (_repeat) {
		_expiration = addTime(now, _interval);
	} else {
		_expiration = Timestamp::invalid();
	}
}
#endif
