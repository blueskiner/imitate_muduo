#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__


// 可以拷贝
class Socket
{
public:
	Socket();
	Socket(int fd);
	Socket(const Socket& other);
	
	~Socket();
	
	Socket& operator = (const Socket& other);
	
	// 创建socket
	void create();
	
private:
	int _fd;
};

#endif	// __NET_SOCKET_H__
