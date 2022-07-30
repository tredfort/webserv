#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <cstring>
#include <dirent.h>
#include <exception>
#include <fstream>
#include <map>
#include <sys/socket.h>
#include <vector>

#include "../CGI/CGI.hpp"
#include "../config/Config.hpp"
#include "../interfaces/IRequestHandler.hpp"
#include "../model/Request.hpp"
#include "../model/Response.hpp"
#include "../model/WebClient.hpp"
#include "../utils/constants.hpp"
#include "../utils/usings.hpp"
#include "FileReader.hpp"

class RequestHandler {
public:
	Config* config;
	map<string, string> _types;
	map<string, string> _cgiFileFormats;
	Env& _env;

	RequestHandler(Config* config, Env& env);

	~RequestHandler();

	void formResponse(WebClient* client);

private:
	const string& mimeType(const string& uri);

	bool isBadRequest(Request* request) const;

	bool isProtocolSupported(const string& protocol) const;

	void readfile(Response* response, const string&);

	void folderContents(Response* response, const string&, const string&);

	void setStatusLine(Response* response);

	void fillHeaders(Response*, LocationContext*);

	string getPathFromUri(const string& uri, LocationContext* location) const;

	bool isFileShouldBeHandleByCGI(string& pathToFile) const;

	void setBodyForStatusCode(Response* response, LocationContext* location);

public:
	void doPost(LocationContext* location, Request* request, Response* response);

	void doGet(LocationContext* location, Request* request, Response* response, string& pathToFile);

	void doDelete(LocationContext* location, Request* request, Response* response);
};

#endif
