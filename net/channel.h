#ifndef __NET_CHANNEL_H__
#define __NET_CHANNEL_H__

#include <boost/function.hpp>

class EventLoop;
class ChannelCallBack;
// ���¼��ַ�����Ϊͨ��
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
	// ˢ��I/O�¼�
	void update();
	
private:
	EventLoop* _loop;
//	int _epfd;			// �ַ�������epoll �����epoll���ļ�������
	const int _sockfd;	// ���崦��I/O��socket fd
	int _events;
	int _revents;		// ���յ����¼�
	
	ReadEventCallback _readEventCallback;
};

#endif	// __NET_CHANNEL_H__
