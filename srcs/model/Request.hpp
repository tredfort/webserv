#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../interfaces/IHeader.hpp"
#include "../interfaces/IRequest.hpp"
#include "Method.hpp"
#include <iostream>
#include <map>
#include <vector>

using std::make_pair;
using std::map;
using std::string;
using std::vector;

class Request : public IHeader {
private:
	Method _method;
	string _uri;
	string _protocol;
	string _buffer;
	map<string, string> _headers;

public:
	Request();

	~Request();

	const Method& getMethod();

	const string& getUri();

	const string& getProtocol();

	const string& getBuffer() const;

	void setMethod(const string&);

	void setUri(const string&);

	void setProtocol(const string&);

	void setBuffer(const string& buffer);

	void appendBuffer(const string& buffer);

	string getHeader(const string&) const;

	void setHeader(string key, string value);

	bool emptyHeader() const;
};

#endif
