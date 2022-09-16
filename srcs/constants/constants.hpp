#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstring>
#include <iostream>
#include <map>

using std::map;
using std::string;

#define BUFFER_SIZE 1024 * 1024

#define DELIMITER "\r\n"

#define POLL_TIMEOUT 1000

#define HOSTNAME "localhost"

#define ROOT_DEFAULT "html/"

void fillTypes(map<string, string>& types);

#define GET_METHOD "GET"
#define POST_METHOD "POST"
#define PUT_METHOD "PUT"
#define DELETE_METHOD "DELETE"

namespace method {

const string GET = "GET";
const string POST = "POST";
const string DELETE = "DELETE";
const string PUT = "PUT";

}

namespace defaults {

const string UNLOAD_PATH = "html/unloadPath";
const string ROOT = "html/";

}

#endif