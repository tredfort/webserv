#pragma once

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
#include "../model/WebClient.hpp"
#include "FileReader.hpp"

class RequestHandler : public IRequestHandler {
private:
	WebClient* client;
	std::map<std::string, std::string> types;
	Method _method;
	std::string _uri;
	std::vector<std::string> _index;
	std::map<std::string, std::string> _header_fields;
	std::string _header;
	std::vector<std::string> _requestHeaders;

	int status_code;
	unsigned int content_lengh;
	std::string toSend;
	std::string body;
	std::string filepath;

private:
	const std::string& mimeType(const std::string& uri);

	bool isBadRequest(Request* request) const;

	void readfile(const std::string&);

public:
	RequestHandler();

	~RequestHandler();

	void formResponse(WebClient* client);

	void doPost(WebClient* client);

	void doGet(WebClient* client);

	void doPut(WebClient* client);

	void doDelete(WebClient* client);
};