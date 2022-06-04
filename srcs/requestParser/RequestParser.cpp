#include "RequestParser.hpp"

RequestParser::RequestParser() { }

RequestParser::~RequestParser() { }

void RequestParser::processRequest(Request* request)
{
	size_t pos = request->getBuffer().find("\r\n\r\n");

	if (pos != std::string::npos && request->emptyHeader()) {
		std::string buffer = request->getBuffer().substr(0, pos);
		request->setBuffer(request->getBuffer().substr(pos + 4));
		vector<string> headers = ft_split(buffer, "\r\n");
		parseStartLine(request, headers[0]);
		fillHeaders(request, headers);
	}

	if (request->getMethod() == "POST") {
		parseBodyHeaders(request);
	}
}

void RequestParser::parseStartLine(Request* request, string& startLine)
{
	size_t start;
	size_t end = startLine.find(' ');
	if (end == std::string::npos)
		return;
	request->setMethod(startLine.substr(0, end));

	start = end + 1;
	end = startLine.find(' ', start);
	if (end == std::string::npos)
		return;
	request->setUri(startLine.substr(start, end - start));

	start = end + 1;
	if (start > startLine.length())
		return;
	request->setProtocol(startLine.substr(start));
}

void RequestParser::fillHeaders(Request* request, vector<string> headers)
{
	size_t pos;
	for (size_t i = 1; i < headers.size(); ++i) {
		pos = headers[i].find(": ");
		if (pos == std::string::npos)
			continue;
		std::string key = headers[i].substr(0, pos);
		std::string value = headers[i].substr(pos + 2);
		request->setHeader(key, value);
	}
}

void RequestParser::parseBodyHeaders(Request* request)
{
	size_t pos = request->getBuffer().find("\r\n\r\n");

	if (pos == std::string::npos)
		return;

	std::string buffer = request->getBuffer().substr(0, pos);
	request->setBuffer(request->getBuffer().substr(pos + 4));

	// TODO: написать этот метод
}

bool RequestParser::isReadyRequest(Request* request)
{
	if (request->getMethod() == "GET") {
		request->setBuffer("");
		return true;
	}
	else if (request->getMethod() == "POST") {

	}
	else if (request->getMethod() == "DELETE") {

	}

	return false;
}
