#ifndef SOCKADDR_H
#define SOCKADDR_H
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"

NAMESPACE_START

class SockAddr
{
	friend bool operator==(const SockAddr &larg, const SockAddr &rarg);
	friend bool operator!=(const SockAddr &larg, const SockAddr &rarg);
public:
	SockAddr();

	SockAddr(const SockAddr &sockAddr);

	SockAddr(const struct sockaddr*, socklen_t socklen);

	SockAddr(uint16_t port, const std::string &ip="0.0.0.0", uint8_t family = AF_INET);

	SockAddr& operator=(const SockAddr &sockAddr);

	~SockAddr(){}

	const struct sockaddr* getAddr() const;

	struct sockaddr* getAddr();

	std::string getIp() const;

	int getPort() const;

	int getFamily() const;

	socklen_t getAddrLength() const;

	int set(const struct sockaddr* sockAddr, socklen_t socklen);

	int set(const std::string &ip, uint16_t port, uint8_t family = AF_INET);

	int setIp(const std::string &ip);

	int setPort(uint16_t port);

	int setFamily(uint8_t family);

	int setAny();

	std::string format() const;

private:
	struct sockaddr_storage sockaddr;

	void reset();

	int setSockaddrFamily(uint8_t family);

	int setSockaddrAddr(uint8_t family, const std::string &ip);

	int setSockaddrPort(uint8_t family, uint16_t port);
};

inline bool operator==(const SockAddr &larg, const SockAddr &rarg) {
	return bcmp(&larg.sockaddr, &rarg.sockaddr, sizeof(rarg.sockaddr))==0;
}

inline bool operator!=(const SockAddr &larg, const SockAddr &rarg) {
	return !(larg==rarg);
}

NAMESPACE_END

#endif

