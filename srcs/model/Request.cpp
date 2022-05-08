#include "Request.hpp"

Request::Request()
	: _method(UNKNOWN_METHOD)
	, _protocol(UNKNOWN_PROTOCOL)
{
}

Request::~Request() { }

const Method& Request::getMethod() { return _method; }

const string& Request::getUri() { return _uri; }

const Protocol& Request::getProtocol() { return _protocol; }

void Request::setMethod(const string& method)
{
	if (method == "POST")
		_method = POST;
	else if (method == "GET")
		_method = GET;
	else if (method == "PUT")
		_method = PUT;
	else if (method == "DELETE")
		_method = DELETE;
}

void Request::setUri(const string& uri) { _uri = uri; }

void Request::setProtocol(const string& protocol)
{
	if (protocol == "HTTP/1.1")
		_protocol = HTTP1_1;
}

string Request::getHeader(const string& key) const
{
	map<string, string>::const_iterator it = headers.find(key);
	if (it == headers.end())
		return "";
	return it->second;
}

bool Request::emptyHeader() const { return headers.empty(); }

void Request::setHeadersVector(vector<string> headers) { headersVector = headers; }

vector<string>& Request::getHeadersVector() { return headersVector; }

void Request::setHeader(string key, string value) { headers.insert(make_pair(key, value)); }

const string& Request::getBuffer() const { return _buffer; }

void Request::setBuffer(const string& buffer) { _buffer = buffer; }
