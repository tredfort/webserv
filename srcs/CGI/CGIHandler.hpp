#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <fcntl.h>
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>

#include "../model/Request.hpp"
#include "../requestHandler/FileReader.hpp"
#include "../utils/utils.hpp"

#define CGI_OUTPUT "outputCGI"

class CGIHandler {
private:
	static const int CHILD_PID = 0;
	static const int FAILURE_CODE = -1;
	string _cgiName;

public:
	int handle(const string& cgiName, const string& pathToFile, Request* request);
	std::string getBody();

private:
	void executeCgi(const string& cgiName, const string& pathToFile, Request* request);
	int cgiParent(pid_t pid);
	void addToEnv(char** env, const string& newEnvValue);
	string generateNewEnv(const string& key, const string& value);
	char** generateEnv(Request* request);
	long calculateFileSize(const string& fileName) const;
};

#endif
