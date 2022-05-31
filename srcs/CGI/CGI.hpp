#ifndef CGI_HPP
#define CGI_HPP

#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include "../utils/usings.hpp"
#include "../interfaces/ICGI.hpp"
#include "../utils/utils.hpp"
#include "CGIModel.hpp"
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <string.h>
#include "../server/Env.hpp"
#include <sys/mman.h>

class CGI : public ICGI {

private:
	string				_pathToExecFile;
	map<string, string>	supportedFileFormats;
	const string	_cgiFolder;
	int				_outputFileFd;
	Env				_env;
	int				_shmFd;
	int*			_sharedMemory;

public:
	CGI(string pathToFile, Env& env);
	~CGI();

	// throws exceptions
	CGIModel	getPathToFileWithResult();
	bool	isFileShouldBeHandleByCGI() const;
	void	setPathToFile(string str);

	struct FileDoesNotExist : public std::exception {
		virtual const char *	what(void) const throw();
	};

	struct FileFormatUnsupported : public std::exception {
		virtual const char *	what(void) const throw();
	};

	struct BadAlloc : public std::exception {
		virtual const char *	what(void) const throw();
	};

private:
	// throws exceptions
	string		getFileFormat() const;
	CGIModel	executeCgi(const ExecveArguments & execArguments);
	char**		configureArgumentsForComand() const;
	bool		openOutputFile(std::string file);
	void		clearEverything(ExecveArguments * arguments);
	CGIModel	constructCGIResult(int code, bool isSuccessful, string path);
	ExecveArguments *	constructExecveArguments();
	string		constructExecutablePath(string format);
	bool		createSharedMemory();
	void		freeSharedMemory();
};

#endif