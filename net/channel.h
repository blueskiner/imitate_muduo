#ifndef __NET_CHANNEL_H__
#define __NET_CHANNEL_H__

#include <boost/function.hpp>

class EventLoop;
class ChannelCallBack;
// 将事件分发抽象为通道
class Channel
{
public:
	typedef boost::function<void()> ReadEventCallback;
	typedef boost::function<void()> EventCallback;
	
	Channel(EventLoop* loop, int fd);
	~Channel();
	
	void handleEvent();
	void setReadCallback(const ReadEventCallback& cb);
	void setWriteCallback(const EventCallback& cb);
	void setCloseCallback(const EventCallback& cb);
	void setErrorCallback(const EventCallback& cb);
	
	int getEvents() const;
	void setRevents(int revents);
	
	void enableReading();
	void disableReading();
	
	void enableWriting();
	void disableWriting();
	
	int fd() const;
	
private:
	// 刷新I/O事件
	void update();
	
private:
	EventLoop* _loop;
	const int _sockfd;	// 具体处理I/O的socket fd
	int _events;
	int _revents;		// 接收到的事件
	
	ReadEventCallback _readEventCallback;
	EventCallback _writeEventCallback;
	EventCallback _closeEventCallback;
	EventCallback _errorCallback;
};

#endif	// __NET_CHANNEL_H__
