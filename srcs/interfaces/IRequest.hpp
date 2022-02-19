#ifndef IREQUEST_HPP
#define IREQUEST_HPP

#include <iostream>

class IRequest {
public:
	enum Method { UNKNOWN, POST, GET, PUT, DELETE };

	enum Protocol { ANOTHER, HTTP1_1 };

protected:
	Method method;
	std::string uri;
	Protocol protocol;

public:
	virtual ~IRequest() {};

	virtual bool isBadRequest() const = 0;
	virtual const Method& getMethod() const = 0;
	virtual const std::string& getUri() const = 0;
	virtual const Protocol& getProtocol() const = 0;

	virtual void setMethod(const std::string&) = 0;
	virtual void setUri(const std::string&) = 0;
	virtual void setProtocol(const std::string&) = 0;
};

#endif
