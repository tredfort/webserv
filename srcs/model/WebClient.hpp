
#pragma once

#include "Request.hpp"
#include "Response.hpp"
#include "../interfaces/IEntity.hpp"

class WebClient : public IEntity {
public:
    enum Status {
        READABLE,
        WRITING,
		CLOSE
    };

private:
	int _fd;
	int _port;
	Status _status;
	Request* _request;
	Response* _response;

public:
	WebClient(int fd, int port);
	WebClient(const WebClient& client);
	WebClient& operator=(const WebClient& client);
	~WebClient();

	void setStatus(Status);
	void setRequest(Request* request);
	void setResponse(Response* response);

	int getFd() const;
	int getPort() const;
	Status getStatus() const;
	Request* getRequest() const;
	Response* getResponse() const;

	void update();

private:
    WebClient();

};

