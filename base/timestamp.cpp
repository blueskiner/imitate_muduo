#include "base/timestamp.h"

#include <sys/time.h>
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

Timestamp::Timestamp()
	: _microSecondsSinceEpoch(0)
{
}

Timestamp::Timestamp(int64_t microSeconds)
	: _microSecondsSinceEpoch(microSeconds)
{
}

Timestamp::~Timestamp()
{
}

bool Timestamp::valid()
{
	return _microSecondsSinceEpoch > 0;
}

int64_t Timestamp::microSecondsSinceEpoch()
{
	return _microSecondsSinceEpoch;
}

std::string Timestamp::toString() const
{
	char buf[32] = {0};
	int64_t seconds = _microSecondsSinceEpoch / kMicroSecondsPerSecond;
	int64_t microseconds = _microSecondsSinceEpoch % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
	return buf;
}

////////////////////////////////////////////////////////////

Timestamp Timestamp::now()
{
	struct timeval tv;
	::gettimeofday(&tv, NULL);
	int64_t seconds = tv.tv_sec;
	return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

Timestamp Timestamp::nowAfter(int64_t seconds)
{
	return Timestamp(Timestamp::nowMicroSeconds() + kMicroSecondsPerSecond * seconds);
}

int64_t Timestamp::nowMicroSeconds()
{
	struct timeval tv;
	::gettimeofday(&tv, NULL);
	int64_t seconds = tv.tv_sec;
	return seconds * kMicroSecondsPerSecond + tv.tv_usec;
}
