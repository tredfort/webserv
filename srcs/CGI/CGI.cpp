#include "CGI.hpp"

CGI::CGI(string pathToFile, Env & env) :
	_pathToExecFile(pathToFile),
	_cgiFolder("resources/html_data/cgi/"),
	_env(env)
{
	supportedFileFormats["py"] = "python";
	supportedFileFormats["php"] = "php";
}

CGI::~CGI() { }

CGIModel	CGI::getPathToFileWithResult() {

	ExecveArguments *arguments = constructExecveArguments();
	if (!arguments)
		return constructCGIResult(500, false, "");

	CGIModel result = executeCgi(*arguments);
	clearEverything(arguments);
	return result;
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

string	CGI::getFileFormat() const {
	unsigned long i = 100;
	cout << _pathToExecFile.size() << endl;
	for (i = _pathToExecFile.size(); i > 0 && _pathToExecFile[i] != '.'; i--) { }
	if (_pathToExecFile[i] != '.' && i != _pathToExecFile.length())
		return "";
	return _pathToExecFile.substr(++i, _pathToExecFile.length());
}

// TODO:: ADD env variable
ExecveArguments *	CGI::constructExecveArguments() {

	ExecveArguments *	arguments = new ExecveArguments[1];
	char **args = configureArgumentsForComand();

	if (!args) {
		return NULL;
	}
	if (!openOutputFile(_cgiFolder + "fileCgi.html")) {
		clearEverything(arguments);
		return NULL;
	}
	string format = getFileFormat();
	map<string, string>::iterator fileFormat = supportedFileFormats.find(format);
	if (fileFormat == supportedFileFormats.end()) {
		clearEverything(arguments);
		return NULL;
	}
	string pathToExecutable = constructExecutablePath(fileFormat->second);
	if (pathToExecutable.empty()) {
		clearEverything(arguments);
		return NULL;
	}
	arguments->args = args;
	arguments->env = NULL;
	arguments->pathToExecutable = strdup(const_cast<char *>(pathToExecutable.c_str()));
	// arguments->pathToExecutable = strdup("/usr/bin/python");
	return arguments;
}

// если делай throw то не забывай чистить все говно
CGIModel CGI::executeCgi(const ExecveArguments & execArguments) {
	
	int saveStdout = dup(STDOUT_FILENO);

	if (!createSharedMemory()) {
		return constructCGIResult(500, false, "");
	}
	_sharedMemory[0] = 1;
	pid_t pid = fork();
	if (pid == -1) {
		return constructCGIResult(500, false, "");
	} else if (!pid) {
		dup2(_outputFileFd, STDOUT_FILENO);
		if (execve(execArguments.pathToExecutable, execArguments.args, execArguments.env) == -1) {
			_sharedMemory[0] = 0;
			exit(1);
		}
	} else {
		waitpid(-1, NULL, 0);
	}
	if (!pid)
		exit(0);
	dup2(saveStdout, STDOUT_FILENO);
	int	execResult = _sharedMemory[0];
	freeSharedMemory();
	if (execResult == 0)
		return constructCGIResult(500, false, "");
	return constructCGIResult(200, true, _cgiFolder + "fileCgi.html");
}

void	CGI::clearEverything(ExecveArguments * arguments) {
	close(_outputFileFd);
	if (arguments) {
		delete[] arguments;
	}
}

bool	CGI::openOutputFile(std::string file) {
	_outputFileFd = open(file.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
	if (_outputFileFd == -1)
		return (false);
	return (true);
}

char**	CGI::configureArgumentsForComand() const {
	char **args = NULL;

	try {
		args = new char*[3];
		args[0] = new char[1];
		args[0] = strcpy(args[0], "");
		args[1] = new char[_pathToExecFile.size() + 1];
		args[1] = strcpy(args[1], _pathToExecFile.c_str());
		args[2] = NULL;
	} catch (std::bad_alloc &e) {
		std::cerr << "CGI, configureArgumentsForComand - " << e.what() << std::endl;
		return NULL;
	}
	return args;
}

void	CGI::setPathToFile(string str) {
	_pathToExecFile = str;
}

CGIModel	CGI::constructCGIResult(int code, bool isSuccessful, string path) {
	CGIModel result = CGIModel();
	result.code = code;
	result.isSuccess = isSuccessful;
	result.pathToFile = path;
	return result;
}

string		CGI::constructExecutablePath(string format) {
	vector<string> paths = _env.getPaths();
	for (unsigned int i = 0; i < paths.size(); i++) {
		string fullPath = paths[i] + "/" + format;
		if (isFileExists(fullPath)) {
			return fullPath;
		}
	}
	return "";
}

bool		CGI::createSharedMemory() {
	_shmFd = shm_open("exec_result", O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);
	if (_shmFd < 0)
		return false;
	ftruncate(_shmFd, sizeof(int));
	_sharedMemory = (int *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, _shmFd, 0);
	if (_sharedMemory == NULL) {
		close(_shmFd);
		return false;
	}
	return true;
}

void		CGI::freeSharedMemory() {
	shm_unlink("exec_result");
	close(_shmFd);
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