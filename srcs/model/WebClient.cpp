#include "WebClient.hpp"

WebClient::WebClient(int fd, const string& ip, const string& port)
	: _fd(fd)
	, _ip(ip)
	, _port(port)
	, _request(new Request())
	, _response(new Response())
{
}

WebClient::WebClient(const WebClient& other)
	: _fd(other._fd)
	, _ip(other._ip)
	, _port(other._port)
	, _request(other._request)
	, _response(other._response)
{
}

WebClient& WebClient::operator=(const WebClient& other)
{
	if (this == &other)
		return *this;

	_fd = other._fd;
	_ip = other._ip;
	_port = other._port;
	_request = other._request;
	_response = other._response;
	return *this;
}

WebClient::~WebClient()
{
	delete _request;
	delete _response;
}

void WebClient::setRequest(Request* request) { this->_request = request; }

void WebClient::setResponse(Response* response) { this->_response = response; }

int WebClient::getFd() const { return _fd; }

const string& WebClient::getIp() const { return _ip; }

const string& WebClient::getPort() const { return _port; }

Request* WebClient::getRequest() const { return _request; }

Response* WebClient::getResponse() const { return _response; }
void WebClient::update()
{
	delete _request;
	delete _response;
	_request = new Request();
	_response = new Response();
}
