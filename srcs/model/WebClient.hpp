#ifndef WEBCLIENT_HPP
#define WEBCLIENT_HPP

#include "Request.hpp"
#include "Response.hpp"

class WebClient {
private:
	int _fd;
	string _ip;
	string _port;
	Request* _request;
	Response* _response;

public:
	WebClient(int fd, const string& ip, const string& port);

	WebClient(const WebClient& client);

	WebClient& operator=(const WebClient& client);

	~WebClient();

	int getFd() const;

	const string& getIp() const;

	const string& getPort() const;

	Request* getRequest() const;

	Response* getResponse() const;

	void setRequest(Request* request);

	void setResponse(Response* response);

	void clear();

private:
	WebClient();
};

#endif
