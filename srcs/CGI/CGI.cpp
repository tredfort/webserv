#include "CGI.hpp"

//CGI::CGI(Request & request, string uri, Env &env, LocationContext* location) :
//	_cgiFolder("resources/html_data/cgi/"),
//	_env(env),
//	_location(location)
//{
//	supportedFileFormats["py"] = "python3";
//	supportedFileFormats["php"] = "php";
//	vector<string> result = ft_split(uri, "?");
//	string path = result[0];
//	query = result.size() == 2 ? result[1] : "";
//	pathToFile = getPathToFile(path);
//	initCgiEnv(request, path);
//}

CGI::CGI(Env& env) :
	_cgiFolder("resources/html_data/cgi/"),
	_env(env)
{
	supportedFileFormats["py"] = "python3";
	supportedFileFormats["php"] = "php";
}

CGI::~CGI() { }

void CGI::setParameters(Request* request, LocationContext* location, string& pathToFile)
{
	_location = location;
	_pathToFile = pathToFile;
	initCgiEnv(*request, pathToFile);
	vector<string> result = ft_split(request->getUri(), "?");
	query = result.size() == 2 ? result[1] : "";
}

CGIModel	CGI::getPathToFileWithResult() {

	ExecveArguments *arguments = constructExecveArguments();
	if (!arguments)
		return constructCGIResult(500, false, "");

	CGIModel result = executeCgi(*arguments);
	clearEverything(arguments);
	return result;
}

bool CGI::isFileShouldBeHandleByCGI(string& pathToFile) const {
	if (supportedFileFormats.find(getFileFormat(pathToFile)) != supportedFileFormats.end()) {
		return true;
	}
	return false;
}

// Private methods

void CGI::initCgiEnv(Request& request, string path) {
	_cgiEnv["REDIRECT_STATUS"] = "200";
	_cgiEnv["GATEWAY_INTERFACE"] = "CGI/1.1";
	_cgiEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
	_cgiEnv["SERVER_SOFTWARE"] = "Webserv/1.0";
	_cgiEnv["REQUEST_METHOD"] = request.getMethod();
	_cgiEnv["REQUEST_URI"] = request.getUri();
	_cgiEnv["PATH_INFO"] = getPathInfo(path);
	_cgiEnv["PATH_TRANSLATED"] = getPathInfo(path);
	_cgiEnv["QUERY_STRING"] = query;
	string header = request.getHeader("Authorization");
	if (!header.empty()) {
		_cgiEnv["AUTH_TYPE"] = header;
		_cgiEnv["REMOTE_IDENT"] = header;
		_cgiEnv["REMOTE_USER"] = header;
	}
	header = request.getHeader("Content-Type");
	if (!header.empty()) {
		_cgiEnv["CONTENT_TYPE"] = header;
	}
	_cgiEnv["SERVER_NAME"] = request.getHeader("Host");
	// cout << endl << "           ENV              " << endl; 
	// for (map<string, string>::iterator i = _cgiEnv.begin(); i != _cgiEnv.end(); i++) {
	// 	cout << i->first << " = " << i->second << endl;
	// }
	// cout << endl << "                         " << endl; 
	// for (map<string, string>::iterator i = request._headers.begin(); i != request._headers.end(); i++) {
	// 	cout << i->first << " = " << i->second << endl;
	// }
	// cout << endl << "                         " << endl; 
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

string	CGI::getPathInfo(string fullPath) const {
	if (fullPath.size() > _pathToFile.size()) {
		string pathInfo = fullPath.erase(0, _pathToFile.size());
		return pathInfo;
	}
	return "";
}

string	CGI::getPathToFile(string path) {
	for (map<string, string>::iterator it = supportedFileFormats.begin(); it != supportedFileFormats.end(); it++) {
		size_t i = 0;
		string toFind = "." + it->first;
		while (i != string::npos) {
			if (i == 0) {
				i = path.find(toFind, i);
			} else {
				i = path.find(toFind, i + toFind.size());
			}
			if (i != string::npos) {
				unsigned int size = toFind.size();
				cout << path[i + size] << endl;
				if (path[i + size] == '\0' || path[i + size] == '?' || path[i + size] == '/') {
					string pathToFile = path.erase(i + size, path.size());
					format = it->second;
					return pathToFile;
				}
			}
		}
	}
	const vector<string> indexes = _location->getIndex();
	for (vector<string>::const_iterator it = indexes.begin(), ite = indexes.end(); it != ite; ++it) {
		string pathToIndexFile;
		if ((*it)[0] == '/') {
			pathToIndexFile = _location->getRoot() + *it;
		} else {
			pathToIndexFile = path + *it;
		}
		if (isFileExists(pathToIndexFile) && !isDirectory(pathToIndexFile) && !access(pathToIndexFile.c_str(), W_OK)) {
			return pathToIndexFile;
		}
	}
	return "";
}

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
	string pathToExecutable = constructExecutablePath(format);
	if (pathToExecutable.empty()) {
		clearEverything(arguments);
		return NULL;
	}
	arguments->args = args;
	arguments->env = getEnvAsCstrArray();
	arguments->pathToExecutable = strdup(const_cast<char *>(pathToExecutable.c_str()));
	return arguments;
}

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
		args[1] = new char[_pathToFile.size() + 1];
		args[1] = strcpy(args[1], _pathToFile.c_str());
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
