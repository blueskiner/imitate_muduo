#ifndef __NET_CHANNEL_H__
#define __NET_CHANNEL_H__


// 将事件分发抽象为通道
class Channel
{
public:
	Channel(int epollfd, int fd);
	~Channel();
	
	void handleEvent();
	void setRevents(int revents);
	
private:
	int _epfd;			// 分发器依赖epoll 这个是epoll的文件描述符
	const int _sockfd;	// 具体处理I/O的socket fd
	
	int _revents;		// 接收到的事件
};

#endif	// __NET_CHANNEL_H__
