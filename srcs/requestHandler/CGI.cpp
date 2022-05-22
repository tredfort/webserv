#include "CGI.hpp"

CGI::CGI(string pathToFile) :
	_pathToExecFile(pathToFile)
{
	supportedFileFormats["py"] = "python";
	supportedFileFormats["php"] = "php-cgi";
}

CGI::~CGI() { }

string	CGI::getPathToFileWithResult() const {
	validateFile();
	string biba = executeCgi();
	return (biba);
}

// Private methods

void	CGI::validateFile() const {
	cout << _pathToExecFile << endl;
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

std::string CGI::executeCgi() const {
	string fullPathToFile = "/Users/chudapak/code/42/webserv/webserv/" + _pathToExecFile;
	char **args = NULL;

	args = new char*[2];
	args[0] = new char[1];
	args[0] = strcpy(args[0], "");
	args[1] = new char[fullPathToFile.size() + 1];
	args[1] = strcpy(args[1], fullPathToFile.c_str());

	pid_t pid = fork();

	if (pid == -1)
	{
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid)
	{
		execve("/usr/bin/python", args, nullptr);
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		waitpid(-1, NULL, 0);
		cout << "waiting" << endl;
	}
	return ("");
}

const char *	CGI::FileDoesNotExist::what() const throw() {
	return "class - CGI\nmethod - getPathToFileWithResult\n Error - File with path: - does not exist";
}

const char *	CGI::FileFormatUnsupported::what() const throw() {
	return "class - CGI\nmethod - getPathToFileWithResult\n Error - File Format is unsupported";
}
