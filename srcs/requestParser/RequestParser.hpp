
#pragma once

#include <sys/socket.h>
#include <sys/poll.h>
#include <vector>

#include "../model/WebClient.hpp"
#include "../utils/utils.hpp"

class RequestParser {

public:
	RequestParser();
	~RequestParser();

	void processRequest(WebClient* client);
	void parseBodyHeaders(WebClient* client);

private:
	RequestParser(const RequestParser& parser);
	RequestParser& operator=(const RequestParser& parser);

	void parseStartLine(Request* request, string& startLine);
	void fillHeaders(Request* request, vector<string> headers);
};
