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

void Response::setStatusCode(int status_code) { _statusCode = status_code; }

void Response::setBuffer(const string& buffer) { _buffer = buffer; }

void Response::setHeader(const string& header) { _headers.push_back(header); }

void Response::setBody(const string& body) { _body = body; }
