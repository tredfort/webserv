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
	int _statusCode;
	string _buffer;
	vector<string> _headers;
	string _body;

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

	void setProtocol(const string& protocol);

	void setStatusCode(int status_code);

	void setBuffer(const string& buffer);

	void setHeader(const string& header);

	void setBody(const string& body);

private:
	Response(const Response&);

	Response& operator=(const Response&);
};

#endif
