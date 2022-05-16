#ifndef CGI_HPP
#define CGI_HPP

#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include "../utils/usings.hpp"
#include "../interfaces/ICGI.hpp"
#include "../utils/utils.hpp"

class CGI : public ICGI {

private:
	const string	_pathToExecFile;
	map<string, string>	supportedFileFormats;

public:
	CGI(string pathToFile);
	~CGI();

	// throws exceptions
	string	getPathToFileWithResult() const;

	struct FileDoesNotExist : public std::exception {
		virtual const char *	what(void) const throw();
	};

	struct FileFormatUnsupported : public std::exception {
		virtual const char *	what(void) const throw();
	};

private:
	// throws exceptions
	void validateFile() const;
	string getFileFormat() const;
};

#endif