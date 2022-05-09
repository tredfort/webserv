#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../interfaces/IResponse.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using std::make_pair;
using std::map;
using std::string;
using std::vector;

class Response : public IResponse {
private:
	string _protocol;
	string _status;
	string _buffer;
	map<string, string> _headers;

public:
	//    std::string fileName;
	//    std::ifstream file;
	//    long fileSize;
	//    bool headersSent;
	string toSend;

public:
	Response();
	~Response();

	string toString() const;

private:
	Response(const Response&);
	Response& operator=(const Response&);
};

#endif
