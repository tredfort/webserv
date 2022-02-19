
#pragma once

#include "../interfaces/IEntity.hpp"
#include "Request.hpp"
#include "Response.hpp"

class WebClient : public IEntity {
private:
	int _fd;
	int _port;
	Request* _request;
	Response* _response;

public:
	WebClient(int fd, int port);

	WebClient(const WebClient& client);

	WebClient& operator=(const WebClient& client);

	~WebClient();

	void setRequest(Request* request);

	void setResponse(Response* response);

	int getFd() const;

	int getPort() const;

	Request* getRequest() const;

	Response* getResponse() const;

	void update();

private:
	WebClient();
};
