#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../interfaces/IHeader.hpp"
#include "../interfaces/IRequest.hpp"
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

using std::make_pair;
using std::map;
using std::string;

class Request {
private:
	string _method;
	string _uri;
	string _protocol;
	string _buffer;
	map<string, string> _headers;

public:
	Request();

	~Request();

	const string& getMethod() const;

	const string& getUri() const;

	const string& getProtocol() const;

	const string& getBuffer() const;

	void setMethod(const string& method);

	void setUri(const string&);

	void setProtocol(const string&);

	void setBuffer(const string& buffer);

	void appendBuffer(const string& buffer);

	string getHeader(const string&) const;

	void setHeader(const string& key, const string& value);

	bool emptyHeader() const;
};

#endif
