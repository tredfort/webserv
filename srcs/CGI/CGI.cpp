#include "CGI.hpp"

CGI::CGI(Request& request, const string& uri, Env& env)
	: _cgiFolder(defaults::CGI_FOLDER)
	, _env(env)
{
	vector<string> result = ft_split(uri, "?");
	string path = result[0];
	query = result.size() == 2 ? result[1] : "";
	_pathToFile = path;
	initCgiEnv(request, path);
}

CGI::~CGI() { }

CGIModel CGI::getPathToFileWithResult(LocationContext* location)
{

	ExecveArguments* arguments = constructExecveArguments(location);
	if (!arguments)
		return constructCGIResult(500, false, "");

	CGIModel result = executeCgi(*arguments);
	clearEverything(arguments);
	return result;
}

// Private methods

void CGI::initCgiEnv(Request& request, string path)
{
	for (map<string, string>::iterator it = request._headers.begin();
		 it != request._headers.end();
		 ++it) {
		string key = replace(it->first, "-", "_");
		transform(key.begin(), key.end(), key.begin(), ::toupper);
		_cgiEnv.insert(make_pair("HTTP_" + key, it->second));
	}
	_cgiEnv.insert(make_pair("REQUEST_METHOD", request.getMethod()));
	_cgiEnv.insert(make_pair("SERVER_PROTOCOL", request.getProtocol()));
	_cgiEnv.insert(make_pair("PATH_INFO", "/" + path));
//	_cgiEnv.insert(make_pair("REQUEST_URI", path));
//	_cgiEnv["REDIRECT_STATUS"] = "200";
//	_cgiEnv["GATEWAY_INTERFACE"] = "CGI/1.1";
//	_cgiEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
//	_cgiEnv["SERVER_SOFTWARE"] = defaults::SERVER_NAME;
//	_cgiEnv["REQUEST_METHOD"] = request.getMethod();
//	_cgiEnv["REQUEST_URI"] = request.getUri();
//	_cgiEnv["PATH_INFO"] = getPathInfo(path);
//	_cgiEnv["PATH_TRANSLATED"] = getPathInfo(path);
//	_cgiEnv["QUERY_STRING"] = query;
//	string header = request.getHeader("Authorization");
//	if (!header.empty()) {
//		_cgiEnv["AUTH_TYPE"] = header;
//		_cgiEnv["REMOTE_IDENT"] = header;
//		_cgiEnv["REMOTE_USER"] = header;
//	}
//	header = request.getHeader("Content-Type");
//	if (!header.empty()) {
//		_cgiEnv["CONTENT_TYPE"] = header;
//	} else {
//		_cgiEnv["CONTENT_TYPE"] = "text/plain";
//	}
//	_cgiEnv["SERVER_NAME"] = request.getHeader("Host");
}

char** CGI::getEnvAsCstrArray() const
{
	char** env = new char*[_cgiEnv.size() + 1];
	int j = 0;
	for (std::map<std::string, std::string>::const_iterator it = _cgiEnv.begin(); it != _cgiEnv.end(); it++) {
		std::string element = it->first + "=" + it->second;
		env[j] = strdup(element.c_str());
		j++;
	}
	env[j] = NULL;
	return env;
}

//string CGI::getPathInfo(string fullPath) const
//{
//	if (fullPath.size() > _pathToFile.size()) {
//		string pathInfo = fullPath.erase(0, _pathToFile.size());
//		return pathInfo;
//	}
//	return "";
//}

ExecveArguments* CGI::constructExecveArguments(LocationContext* location)
{

	ExecveArguments* arguments = new ExecveArguments[1];
	char** args = configureArgumentsForComand();

	if (!args) {
		return NULL;
	}
	if (!openOutputFile(createPath(_cgiFolder, "fileCgi.html"))) {
		clearEverything(arguments);
		return NULL;
	}
	const string& pathToExecutable = location->getCgiPath();
	if (pathToExecutable.empty()) {
		clearEverything(arguments);
		return NULL;
	}
	arguments->args = args;
	arguments->env = getEnvAsCstrArray();
	arguments->pathToExecutable = strdup(pathToExecutable.c_str());
	return arguments;
}

CGIModel CGI::executeCgi(const ExecveArguments& execArguments)
{

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
	int execResult = _sharedMemory[0];
	freeSharedMemory();
	if (execResult == 0)
		return constructCGIResult(500, false, "");
	return constructCGIResult(200, true, createPath(_cgiFolder, "fileCgi.html"));
}

void CGI::clearEverything(ExecveArguments* arguments)
{
	close(_outputFileFd);
	if (arguments) {
		delete[] arguments;
	}
}

bool CGI::openOutputFile(std::string file)
{
	_outputFileFd = open(file.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
	if (_outputFileFd == -1)
		return (false);
	return (true);
}

char** CGI::configureArgumentsForComand() const
{
	char** args = NULL;

	try {
		args = new char*[3];
		args[0] = new char[1];
		args[0] = strcpy(args[0], "");
		args[1] = new char[_pathToFile.size() + 1];
		args[1] = strcpy(args[1], _pathToFile.c_str());
		args[2] = NULL;
	} catch (std::bad_alloc& e) {
		std::cerr << "CGI, configureArgumentsForComand - " << e.what() << std::endl;
		return NULL;
	}
	return args;
}

CGIModel CGI::constructCGIResult(int code, bool isSuccessful, string path)
{
	CGIModel result = CGIModel();
	result.code = code;
	result.isSuccess = isSuccessful;
	result.pathToFile = path;
	return result;
}

bool CGI::createSharedMemory()
{
	freeSharedMemory();
	_shmFd = shm_open("exec_result", O_CREAT | O_EXCL | O_RDWR, S_IRWXU | S_IRWXG);
	if (_shmFd < 0)
		return false;
	ftruncate(_shmFd, sizeof(int));
	_sharedMemory = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, _shmFd, 0);
	if (_sharedMemory == NULL) {
		close(_shmFd);
		return false;
	}
	return true;
}

void CGI::freeSharedMemory()
{
	shm_unlink("exec_result");
	close(_shmFd);
}
