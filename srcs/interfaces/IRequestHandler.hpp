#ifndef IREQUESTHANDLER_HPP
#define IREQUESTHANDLER_HPP

#include "../model/Request.hpp"
#include "../model/Response.hpp"
#include "../server/Env.hpp"

class IRequestHandler {
public:
	~IRequestHandler() {};

	virtual void formResponse(Request* request, Response* response, Env& env) = 0;

	virtual void doPost(Request* request, Response* response) = 0;

	virtual void doGet(Request* request, Response* response, Env& env) = 0;

	virtual void doPut(Request* request, Response* response) = 0;

	virtual void doDelete(Request* request, Response* response) = 0;
};

#endif
