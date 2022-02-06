#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <vector>
#include "../interfaces/IRequest.hpp"
#include "../interfaces/IHeader.hpp"

class Request : public IHeader {
public:
	enum Method {
		UNKNOWN,
		POST,
		GET,
		PUT,
		DELETE
	};

	enum Protocol {
		ANOTHER,
		HTTP1_1
		};

private:
	Method method;
	std::string uri;
	Protocol protocol;
    std::string buffer;
public:
    const std::string &getBuffer() const;

    void setBuffer(const std::string &buffer);

private:
    std::map<std::string, std::string> headers;
	std::vector<std::string> headersVector;

public:
	Request();
	~Request();

	bool isBadRequest();
	const Method &getMethod();
	const std::string &getUri();
	const Protocol &getProtocol();

	void setMethod(const std::string &);
	void setUri(const std::string &);
	void setProtocol(const std::string &);

	std::string getHeader(const std::string&) const;
	void setHeader(std::string key, std::string value);
	void setHeadersVector(std::vector<std::string>);
	std::vector<std::string>& getHeadersVector();
	bool emptyHeader() const;

};

#endif
