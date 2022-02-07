#include "Request.hpp"

Request::Request() :
        _method(UNKNOWN),
        _protocol(ANOTHER) {}

Request::~Request() {}

bool Request::isBadRequest() {
    return _method == UNKNOWN || _uri.empty() || _protocol == ANOTHER;
}

const Request::Method &Request::getMethod() {
    return _method;
}

const std::string &Request::getUri() {
    return _uri;
}

const Request::Protocol &Request::getProtocol() {
    return _protocol;
}

void Request::setMethod(const std::string &method) {
    if (method == "POST")
        _method = POST;
    else if (method == "GET")
        _method = GET;
    else if (method == "PUT")
        _method = PUT;
    else if (method == "DELETE")
        _method = DELETE;
}

void Request::setUri(const std::string &uri) {
    _uri = uri;
}

void Request::setProtocol(const std::string &protocol) {
    if (protocol == "HTTP/1.1")
        _protocol = HTTP1_1;
}

std::string Request::getHeader(const std::string &key) const {
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

std::vector<std::string> &Request::getHeadersVector() {
    return headersVector;
}

void Request::setHeader(std::string key, std::string value) {
    headers.insert(std::make_pair<std::string, std::string>(key, value));
}

const std::string &Request::getBuffer() const {
    return _buffer;
}

void Request::setBuffer(const std::string &buffer) {
    _buffer = buffer;
}
