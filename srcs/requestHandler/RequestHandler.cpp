#include "RequestHandler.hpp"

#define NOTEXIST 0
#define REGFILE 1
#define ISFOLDER 2

#define CGICODE -1

RequestHandler::RequestHandler(Config* config, Env& env)
	: config(config)
	, cgi(CGI(env))
{
	fillTypes(_types);

	//Временные переменные
	vector<string> index;
	index.push_back("index.htm");
	index.push_back("index.html");

	vector<string> allowedMethods;
	allowedMethods.push_back("GET");
	allowedMethods.push_back("POST");
	allowedMethods.push_back("DELETE");

	_location.setAutoIndex(true);
	_location.setClientMaxBodySize(10240);
	_location.setIndex(index);
	_location.setRoot("resources/html_data");
	_location.parseAllowedMethods(allowedMethods);
	//	cout << "***test***" << endl;
	//	location.printConfig();
}

RequestHandler::~RequestHandler() { }

const std::string& RequestHandler::mimeType(const std::string& uri)
{
	std::string::size_type found = uri.find_last_of('.');
	if (found == std::string::npos)
		return _types["txt"];
	std::string extension = uri.substr(found + 1);
	if (_types.find(extension) != _types.end())
		return _types[extension];
	return _types["bin"];
}

void RequestHandler::readfile(Response* response, const std::string& path)
{
	string extension = path.substr(path.find_last_of("/\\") + 1);
	string fileExtension = path.substr(path.find_last_of(".") + 1);

	response->setContentType(mimeType(extension));
	try {
		response->setBody(FileReader::readFile(path));
		response->setStatus("200 OK");
	} catch (FileReader::FileNotFoundException& ex) {
		response->setBody(getErrorPageBody("ДОБРО ПОЖАЛОВАТЬ НА СТРАНИЦУ 404!"));
		response->setStatus("404 NOT FOUND");
	}
}

bool RequestHandler::isBadRequest(Request* request) const { return request->getMethod().empty() || request->getUri().empty() || request->getProtocol().empty(); }

bool RequestHandler::isProtocolSupported(const string& protocol) const { return protocol == "HTTP/1.1"; }

void RequestHandler::formResponse(WebClient* client)
{
	Response* response = client->getResponse();
	Request* request = client->getRequest();
	LocationContext* location = config->getLocationContext(client->getIp(), client->getPort(), request->getHost(), request->getUri());
	if (!location) {
		location = &_location;
	} else {
		cout << "using not null location" << endl;
		location->printConfig();
	}
	response->setProtocol("HTTP/1.1");

	if (isBadRequest(request))
		setResponseWithError(response, "400 Bad Request", location->getErrorPagePath(400));
	else if (!isProtocolSupported(request->getProtocol()))
		setResponseWithError(response, "505 HTTP Version Not Supported", location->getErrorPagePath(505));
	else if (!_location.getAllowedMethods().count(request->getMethod()))
		setResponseWithError(response, "405 Method Not Allowed", location->getErrorPagePath(405));
	else if (request->getMethod() == "GET")
		doGet(location, request, response);
	else if (request->getMethod() == "POST")
		doPost(request, response);
	else if (request->getMethod() == "DELETE")
		doDelete(request, response);
	else if (request->getMethod() == "PUT")
		doPut(request, response);
	else
		setResponseWithError(response, "501 Not Implemented", location->getErrorPagePath(501));
	fillHeaders(response);
}

void RequestHandler::doPost(Request* request, Response* response) { (void)request, (void)response; }

void RequestHandler::doGet(LocationContext* location, Request* request, Response* response)
{
	string pathToFile = location->getRoot() + request->getUri();

	// Check files extension
	// CGI
	// создать свой фаайл записать в него результат компиляции файла и прописать путь в path to file
	string path = pathToFile;
	if (cgi.isFileShouldBeHandleByCGI(path)) {
		cout << "CGIIIIII" << endl;
		CGIModel cgiResult = cgi.getPathToFileWithResult(path);
		if (cgiResult.isSuccess) {
			readfile(response, cgiResult.pathToFile);
		} else {
			setResponseWithError(response, "500 Server Error", location->getErrorPagePath(500));
		}
	} else {
		if (pathToFile.back() == '/' && isDirectory(pathToFile.substr(0, pathToFile.size() - 1))) {
			const bool isBodyFilledFromIndexFile = fillBodyFromIndexFile(response, pathToFile, location);
			if (!isBodyFilledFromIndexFile) {
				if (location->isAutoIndex()) {
					folderContents(response, pathToFile, request->getUri());
				} else {
					setResponseWithError(response, "403 Forbidden", location->getErrorPagePath(403));
				}
			}
			response->setContentType(mimeType(".html"));
		} else if (isFileExists(pathToFile)) {
			readfile(response, pathToFile);
		} else {
			setResponseWithError(response, "404 Not Found", location->getErrorPagePath(404));
		}
	}
}

/**
 * Берёт из локейшен индекс файлы, если находит файл то заносит его в ответ иначе возвращает false и ничего не делает
 * @param response
 * @param pathToDir
 * @return Возвращает true, если файл успешно считан
 */
bool RequestHandler::fillBodyFromIndexFile(Response* response, const string& pathToDir, const LocationContext* location)
{
	const vector<string> indexes = location->getIndex();

	for (vector<string>::const_iterator it = indexes.begin(), ite = indexes.end(); it != ite; ++it) {
		string pathToIndexFile;
		if ((*it)[0] == '/') {
			pathToIndexFile = location->getRoot() + *it;
		} else {
			pathToIndexFile = pathToDir + *it;
		}
		if (isFileExists(pathToIndexFile) && !isDirectory(pathToIndexFile) && !access(pathToIndexFile.c_str(), W_OK)) {
			readfile(response, pathToIndexFile);
			return true;
		}
	}
	return false;
}

void RequestHandler::folderContents(Response* response, const std::string& path, const string& uri)
{
	DIR* dp;
	struct dirent* di_struct;
	struct stat file_stats;
	time_t lastModified;
	string body;

	(void)path;
	string title = "Index of " + uri;
	body.append("<html>\n"
				"<head><title>"
		+ title
		+ "</title></head>\n"
		  "<body>\n"
		  "<h1>"
		+ title + "</h1><hr><pre><a href=\"../\">../</a>\n");
	if ((dp = opendir(path.c_str())) != nullptr) {
		while ((di_struct = readdir(dp)) != nullptr) {
			if (strcmp(di_struct->d_name, ".") && strcmp(di_struct->d_name, "..")) {

				string tmp_path = path + "/" + di_struct->d_name;
				stat(tmp_path.data(), &file_stats);

				body.append("<a href=\"" + uri + string(di_struct->d_name));
				if (S_ISDIR(file_stats.st_mode))
					body.append("/");
				body.append("\">" + string(di_struct->d_name));
				if (S_ISDIR(file_stats.st_mode))
					body.append("/");
				body.append("</a>                                               ");
				lastModified = getFileModificationDate(tmp_path);
				string date = string(ctime(&lastModified));
				date = date.substr(0, date.size() - 1);
				body.append(date + "                   ");
				if (S_ISDIR(file_stats.st_mode))
					body.append("-\n");
				else {
					body.append(std::to_string(static_cast<float>(file_stats.st_size)) + "\n");
				}
			}
		}
		closedir(dp);
	}
	body.append("</pre><hr></body>\n"
				"</html>\n");
	response->setBody(body);
	response->setStatus("200 OK");
}

void RequestHandler::doPut(Request* request, Response* response) { (void)request, (void)response; }

void RequestHandler::doDelete(Request* request, Response* response) { (void)request, (void)response; }

void RequestHandler::setResponseWithError(Response* response, string errorMessage, string pathToErrorPage)
{
	if (!pathToErrorPage.empty()) {
		string extension = pathToErrorPage.substr(pathToErrorPage.find_last_of("/\\") + 1);
		string fileExtension = pathToErrorPage.substr(pathToErrorPage.find_last_of(".") + 1);

		response->setContentType(mimeType(extension));
		string body;
		try {
			body = FileReader::readFile(pathToErrorPage);
		} catch (FileReader::FileNotFoundException& ex) {
			body = "";
		}
		response->setBody(body.empty() ? getErrorPageBody(errorMessage) : body);
	} else {
		string body = getErrorPageBody(errorMessage);
		response->setBody(body);
		response->setContentType(mimeType(".html"));
	}
	response->setStatus(errorMessage);
}

void RequestHandler::fillHeaders(Response* response)
{
	time_t currentTime = time(0);
	char* time = ctime(&currentTime);
	response->setHeader(response->getProtocol() + " " + response->getStatus() + "\r\n");
	response->setHeader("Server: webserv/2.0\r\n");
	response->setHeader("Date: " + string(time, strlen(time) - 1) + "\r\n");
	response->setHeader("Content-Type: " + response->getContentType() + "\r\n");
	response->setHeader("Content-Length: " + std::to_string(response->getBody().size()) + "\r\n");
	response->setHeader("Connection: keep-alive\r\n\r\n");
	response->setBuffer(response->getHeaders() + response->getBody());
}
