#include "CGI.hpp"

CGI::CGI(string pathToFile) :
	_pathToExecFile(pathToFile)
{
	supportedFileFormats["py"] = "python";
	supportedFileFormats["php"] = "php";
}

CGI::~CGI() { }

string	CGI::getPathToFileWithResult() const {
	validateFile();
	return ("biba");
}

// Private methods

void	CGI::validateFile() const {
	if (!isFileExists(_pathToExecFile)) {
		throw CGI::FileDoesNotExist();
	}
	string format = getFileFormat();
	if (supportedFileFormats.find(format) == supportedFileFormats.end()) {
		throw CGI::FileFormatUnsupported();
	}
}

string	CGI::getFileFormat() const {
	unsigned long i = 0;
	for (i = _pathToExecFile.length(); i >= 0 && _pathToExecFile[i] != '.'; i--) { }
	if (_pathToExecFile[i] != '.' && i != _pathToExecFile.length())
		return "";
	return _pathToExecFile.substr(++i, _pathToExecFile.length());
}

const char *	CGI::FileDoesNotExist::what() const throw() {
	return "class - CGI\nmethod - getPathToFileWithResult\n Error - File with path: - does not exist";
}

const char *	CGI::FileFormatUnsupported::what() const throw() {
	return "class - CGI\nmethod - getPathToFileWithResult\n Error - File Format is unsupported";
}
