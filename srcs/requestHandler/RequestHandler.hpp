#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <exception>
#include <fstream>
#include <map>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <vector>

#include "../config/Config.hpp"
#include "../interfaces/IRequestHandler.hpp"
#include "../model/Request.hpp"
#include "../model/Response.hpp"
#include "FileReader.hpp"

class RequestHandler : public IRequestHandler {
private:
	map<string, string> types;
	map<string, string> _header_fields;

	int status_code;
	unsigned int content_lengh;
	string toSend;
	string body;
//	string filepath;

private:
	const string& mimeType(const string& uri);

	bool isBadRequest(Request* request) const;

	void readfile(const string&);

public:
	RequestHandler();

	~RequestHandler();

	void formResponse(Request* request, Response* response);

	void doPost(Request* request, Response* response);

	void doGet(Request* request, Response* response);

	void doPut(Request* request, Response* response);

	void doDelete(Request* request, Response* response);
};

#endif
