
#pragma once

#include <sys/socket.h>
#include <vector>

#include "../model/WebClient.hpp"


class RequestParser {

public:
    RequestParser();
    ~RequestParser();

	void processRequest(WebClient* client);
    void bodyHeadersParse(WebClient* client);

private:
    RequestParser(const RequestParser& parser);
    RequestParser& operator=(const RequestParser& parser);

    void parseStartLine(Request* request);
    void fillHeaders(Request* request);

};
