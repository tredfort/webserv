#include "RequestParser.hpp"

static std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos;
	do {
		pos = str.find(delim, prev);
		if (pos == std::string::npos)
			pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty())
			tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

RequestParser::RequestParser() { }

RequestParser::~RequestParser() { }

void RequestParser::processRequest(WebClient* client)
{
	Request* request = client->getRequest();
	size_t pos = request->getBuffer().find("\r\n\r\n");

	if (pos == std::string::npos)
		return;

	std::string buffer = request->getBuffer().substr(0, pos);
	request->setBuffer(request->getBuffer().substr(pos + 4));

	if (client->getRequest()->emptyHeader()) {
		client->getRequest()->setHeadersVector(split(buffer, "\r\n"));
		parseStartLine(client->getRequest());
		fillHeaders(client->getRequest());
	}
	if (!request->getBuffer().empty()) {
		bodyHeadersParse(client);
	}
}

void RequestParser::bodyHeadersParse(WebClient* client)
{
	Request* request = client->getRequest();
	size_t pos = request->getBuffer().find("\r\n\r\n");

	if (pos == std::string::npos)
		return;

	std::string buffer = request->getBuffer().substr(0, pos);
	request->setBuffer(request->getBuffer().substr(pos + 4));

	// TODO: написать этот метод
}

void RequestParser::parseStartLine(Request* request)
{
	std::string& line = request->getHeadersVector()[0];
	size_t start;
	size_t end = line.find(' ');
	if (end == std::string::npos)
		return;
	request->setMethod(line.substr(0, end));

	start = end + 1;
	end = line.find(' ', start);
	if (end == std::string::npos)
		return;
	request->setUri(line.substr(start, end - start));

	start = end + 1;
	if (start > line.length())
		return;
	request->setProtocol(line.substr(start));
}

void RequestParser::fillHeaders(Request* request)
{
	std::vector<std::string> strings = request->getHeadersVector();
	size_t pos;
	for (size_t i = 1; i < strings.size(); ++i) {
		pos = strings[i].find(": ");
		if (pos == std::string::npos)
			continue;
		std::string key = strings[i].substr(0, pos);
		std::string value = strings[i].substr(pos + 2);
		request->setHeader(key, value);
	}
}
