#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__


// ���Կ���
class Socket
{
public:
	Socket();
	Socket(int fd);
	Socket(const Socket& other);
	
	~Socket();
	
	Socket& operator = (const Socket& other);
	
	// ����socket
	void create();
	
private:
	int _fd;
};

#endif	// __NET_SOCKET_H__
