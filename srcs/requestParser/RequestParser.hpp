#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>

#include "../model/Request.hpp"
#include "../utils/utils.hpp"

class RequestParser {

public:
	RequestParser();

	~RequestParser();

	/**
	 * parse request
	 * @param request
	 * @return true if request ended false otherwise
	 */
	bool parseRequest(Request* request);

	bool isReadyRequest(Request* request);

private:
	RequestParser(const RequestParser& parser);

	RequestParser& operator=(const RequestParser& parser);

	void parseStartLine(Request* request, string& startLine);

	void setHeaders(Request* request, vector<string> headers);

	void setHost(Request* request);

	void parsePostBodyHeaders(Request* request);

	bool isChunkedRequest(Request* request) const;

	bool isRequestWithContentLength(Request* request) const;
};

#endif
