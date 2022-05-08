#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../interfaces/IResponse.hpp"
#include "Protocol.hpp"
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
	Protocol _protocol;
	string _status;
	string _buffer;
	map<string, string> _headers;

public:
	//    std::string fileName;
	//    std::ifstream file;
	//    long fileSize;
	//    bool headersSent;
	//	std::string toSend;

public:
	Response();
	~Response();

	std::string toString() const;

private:
	Response(const Response&);
	Response& operator=(const Response&);
};

#endif
