#include "base/thread.h"
#include "base/exception.h"
#include "base/timestamp.h"

#include "glog/logging.h"

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/weak_ptr.hpp>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace CurrentThread {

	__thread int t_cachedTid = 0;
	__thread char t_tidString[32];
	__thread int t_tidStringLength = 6;
	__thread const char* t_threadName = "unknown";
	const bool sameType = boost::is_same<int, pid_t>::value;
	BOOST_STATIC_ASSERT(sameType);

}	// namespace CurrentThread

namespace detail {

	pid_t gettid() {
		return static_cast<pid_t>(::syscall(SYS_gettid));
	}
	
	void afterFork() {
		CurrentThread::t_cachedTid = 0;
		CurrentThread::t_threadName = "main";
		CurrentThread::tid();
		// no need to call pthread_atfork(NULL, NULL, &afterFork);
	}
	
class ThreadNameInitializer
{
public:
	ThreadNameInitializer() {
		CurrentThread::t_threadName = "main";
		CurrentThread::tid();
		pthread_atfork(NULL, NULL, &afterFork);
	}
};
	
ThreadNameInitializer init;

struct ThreadData
{
	typedef Thread::ThreadFunc ThreadFunc;
	ThreadFunc _func;
	std::string _name;
	boost::weak_ptr<pid_t> _wkTid;
	
	ThreadData(const ThreadFunc& func, const std::string& name, const boost::shared_ptr<pid_t>& tid)
		: _func(func),
		_name(name),
		_wkTid(tid)
	{ }

	void runInThread() {
		pid_t tid = CurrentThread::tid();

		boost::shared_ptr<pid_t> ptid = _wkTid.lock();
		if (ptid) {
			*ptid = tid;
			ptid.reset();
		}

		CurrentThread::t_threadName = _name.empty() ? "muduoThread" : _name.c_str();
		::prctl(PR_SET_NAME, CurrentThread::t_threadName);
		try {
			_func();
			CurrentThread::t_threadName = "finished";
		} catch (const Exception& ex) {
			CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", _name.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
			abort();
		} catch (const std::exception& ex) {
			CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", _name.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			abort();
		} catch (...) {
			CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "unknown exception caught in Thread %s\n", _name.c_str());
			throw; // rethrow
		}
	}
};

void* startThread(void* obj)
{
	ThreadData* data = static_cast<ThreadData*>(obj);
	data->runInThread();
	delete data;
	return NULL;
}

}	// namespace detail


void CurrentThread::cacheTid()
{
	if (t_cachedTid == 0) {
		t_cachedTid = detail::gettid();
		t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
	}
}

bool CurrentThread::isMainThread()
{
	return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec)
{
	struct timespec ts = { 0, 0 };
	ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
	ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
	::nanosleep(&ts, NULL);
}


int Thread::_numCreated = 0;

Thread::Thread(const ThreadFunc& func, const string& n)
	: _started(false),
	_pthreadId(0),
	_tid(new pid_t(0)),
	_func(func),
	_name(n)
{
	setDefaultName();
}

Thread::~Thread()
{
	if (_started) {
		::pthread_detach(_pthreadId);
	}
}

void Thread::start()
{
	assert(!_started);
	
	detail::ThreadData* data = new detail::ThreadData(_func, _name, _tid);
	if (0 != ::pthread_create(&_pthreadId, NULL, &detail::startThread, data)) {
		delete data;	// or no delete?
		LOG(FATAL) << "Failed in pthread_create";
		return;
	}
	_started = true;
}

int Thread::join()
{
	return ::pthread_join(_pthreadId, NULL);
}

bool Thread::isStarted() const
{
	return _started;
}

pid_t Thread::tid() const
{
	return *_tid;
}

const std::string& Thread::name() const
{
	return _name;
}

////////////////////////////////////////////////////////////

void Thread::setDefaultName()
{
	int num = _numCreated++;
	if (_name.empty()) {
		char buf[32];
		snprintf(buf, sizeof(buf), "Thread%d", num);
		_name = buf;
	}
}

int Thread::numCreated()
{
	return _numCreated;
}
