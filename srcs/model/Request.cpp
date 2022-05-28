#include "Request.hpp"

Request::Request() { }

Request::~Request() { }

const string& Request::getMethod() { return _method; }

const string& Request::getUri() { return _uri; }

const string& Request::getProtocol() { return _protocol; }

void Request::setMethod(const string& method) { _method = method; }

void Request::setUri(const string& uri) { _uri = uri; }

void Request::setProtocol(const string& protocol)
{
	if (protocol == "HTTP/1.1")
		_protocol = protocol;
}

string Request::getHeader(const string& key) const
{
	map<string, string>::const_iterator it = _headers.find(key);
	if (it == _headers.end())
		return "";
	return it->second;
}

bool Request::emptyHeader() const { return _headers.empty(); }

void Request::setHeader(string key, string value) { _headers.insert(make_pair(key, value)); }

const string& Request::getBuffer() const { return _buffer; }

void Request::setBuffer(const string& buffer) { _buffer = buffer; }

void Request::appendBuffer(const string& buffer) { _buffer.append(buffer); }
