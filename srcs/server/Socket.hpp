#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <unistd.h>

class Socket {

private:
	int _sockfd;
	int _backlog;

public:
	Socket(int, int, int);
	~Socket();

	int getSockfd() const;
	int getBacklog() const;

	void setAddressReuseMode();
	void setNonblockMode();
	void bindToAddress(struct sockaddr*);
	void startListening(int);

private:
	Socket();
	Socket(const Socket&);
	Socket& operator=(const Socket&);

	void testConnection(int);
};

#endif
