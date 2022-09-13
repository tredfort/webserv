#include "RequestParser.hpp"

RequestParser::RequestParser() { }

RequestParser::~RequestParser() { }

void RequestParser::parseRequest(Request* request)
{
	size_t pos = request->getBuffer().find("\r\n\r\n");

	if (pos != std::string::npos && request->emptyHeader()) {
		std::string buffer = request->getBuffer().substr(0, pos);
		request->setBuffer(request->getBuffer().substr(pos + 4));
		vector<string> headers = ft_split(buffer, "\r\n");
		parseStartLine(request, headers[0]);
		setHeaders(request, headers);
		setHost(request); // TODO: вынести в utils, реализовать как метод getHostName()
	}

	if (request->getMethod() == "POST") {
		if (isChunkedRequest(request)) {
			parseChunked(request);
		} else if (isRequestWithContentLength(request)) {
			parseBody(request);
		}
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

void RequestParser::setHeaders(Request* request, vector<string> headers)
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

void RequestParser::setHost(Request* request)
{
	string host = request->getHeader("Host");
	size_t pos = host.find(':');
	if (pos != std::string::npos) {
		request->setHost(host.substr(0, pos));
	} else {
		request->setHost(host);
	}
}

bool RequestParser::isReadyRequest(Request* request)
{
	if (request->getMethod() == "GET") {
		request->setBuffer("");
		return true;
	} else if (request->getMethod() == "POST") {
		if (isChunkedRequest(request)) {
			return request->getBuffer().empty();
		}
		return request->getContentLength() == (size_t)stringToInt(request->getHeader("Content-Length"));
	} else if (request->getMethod() == "DELETE") {
		return true;
	}
	if (request->getMethod() != "GET" && request->getMethod() != "POST" && request->getMethod() != "DELETE" && !request->getMethod().empty()) {
		return true;
	}

	return false;
}

bool RequestParser::isChunkedRequest(Request* request) const
{
	return request->getHeader("Transfer-Encoding") == "chunked";
}

bool RequestParser::isRequestWithContentLength(Request* request) const
{
	return !request->getHeader("Content-Length").empty();
}

void RequestParser::parseBody(Request* request)
{
	string fileName = getFileName(request->getUri());
	request->setFileName(fileName);
	size_t contentLength = stringToInt(request->getHeader("Content-Length"));
	if (request->getBuffer().size() > contentLength) {
		throw "400 BAD REQUEST! RECV size > then MUST BE";
	}
	string body = request->getBuffer();
	request->setBuffer("");
	request->setBody(request->getBody() + body);
}

void RequestParser::parseChunked(Request* request)
{
	const string newLine = "\r\n";
	const string endOfChunkedRequest = "0\r\n\r\n";
	size_t newLineLen = newLine.length();
	const string& chunks = request->getBuffer();
	size_t chunkedRequestEnd = chunks.find(endOfChunkedRequest);
	size_t chunkEnd = 0;
	if (chunkedRequestEnd != std::string::npos) {
		if (chunks.length() != chunkedRequestEnd + endOfChunkedRequest.length()) {
			throw BadChunkedRequestException();
		}
		while (chunkEnd < chunkedRequestEnd) {
			size_t chunkStart = chunks.find(newLine, chunkEnd);
			size_t chunkLength = stringToInt(chunks.substr(chunkEnd, chunkStart - chunkEnd));
			chunkStart += newLineLen;
			chunkEnd = chunks.find(newLine, chunkStart);
			string chunk = chunks.substr(chunkStart, chunkEnd - chunkStart);
			chunkEnd += newLineLen;
			if (chunk.length() != chunkLength) {
				throw BadChunkedRequestException();
			}
			request->setBody(request->getBody() + chunk);
		}
		request->setBuffer("");
		request->setFileName(getFileName(request->getUri()));
	}
}
