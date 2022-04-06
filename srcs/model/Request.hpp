#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../interfaces/IHeader.hpp"
#include "../interfaces/IRequest.hpp"
#include "Method.hpp"
#include "Protocol.hpp"
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
	Protocol _protocol;
	string _buffer;

public:
	const string& getBuffer() const;

	void setBuffer(const string& buffer);

private:
	map<string, string> headers;
	vector<string> headersVector;

public:
	Request();

	~Request();

	bool isBadRequest();

	const Method& getMethod();

	const string& getUri();

	const Protocol& getProtocol();

	void setMethod(const string&);

	void setUri(const string&);

	void setProtocol(const string&);

	string getHeader(const string&) const;

	void setHeader(string key, string value);

	void setHeadersVector(vector<string>);

	vector<string>& getHeadersVector();

	bool emptyHeader() const;

	map<string, string> getHeadrsMap() const;
};

#endif
