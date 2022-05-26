#include "CGI.hpp"

CGI::CGI(string pathToFile) :
	_pathToExecFile(pathToFile),
	rootWebserv("/Users/chudapak/code/42/webserv/webserv/"),
	cgiFolder("resources/html_data/cgi/")
{
	supportedFileFormats["py"] = "/usr/bin/python";
	supportedFileFormats["php"] = "/usr/bin/php";
}

CGI::~CGI() { }

bool	CGI::getPathToFileWithResult(string & path) {
	try {
		string biba = executeCgi();
		path = biba;
		cout << biba << endl << endl;
	} catch (std::bad_alloc & e) {
		cout << e.what() << endl;
		return false;
	} catch (std::exception & e) {
		cout << e.what() << endl;
		return false;
	}
	return (true);
}

bool	CGI::isFileShouldBeHandleByCGI() const {
	if (!isFileExists(_pathToExecFile)) {
		return false;
	}
	string format = getFileFormat();
	if (supportedFileFormats.find(format) == supportedFileFormats.end()) {
		return false;
	}
	// TODO: think of other cases
	return true;
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
	//нуужно сначала слово достатт из пути и потом точку искать
	unsigned long i = 100;
	cout << _pathToExecFile.size() << endl;
	for (i = _pathToExecFile.size(); i > 0 && _pathToExecFile[i] != '.'; i--) { }
	if (_pathToExecFile[i] != '.' && i != _pathToExecFile.length())
		return "";
	return _pathToExecFile.substr(++i, _pathToExecFile.length());
}

// если делай throw то не забывай чистить все говно
std::string CGI::executeCgi() {
	char **args = configureArgumentsForComand();

	if (!args)
		throw CGI::BadAlloc();//переделай на нормальную ошибку
	
	if (!openOutputFile(rootWebserv + cgiFolder + "fileCgi.html"))
		return "";
	
	string format = getFileFormat();
	map<string, string>::iterator pathToExecutable = supportedFileFormats.find(format);
	if (pathToExecutable == supportedFileFormats.end())
		throw CGI::BadAlloc();//переделай на нормальную ошибку
	int saveStdout = dup(STDOUT_FILENO);
	pid_t pid = fork();

	if (pid == -1) {
		throw CGI::BadAlloc();//переделай на нормальную ошибку
	} else if (!pid) {
		dup2(_outputFileFd, STDOUT_FILENO);
		if (execve(pathToExecutable->second.c_str(), args, nullptr) == -1)
			throw CGI::BadAlloc(); //переделай на норм
	} else {
		waitpid(-1, NULL, 0);
		cout << "waiting" << endl;
	}
	dup2(saveStdout, STDOUT_FILENO);
	clearEverything(args);
	if (!pid)
		exit(0);
	return (rootWebserv + cgiFolder + "fileCgi.html");
}

void	CGI::clearEverything(char** args) {
	close(_outputFileFd);
	for (int i = 0; args[i]; i++)
		delete[] args[i];
	delete[] args;
}

bool	CGI::openOutputFile(std::string file) {
	_outputFileFd = open(file.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
	if (_outputFileFd == -1)
		return (false);
	return (true);
}

char**	CGI::configureArgumentsForComand() const {
	char **args = NULL;
	string fullPathToFile = rootWebserv + _pathToExecFile;

	try {
		args = new char*[3];
		args[0] = new char[1];
		args[0] = strcpy(args[0], "");
		args[1] = new char[fullPathToFile.size() + 1];
		args[1] = strcpy(args[1], fullPathToFile.c_str());
		args[2] = NULL;
	} catch (std::bad_alloc &e) {
		std::cerr << "CGI, configureArgumentsForComand - " << e.what() << std::endl;
		return NULL;
	}
	return args;
}

const char *	CGI::FileDoesNotExist::what() const throw() {
	return "class - CGI\nmethod - getPathToFileWithResult\n Error - File with path: - does not exist";
}

const char *	CGI::FileFormatUnsupported::what() const throw() {
	return "class - CGI\nmethod - getPathToFileWithResult\n Error - File Format is unsupported";
}

const char *	CGI::BadAlloc::what() const throw() {
	return "class - CGI\nmethod - getPathToFileWithResult\n Error - Can't allocate memory for cgi";
}