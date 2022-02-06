#include "Request.hpp"

Request::Request() :
		method(UNKNOWN),
		protocol(ANOTHER) {}

Request::~Request() {}

bool Request::isBadRequest() {
	return method == UNKNOWN || uri.empty() || protocol == ANOTHER;
}

const Request::Method& Request::getMethod() {
	return method;
}

const std::string& Request::getUri() {
	return uri;
}

const Request::Protocol& Request::getProtocol() {
	return protocol;
}

void Request::setMethod(const std::string& method) {
	if (method == "POST")
		this->method = POST;
	else if (method == "GET")
		this->method = GET;
	else if (method == "PUT")
		this->method = PUT;
	else if (method == "DELETE")
		this->method = DELETE;
}

void Request::setUri(const std::string& uri) {
	this->uri = uri;
}

void Request::setProtocol(const std::string& protocol) {
	if (protocol == "HTTP/1.1")
		this->protocol = HTTP1_1;
}

std::string Request::getHeader(const std::string& key) const {
	std::map<std::string, std::string>::const_iterator it = headers.find(key);
	if (it == headers.end())
		return "";
	return it->second;
}

bool Request::emptyHeader() const {
	return headers.empty();
}

void Request::setHeadersVector(std::vector<std::string> headers) {
	headersVector = headers;
}

std::vector<std::string>& Request::getHeadersVector() {
	return headersVector;
}

void Request::setHeader(std::string key, std::string value) {
	headers.insert(std::make_pair<std::string, std::string>(key, value));
}

const std::string &Request::getBuffer() const {
    return buffer;
}

void Request::setBuffer(const std::string &buffer) {
    Request::buffer = buffer;
}
