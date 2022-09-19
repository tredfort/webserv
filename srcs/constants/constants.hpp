#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstring>
#include <iostream>
#include <map>

using std::map;
using std::string;

void fillTypes(map<string, string>& types);

namespace defaults {

const int BUFFER_SIZE = 1024 * 1024;
const string HOSTNAME = "localhost";
const string UNLOAD_PATH = "html/unloadPath";
const string ROOT = "html/";
const int CLIENT_MAX_BODY_SIZE = INT32_MAX;
const int POLL_TIMEOUT = 1000;
const bool AUTOINDEX = false;
const string CGI_FOLDER = "testInstructions/cgi/tmp/";
const string SERVER_NAME = "webserv/1.0";

}

#endif