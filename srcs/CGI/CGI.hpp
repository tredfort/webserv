#ifndef CGI_HPP
#define CGI_HPP

#include "../interfaces/ICGI.hpp"
#include "../server/Env.hpp"
#include "../utils/usings.hpp"
#include "../utils/utils.hpp"
#include "../model/Request.hpp"
#include "../config/Config.hpp"
#include "CGIModel.hpp"
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

class CGI : public ICGI {

private:
	map<string, string> supportedFileFormats;
	const string _cgiFolder;
	int _outputFileFd;
	Env _env;
	int _shmFd;
	int* _sharedMemory;
	map<string, string>	_cgiEnv;
	string pathToFile;
	string query;
	string format;
	LocationContext* _location;

public:
	CGI(Request & request, string path, Env &env, LocationContext* location);
	CGI(Env& env);
	~CGI();

	CGIModel getPathToFileWithResult();
	bool isFileShouldBeHandleByCGI(string& pathToFile) const;

private:
	void initCgiEnv(Request & request, string path);
	char **getEnvAsCstrArray() const;
	string getFileFormat(string pathToExecFile) const;
	CGIModel executeCgi(const ExecveArguments& execArguments);
	char** configureArgumentsForComand() const;
	bool openOutputFile(std::string file);
	void clearEverything(ExecveArguments* arguments);
	CGIModel constructCGIResult(int code, bool isSuccessful, string path);
	ExecveArguments* constructExecveArguments();
	string constructExecutablePath(string format);
	bool createSharedMemory();
	void freeSharedMemory();
	string getPathInfo(string pathToExecFile) const;
	string getPathToFile(string path);
};

#endif