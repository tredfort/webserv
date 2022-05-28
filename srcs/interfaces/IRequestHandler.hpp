#ifndef IREQUESTHANDLER_HPP
#define IREQUESTHANDLER_HPP

#include "../model/WebClient.hpp"

class IRequestHandler {
public:
	~IRequestHandler() {};

	virtual void formResponse(WebClient* client) = 0;

	virtual void doPost(Request* request, Response* response) = 0;

	virtual void doGet(Request* request, Response* response) = 0;

	virtual void doPut(Request* request, Response* response) = 0;

	virtual void doDelete(Request* request, Response* response) = 0;
};

#endif
