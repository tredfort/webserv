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
	// tmp
	LocationContext _location;

	RequestHandler(Config* config, Env& env);

	~RequestHandler();

	void formResponse(WebClient* client);

private:
	vector<string> index;
	string locationPath;
	CGI cgi;

	const string& mimeType(const string& uri);

	bool isBadRequest(Request* request) const;

	bool isProtocolSupported(const string& protocol) const;

	void readfile(Response* response, const string&);

	void folderContents(Response* response, const string&, const string&);

	bool fillBodyFromIndexFile(Response*, const string&, const LocationContext*);

	void setResponseWithError(Response* response, string errorMessage, string pathToErrorPage);

	void fillHeaders(Response* response);

public:
	void doPost(Request* request, Response* response);

	void doGet(LocationContext* location, Request* request, Response* response);

	void doPut(Request* request, Response* response);

	void doDelete(Request* request, Response* response);

	string getErrorPage(string errorMessage);
};

#endif
