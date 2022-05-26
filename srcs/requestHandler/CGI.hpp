#ifndef CGI_HPP
#define CGI_HPP

#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include "../utils/usings.hpp"
#include "../interfaces/ICGI.hpp"
#include "../utils/utils.hpp"
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>

class CGI : public ICGI {

private:
	const string	_pathToExecFile;
	map<string, string>	supportedFileFormats;
	const string	rootWebserv;
	const string		cgiFolder;
	int				_outputFileFd;

public:
	CGI(string pathToFile);
	~CGI();

	// throws exceptions
	bool	getPathToFileWithResult(string & path);
	bool	isFileShouldBeHandleByCGI() const;

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
	void validateFile() const;
	string getFileFormat() const;
	string executeCgi();
	char**	configureArgumentsForComand() const;
	bool	openOutputFile(std::string file);
	void	clearEverything(char** args);
};

#endif