#ifndef __NET_CHANNEL_H__
#define __NET_CHANNEL_H__


// ���¼��ַ�����Ϊͨ��
class Channel
{
public:
	Channel(int epollfd, int fd);
	~Channel();
	
	void handleEvent();
	void setRevents(int revents);
	
private:
	int _epfd;			// �ַ�������epoll �����epoll���ļ�������
	const int _sockfd;	// ���崦��I/O��socket fd
	
	int _revents;		// ���յ����¼�
};

#endif	// __NET_CHANNEL_H__
