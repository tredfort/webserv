#ifndef WEBCLIENT_HPP
#define WEBCLIENT_HPP

#include "Request.hpp"
#include "Response.hpp"

class WebClient {
private:
	int _fd;
	int _port;
	string _ip;
	Request* _request;
	Response* _response;

public:
	WebClient(int fd, const string& ip, int port);

	WebClient(const WebClient& client);

	WebClient& operator=(const WebClient& client);

	~WebClient();

	int getFd() const;

	const string& getIp() const;

	int getPort() const;

	Request* getRequest() const;

	Response* getResponse() const;

	void setRequest(Request* request);

	void setResponse(Response* response);

	void clear();

private:
	WebClient();
};

#endif
