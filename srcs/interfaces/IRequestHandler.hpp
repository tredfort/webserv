#ifndef IREQUESTHANDLER_HPP
#define IREQUESTHANDLER_HPP

#include "../model/WebClient.hpp"

class IRequestHandler {
public:
	virtual ~IRequestHandler() {};

	virtual void formResponse(WebClient* client) = 0;
	virtual void doPost(WebClient* client) = 0;
	virtual void doGet(WebClient* client) = 0;
	virtual void doPut(WebClient* client) = 0;
	virtual void doDelete(WebClient* client) = 0;
};

#endif
