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

	if (request->isPostMethod()) {
		if (isChunkedRequest(request)) {
			//			parseChunked(request);
		} else if (isRequestWithContentLength(request)) {
			//			parseBody(request);
		}
		parsePostBodyHeaders(request);
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

void RequestParser::parsePostBodyHeaders(Request* request)
{
	std::cout << "request buffer:" << request->getBuffer() << endl << endl;

	const string contentType = request->getHeader("Content-Type");
	const size_t boundaryPos = contentType.find("boundary=");
	const string boundary = boundaryPos == string::npos ? "" : contentType.substr(boundaryPos + string("boundary=").size());
	vector<string> values = ft_split(request->getBuffer(), "--" + boundary);

	for (vector<string>::iterator it = values.begin(), ite = values.end(); it != ite; ++it)
		request->addPostVariable(new PostVariable(*it));
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
	if (request->isGetMethod()) {
		request->setBuffer("");
		return true;
	} else if (request->isPostMethod()) {
		return true;
	} else if (request->isDeleteMethod()) {
		return true;
	}

	return false;
}

bool RequestParser::isChunkedRequest(Request* request) const { return request->getHeader("Transfer-Encoding") == "chunked"; }

bool RequestParser::isRequestWithContentLength(Request* request) const { return !request->getHeader("Content-Length").empty(); }
