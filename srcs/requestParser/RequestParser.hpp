#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <sys/socket.h>
#include <sys/poll.h>
#include <vector>

#include "../model/Request.hpp"
#include "../utils/utils.hpp"

class RequestParser {

public:
	RequestParser();

	~RequestParser();

	void processRequest(Request* request);

	bool isReadyRequest(Request* request);

private:
	RequestParser(const RequestParser& parser);

	RequestParser& operator=(const RequestParser& parser);

	void parseStartLine(Request* request, string& startLine);

	void setHeaders(Request* request, vector<string> headers);

	void setHost(Request* request);

	void parseBodyHeaders(Request* request);
};

#endif
