#include "Response.hpp"

Response::Response() { }

Response::~Response() { }

void Response::setProtocol(const string& protocol) { _protocol = protocol; }

void Response::setStatus(const string& status) { _status = status; }

void Response::setContentType(const string& contentType) { _contentType = contentType; }

void Response::setBuffer(const string& buffer) { _buffer = buffer; }

void Response::pushHeader(const string& header) { _headers.append(header); }

void Response::setBody(const string& body) { _body = body; }

const string& Response::getProtocol() const { return _protocol; }

const string& Response::getStatus() const { return _status; }

const string& Response::getBuffer() const { return _buffer; }

const string& Response::getContentType() const { return _contentType; }

const string& Response::getHeaders() const { return _headers; }

const string& Response::getBody() const { return _body; }
