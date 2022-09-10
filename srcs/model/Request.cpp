#include "Request.hpp"

Request::Request() { }

Request::~Request() { }

const string& Request::getMethod() const { return _method; }

const string& Request::getUri() const { return _uri; }

const string& Request::getProtocol() const { return _protocol; }

const string& Request::getHost() const { return _host; }

string Request::getHeader(const string& key) const
{
	map<string, string>::const_iterator it = _headers.find(key);
	if (it == _headers.end())
		return "";
	return it->second;
}

void Request::setMethod(const string& method) { _method = method; }

void Request::setUri(const string& uri) { _uri = uri; }

void Request::setProtocol(const string& protocol)
{
	if (!strncmp(protocol.c_str(), "HTTP/", 5))
		_protocol = protocol;
}

void Request::setHost(const string& host) { _host = host; }

bool Request::emptyHeader() const { return _headers.empty(); }

void Request::setHeader(const string& key, const string& value) { _headers.insert(make_pair(key, value)); }

const string& Request::getBuffer() const { return _buffer; }

void Request::setBuffer(const string& buffer) { _buffer = buffer; }

void Request::appendBuffer(const string& buffer) { _buffer.append(buffer); }

const string& Request::getFileName() const { return _fileName; }

void Request::setFileName(const string& fileName) { _fileName = fileName; }

size_t Request::getContentLength() const { return (int)_body.size(); }

const string& Request::getBody() const { return _body; }

void Request::setBody(const string& body) { _body = body; }
