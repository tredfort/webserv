#include "Response.hpp"

Response::Response() : _statusCode(0), _protocol("HTTP/1.1") { }

Response::~Response() { }

void Response::setProtocol(const string& protocol) { _protocol = protocol; }

void Response::setStatusCode(int statusCode) { _statusCode = statusCode; }

void Response::setStatusLine(const string& status) { _statusLine = status; }

void Response::setContentType(const string& contentType) { _contentType = contentType; }

void Response::setBuffer(const string& buffer) { _buffer = buffer; }

void Response::pushHeader(const string& header) { _headers.append(header); }

void Response::setBody(const string& body) { _body = body; }

const string& Response::getProtocol() const { return _protocol; }

int Response::getStatusCode() const { return _statusCode; }

const string& Response::getStatusLine() const { return _statusLine; }

const string& Response::getBuffer() const { return _buffer; }

const string& Response::getContentType() const { return _contentType; }

const string& Response::getHeaders() const { return _headers; }

const string& Response::getBody() const { return _body; }
