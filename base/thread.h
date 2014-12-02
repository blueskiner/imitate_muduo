#ifndef __BASE_THREAD_H__
#define __BASE_THREAD_H__

#include <string>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>
#include <stdint.h>

namespace CurrentThread
{
	// internal
	extern __thread int t_cachedTid;
	extern __thread char t_tidString[32];
	extern __thread int t_tidStringLength;
	extern __thread const char* t_threadName;
	void cacheTid();
	
	inline int tid()
	{
		if (__builtin_expect(t_cachedTid == 0, 0))
		{
			cacheTid();
		}
		return t_cachedTid;
	}
	
	inline const char* tidString() // for logging
	{
		return t_tidString;
	}
	
	inline int tidStringLength() // for logging
	{
		return t_tidStringLength;
	}
	
	inline const char* name()
	{
		return t_threadName;
	}
	
	bool isMainThread();
	
	void sleepUsec(int64_t usec);
}

class Thread : boost::noncopyable
{
public:
	typedef boost::function<void ()> ThreadFunc;
	
	explicit Thread(const ThreadFunc& func, const std::string& n=std::string());
	~Thread();
	
	void start();
	int join();
	
	bool isStarted() const;
	pid_t tid() const;
	const std::string& name() const;
	
	static int numCreated();
	
private:
	void setDefaultName();
	
private:
	bool _started;
	pthread_t _pthreadId;
	boost::shared_ptr<pid_t> _tid;
	ThreadFunc _func;
	
	std::string _name;
	
	static int _numCreated;
};

#endif	// __BASE_THREAD_H__
