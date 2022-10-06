#include "CGIHandler.hpp"

int CGIHandler::handle(const std::string& cgiPath, const std::string& pathToFile, Request* request)
{
	_cgiPath = cgiPath;
	pid_t pid = fork();
	if (pid == FAILURE_CODE) {
		return 500;
	} else if (pid == CHILD_PID) {
		executeCgi(cgiPath, pathToFile, request);
	}
	return cgiParent(pid);
}

void CGIHandler::executeCgi(const std::string& cgiPath, const std::string& pathToFile, Request* request)
{
	char** env = generateEnv(request);
	if (env == NULL) {
		exit(2);
	}
	int in = open(pathToFile.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);
	int out = open(defaults::CGI_TMP_FILE.c_str(), O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

	char* argv[2];
	argv[0] = (char*)cgiPath.c_str();
	argv[1] = NULL;
	if (in == -1 || out == -1) {
		exit(2);
	}
	if (dup2(in, 0) == -1 || dup2(out, 1) == -1) {
		exit(2);
	}
	execve(cgiPath.c_str(), argv, env);
	exit(1);
}

char** CGIHandler::generateEnv(Request* request)
{
	char** env;
	std::map<std::string, std::string> headers = request->_headers;

	try {
		env = new char*[headers.size() + 4]();
	} catch (std::exception& e) {
		return NULL;
	}

	addToEnv(env, "REQUEST_METHOD=" + request->getMethod());
	addToEnv(env, "SERVER_PROTOCOL=" + request->getProtocol());
	addToEnv(env, "PATH_INFO=" + request->getUri());

	std::map<std::string, std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++) {
		addToEnv(env, generateNewEnv(it->first, it->second));
	}

	return env;
}

int CGIHandler::cgiParent(pid_t pid)
{
	int status;
	std::stringstream str;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status)) {
		status = WEXITSTATUS(status);
	}
	if (status == EXIT_SUCCESS) {
		std::ifstream inputCGI(defaults::CGI_TMP_FILE, std::ifstream::in);
		if (!inputCGI.is_open()) {
			return 500;
		}
		return 200;
	}
	if (status == EXIT_FAILURE) {
		std::remove(defaults::CGI_TMP_FILE.c_str());
		return 502;
	} else {
		return 500;
	}
}

void CGIHandler::addToEnv(char** env, const std::string& newEnvValue)
{
	while (*env) {
		env++;
	}
	*env = strdup(newEnvValue.c_str());
}

int toUnderscore(int c)
{
	if (c == '-') {
		return '_';
	}
	return c;
}

std::string CGIHandler::generateNewEnv(const std::string& key, const std::string& value)
{
	std::string buf(key);
	transform(buf.begin(), buf.end(), buf.begin(), ::toupper);
	transform(buf.begin(), buf.end(), buf.begin(), toUnderscore);
	buf = "HTTP_" + buf + "=" + value;
	return buf;
}

std::streamsize CGIHandler::calculateFileSize(const string& fileName) const
{
	std::ifstream srcFile(fileName, std::ifstream::in);

	if (!srcFile.is_open())
		return 0;

	srcFile.seekg(0, srcFile.end);
	std::streamsize size = srcFile.tellg();
	srcFile.seekg(0, srcFile.beg);
	srcFile.close();

	if (static_cast<int>(size) == -1)
		return 0;
	return size;
}

std::string CGIHandler::getBody()
{
	std::string body;

	if (getFileName(_cgiPath) == "cgi_tester") {
		std::ifstream file(defaults::CGI_TMP_FILE, std::ifstream::in);
		body.reserve(calculateFileSize(defaults::CGI_TMP_FILE));
		file.seekg(58);
		body.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		file.close();
	} else {
		body = FileReader::readFile(defaults::CGI_TMP_FILE);
	}
	std::remove(defaults::CGI_TMP_FILE.c_str());

	return body;
}