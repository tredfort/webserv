#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../interfaces/IResponse.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using std::string;
using std::vector;

class Response {
private:
	string _protocol;
	string _status;
	string _contentType;
	string _buffer;
	string _headers;
	string _body;

public:
	Response();

	~Response();

	void setProtocol(const string& protocol);

	void setStatus(const string& status);

	void setContentType(const string& contentType);

	void setBuffer(const string& buffer);

	void pushHeader(const string& header);

	void setBody(const string& body);

	const string& getProtocol() const;

	const string& getStatus() const;

	const string& getBuffer() const;

	const string& getContentType() const;

	const string& getHeaders() const;

	const string& getBody() const;

private:
	Response(const Response&);

	Response& operator=(const Response&);
};

#endif
