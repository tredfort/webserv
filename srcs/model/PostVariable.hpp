#ifndef WEBSERVER_POSTVARIABLE_HPP
#define WEBSERVER_POSTVARIABLE_HPP

#include "../utils/utils.hpp"
#include <cstring>
#include <map>
#include <vector>

using std::map;
using std::string;
using std::vector;

class PostVariable {
private:
	map<string, string> _headings;
	string _body;

public:
	PostVariable(string value);
	map<string, string> getHeadings();
	string getBody();
	string getHeader(string header);
};

#endif
