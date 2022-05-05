
#pragma once

#include "Request.hpp"
#include "Response.hpp"

class WebClient {
private:
	int _fd;
	int _port;
	short _status;
	Request* _request;
	Response* _response;

public:
	WebClient(int fd, int port, short status);

	WebClient(const WebClient& client);

	WebClient& operator=(const WebClient& client);

	~WebClient();

	void setStatus(short status);

	void setRequest(Request* request);

	void setResponse(Response* response);

	int getFd() const;

	int getPort() const;

	short getStatus() const;

	Request* getRequest() const;

	Response* getResponse() const;

	void update();

private:
	WebClient();
};
