#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <cstring>
#include <dirent.h>
#include <exception>
#include <fstream>
#include <map>
#include <sys/socket.h>
#include <vector>

#include "../config/Config.hpp"
#include "../interfaces/IRequestHandler.hpp"
#include "../model/WebClient.hpp"
#include "FileReader.hpp"

class RequestHandler : public IRequestHandler {
public:
	Config* config;
	//временные переменные
	map<string, string> types;
	LocationContext location;

	RequestHandler(Config* config);

	~RequestHandler();

	void formResponse(WebClient* client);

	void doPost(Request* request, Response* response);

	void doGet(Request* request, Response* response);

	void doPut(Request* request, Response* response);

	void doDelete(Request* request, Response* response);

private:
	const string& mimeType(const string& uri);

	bool isBadRequest(Request* request) const;

	void readfile(Response* response, const string&);

	void folderContents(Response* response, const string&, const string&);

	bool fillBodyFromIndexFile(Response* response, const string&);

	void setResponseWithError(Response* response, string errorMessage);

	void fillHeaders(Response* response);

};

#endif
