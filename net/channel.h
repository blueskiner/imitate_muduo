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
	
	Channel(EventLoop* loop, int fd);
	~Channel();
	
	void handleEvent();
	void setReadCallback(const ReadEventCallback& cb);
	
	int getEvents() const;
	void setRevents(int revents);
	
	void enableReading();
	
	int fd() const;
	
private:
	// 刷新I/O事件
	void update();
	
private:
	EventLoop* _loop;
//	int _epfd;			// 分发器依赖epoll 这个是epoll的文件描述符
	const int _sockfd;	// 具体处理I/O的socket fd
	int _events;
	int _revents;		// 接收到的事件
	
	ReadEventCallback _readEventCallback;
};

#endif	// __NET_CHANNEL_H__
