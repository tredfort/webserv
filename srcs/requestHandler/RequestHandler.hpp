#pragma once

#include <fstream>
#include <exception>
#include <vector>
#include <map>
#include <string>
#include <sys/stat.h>
#include <sys/socket.h>


#include "../interfaces/IRequestHandler.hpp"
#include "../model/WebClient.hpp"
#include "../model/Request.hpp"
#include "../config/Config.hpp"


class RequestHandler : public IRequestHandler {

private:
    WebClient* client;
    std::map<std::string, std::string> types;
    Request::Method _method;
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

    std::string &getFilePath();

private:
    const std::string& mimeType(const std::string& uri);

    void readfile(const std::string &);

    Config* _config;

public:
    void formResponse(WebClient *);
	void doPost(WebClient*);
	void doGet(WebClient*);
	void doPut(WebClient*);
	void doDelete(WebClient*);

    //    RequestHandler();
    RequestHandler(Config* config);
    //    RequestHandler(Request &request, IConfig *conf);
    ~RequestHandler();
};