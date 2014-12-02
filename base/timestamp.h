#ifndef __BASE_TIMESTAMP_H__
#define __BASE_TIMESTAMP_H__

#include <sys/types.h>
#include <string>

class Timestamp
{
public:
	Timestamp();
	explicit Timestamp(int64_t microSeconds = 0.0);
	~Timestamp();
	
	bool valid();
    int64_t microSecondsSinceEpoch();
    std::string toString() const;
    
    
    static Timestamp now();
    static Timestamp nowAfter(int64_t seconds);
    static int64_t nowMicroSeconds();
    static const int kMicroSecondsPerSecond = 1000 * 1000;
	
private:
	int64_t _microSecondsSinceEpoch;
};

inline bool operator < (Timestamp l, Timestamp r)
{
	return l.microSecondsSinceEpoch() < r.microSecondsSinceEpoch();
}
inline bool operator == (Timestamp l, Timestamp r)
{
    return l.microSecondsSinceEpoch() == r.microSecondsSinceEpoch();
}

#endif	// __BASE_TIMESTAMP_H__
