#include "Response.hpp"

Response::Response()
{

	//    file.open(fileName, std::ios::binary | std::ios::ate);
	//    try {
	//        fileSize = file.tellg();
	//    } catch (std::exception& e) {
	//        std::cout << e.what() << std::endl;
	//    }
	//    file.seekg(0, std::ios::beg);
}

Response::~Response() { }

std::string Response::toString() const {
	return toSend;
}
void Response::setProtocol(const string& protocol) { _protocol = protocol; }

void Response::setStatus(string status) { _status = status; }

void Response::setContentType(const string& contentType) { _contentType = contentType; }

void Response::setBuffer(const string& buffer) { _buffer = buffer; }

void Response::setHeader(const string& header) { _headers.push_back(header); }

void Response::setBody(const string& body) { _body = body; }

const string& Response::getProtocol() const { return _protocol; }

const string& Response::getStatus() const { return _status; }

const string& Response::getBuffer() const { return _buffer; }

const string& Response::getContentType() const { return _contentType; }

const string& Response::getBody() const { return _body; }
