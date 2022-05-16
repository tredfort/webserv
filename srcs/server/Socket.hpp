#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <cstdlib>
#include <sys/fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

using std::string;
using std::cerr;
using std::endl;

class Socket {

private:
	int _sockfd;
	int _backlog;
	struct sockaddr_in _address;

public:
	Socket(int, int, int);
	~Socket();

	int getSockfd() const;
	int getPort() const;
	int getBacklog() const;
	const sockaddr_in& getAddress() const;

	void setAddressReuseMode();
	void setNonblockMode();
	void bindToAddress(string, string);
	void startListening(int);

private:
	Socket();
	Socket(const Socket&);
	Socket& operator=(const Socket&);

	void testConnection(int);
};

#endif
