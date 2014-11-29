#include "net/socket.h"

#include "glog/logging.h"

#include <sys/types.h>		/* See NOTES */
#include <sys/socket.h>		// basic socket definitions
#include <netinet/tcp.h>	// for TCP_NODELAY
#include <fcntl.h>			// nonblocking
#include <errno.h>

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

Socket& Socket::operator = (const int sockfd)
{
	this->_fd = sockfd;
	return *this;
}

int Socket::fd() const
{
	return _fd;
}

void Socket::create()
{
	_fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (0 > _fd) {
		LOG(ERROR) << "socket create error";
	}
}

void Socket::bind(const InetAddr& addr)
{
	const struct sockaddr_in sa = addr.getSockAddrInet();
	socklen_t addrlen = sizeof(sockaddr);
	if (0 == ::bind(_fd, (const struct sockaddr *)(&sa), addrlen)) {
		return;
	} else {
		LOG(ERROR) << "socket bind error";
		// bind´íÎó·ÖÎö
		int n = errno;
		switch (n) {
		case EACCES:
		case EADDRINUSE:
		case EBADF:
		case EINVAL:
		case ENOTSOCK:
		case EADDRNOTAVAIL:
		case EFAULT:
		case ELOOP:
		case ENAMETOOLONG:
		case ENOENT:
		case ENOMEM:
		case ENOTDIR:
		case EROFS:
			break;
		default:break;
		}
	}
}

void Socket::listen()
{
	if (0 == ::listen(_fd, 10)) {
		return;
	} else {
		LOG(ERROR) << "socket listen error";
		// listen´íÎó·ÖÎö
		int n = errno;
		switch (n) {
		case EADDRINUSE:
		case EBADF:
		case ENOTSOCK:
		case EOPNOTSUPP:// The socket is not of a type that supports the listen() operation.
			break;
		default:break;
		}
	}
}

void Socket::close()
{
	if (0 == ::close(_fd)) {
		return;
	} else {
		LOG(ERROR) << "socket close error";
		// close ´íÎó·ÖÎö
	}
}

int Socket::accept(InetAddr* peeraddr)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(sockaddr);
	bzero(&addr, sizeof(addr));
	int connfd = ::accept(_fd, (struct sockaddr *)(&addr), &addrlen);
	if (connfd >= 0) {
		peeraddr->setSockAddrInet(addr);
	}
	return connfd;
}

//////////////////////////////////////////////////

void Socket::setTcpNoDelay(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval));
	// FIXME CHECK
}

void Socket::setReuseAddr(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval));
	// FIXME CHECK
}

void Socket::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
	int optval = on ? 1 : 0;
	int ret = ::setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval));
	if (ret < 0) {
		//LOG_SYSERR << "SO_REUSEPORT failed.";
	}
#else
	if (on) {
		LOG(ERROR) << "SO_REUSEPORT is not supported.";
	}
#endif
}

void Socket::setKeepAlive(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof optval));
	// FIXME CHECK
}

void Socket::setNonBlock()
{
	// non-block
	int flags = ::fcntl(_fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	int ret = ::fcntl(_fd, F_SETFL, flags);
	// FIXME check
	
	// close-on-exec
	flags = ::fcntl(_fd, F_GETFD, 0);
	flags |= FD_CLOEXEC;
	ret = ::fcntl(_fd, F_SETFD, flags);
}
