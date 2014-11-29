#include "net/socket.h"

#include "glog/logging.h"

#include <sys/socket.h>

Socket::Socket() : _fd(-1)
{
}

Socket::Socket(int fd) : _fd(fd)
{
}

Socket::Socket(const Socket& other)
{
	_fd = other._fd;
}

Socket::~Socket()
{
}

Socket& Socket::operator = (const Socket& other)
{
	_fd = other._fd;
	return *this;
}

void Socket::create()
{
	if (0 > ::socket(AF_INET, SOCK_STREAM, 0)) {
		LOG(ERROR) << "socket create error";
	}
}
