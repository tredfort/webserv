#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <unistd.h>

using std::cerr;
using std::endl;
using std::string;

class Socket {

private:
	int _sockfd;
	int _backlog;
	string _ip;
	int _port;
	struct sockaddr_in _address;

public:
	Socket(int, int, int);
	~Socket();

	int getSockfd() const;
	int getBacklog() const;
	const string& getIp() const;
	int getPort() const;
	const sockaddr_in& getAddress() const;

	void setAddressReuseMode();
	void setNonblockMode();
	void bindToAddress(const string& ip, int port);
	void startListening(int backlog);

private:
	Socket();
	Socket(const Socket&);
	Socket& operator=(const Socket&);

	void testConnection(int);
};

#endif
