#include "CGI.hpp"

CGI::CGI(Request & request, Config & config, Env &env) :
	_cgiFolder("resources/html_data/cgi/"),
	_env(env)
{
	supportedFileFormats["py"] = "python";
	supportedFileFormats["php"] = "php";
	initCgiEnv(request, config);
}

CGI::~CGI() { }

CGIModel	CGI::getPathToFileWithResult(string pathToExecFile) {

	ExecveArguments *arguments = constructExecveArguments(pathToExecFile);
	cout << endl << pathToExecFile << endl << endl;
	if (!arguments)
		return constructCGIResult(500, false, "");

	CGIModel result = executeCgi(*arguments);
	clearEverything(arguments);
	return result;
}

bool	CGI::isFileShouldBeHandleByCGI(string pathToExecFile) const {
	// TODO: Переделай так как нужно проверять еще на индексные файлы
	if (!isFileExists(pathToExecFile)) {
		return false;
	}
	string format = getFileFormat(pathToExecFile);
	if (supportedFileFormats.find(format) == supportedFileFormats.end()) {
		return false;
	}
	// TODO: think of other cases
	return true;
}

// Private methods

void CGI::initCgiEnv(Request & request, Config & config) {
	(void)config;
	_cgiEnv["REDIRECT_STATUS"] = "200";
	_cgiEnv["GATEWAY_INTERFACE"] = "CGI/1.1";
	_cgiEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
	_cgiEnv["SERVER_SOFTWARE"] = "Webserv/1.0";
	_cgiEnv["REQUEST_METHOD"] = request.getMethod();
	_cgiEnv["REQUEST_URI"] = request.getUri();
	string header = request.getHeader("Authorization");
	if (!header.empty()) {
		_cgiEnv["AUTH_TYPE"] = header;
	}
	header = request.getHeader("Content-Type");
	if (!header.empty()) {
		_cgiEnv["CONTENT_TYPE"] = header;
	}
	_cgiEnv["SERVER_NAME"] = request.getHeader("Host");
	
	cout << endl << "           ENV              " << endl; 
	for (map<string, string>::iterator i = _cgiEnv.begin(); i != _cgiEnv.end(); i++) {
		cout << i->first << " = " << i->second << endl;
	}
	cout << endl << "                         " << endl; 
	for (map<string, string>::iterator i = request._headers.begin(); i != request._headers.end(); i++) {
		cout << i->first << " = " << i->second << endl;
	}
	cout << endl << "                         " << endl; 
}

char **CGI::getEnvAsCstrArray() const {
	char	**env = new char*[_cgiEnv.size() + 1];
	int	j = 0;
	for (std::map<std::string, std::string>::const_iterator i = this->_cgiEnv.begin(); i != this->_cgiEnv.end(); i++) {
		std::string	element = i->first + "=" + i->second;
		env[j] = new char[element.size() + 1];
		env[j] = strcpy(env[j], (const char*)element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}

string	CGI::getFileFormat(string pathToExecFile) const {
	unsigned long i = 100;
	cout << pathToExecFile.size() << endl;
	for (i = pathToExecFile.size(); i > 0 && pathToExecFile[i] != '.'; i--) { }
	if (pathToExecFile[i] != '.' && i != pathToExecFile.length())
		return "";
	return pathToExecFile.substr(++i, pathToExecFile.length());
}

// TODO:: ADD env variable
ExecveArguments *	CGI::constructExecveArguments(string pathToExecFile) {

	ExecveArguments *	arguments = new ExecveArguments[1];
	char **args = configureArgumentsForComand(pathToExecFile);

	if (!args) {
		return NULL;
	}
	if (!openOutputFile(_cgiFolder + "fileCgi.html")) {
		clearEverything(arguments);
		return NULL;
	}
	string format = getFileFormat(pathToExecFile);
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
	arguments->env = getEnvAsCstrArray();
	arguments->pathToExecutable = strdup(const_cast<char *>(pathToExecutable.c_str()));
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

char**	CGI::configureArgumentsForComand(string pathToExecFile) const {
	char **args = NULL;

	try {
		args = new char*[3];
		args[0] = new char[1];
		args[0] = strcpy(args[0], "");
		args[1] = new char[pathToExecFile.size() + 1];
		args[1] = strcpy(args[1], pathToExecFile.c_str());
		args[2] = NULL;
	} catch (std::bad_alloc &e) {
		std::cerr << "CGI, configureArgumentsForComand - " << e.what() << std::endl;
		return NULL;
	}
	return args;
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