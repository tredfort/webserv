#ifndef CGI_HPP
#define CGI_HPP

#include "../interfaces/ICGI.hpp"
#include "../server/Env.hpp"
#include "../utils/usings.hpp"
#include "../utils/utils.hpp"
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

public:
	CGI(Env& env);
	~CGI();

	// throws exceptions
	CGIModel getPathToFileWithResult(string pathToExecFile);
	bool isFileShouldBeHandleByCGI(string pathToExecFile) const;

	struct FileDoesNotExist : public std::exception {
		virtual const char* what(void) const throw();
	};

	struct FileFormatUnsupported : public std::exception {
		virtual const char* what(void) const throw();
	};

	struct BadAlloc : public std::exception {
		virtual const char* what(void) const throw();
	};

private:
	// throws exceptions
	string getFileFormat(string pathToExecFile) const;
	CGIModel executeCgi(const ExecveArguments& execArguments);
	char** configureArgumentsForComand(string pathToExecFile) const;
	bool openOutputFile(std::string file);
	void clearEverything(ExecveArguments* arguments);
	CGIModel constructCGIResult(int code, bool isSuccessful, string path);
	ExecveArguments* constructExecveArguments(string pathToExecFile);
	string constructExecutablePath(string format);
	bool createSharedMemory();
	void freeSharedMemory();
	char** getEnvAsCstrArray() const;
};

#endif