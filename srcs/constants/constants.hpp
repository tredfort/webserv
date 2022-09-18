#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstring>
#include <iostream>
#include <map>

using std::map;
using std::string;

void fillTypes(map<string, string>& types);

namespace method {

const string GET = "GET";
const string POST = "POST";
const string DELETE = "DELETE";
const string PUT = "PUT";

}

namespace defaults {

const int BUFFER_SIZE = 1024 * 1024;
const string HOSTNAME = "localhost";
const string UNLOAD_PATH = "html/unloadPath";
const string ROOT = "html/";
const int CLIENT_MAX_BODY_SIZE = INT32_MAX;
const int POLL_TIMEOUT = 1000;
const bool AUTOINDEX = false;

}

#endif