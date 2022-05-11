#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../interfaces/IResponse.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

//using std::make_pair;
//using std::map;
using std::string;
using std::vector;

class Response : public IResponse {
private:
	string _protocol;
	string _status;
	string _contentType;
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

	void setStatus(string status);

	void setContentType(const string& contentType);

	void setBuffer(const string& buffer);

	void setHeader(const string& header);

	void setBody(const string& body);

	const string& getProtocol() const;

	const string& getStatus() const;

	const string& getBuffer() const;

	const string& getContentType() const;

	const string& getBody() const;

private:
	Response(const Response&);

	Response& operator=(const Response&);
};

#endif
