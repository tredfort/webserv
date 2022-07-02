#include "RequestHandler.hpp"

#define NOTEXIST 0
#define REGFILE 1
#define ISFOLDER 2

#define CGICODE -1

RequestHandler::RequestHandler(Config* config, Env &env)
	: config(config),
	_env(env)
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
	} catch (FileReader::FileNotFoundException& ex) {
		response->setBody(FileReader::readFile("resources/errorPages/404.html"));
	}
	response->setStatus("200 OK");
}

bool RequestHandler::isBadRequest(Request* request) const
{
	return request->getMethod().empty() || request->getUri().empty() || request->getProtocol().empty();
}

bool RequestHandler::isProtocolSupported(const string& protocol) const
{
	return protocol == "HTTP/1.1";
}

void RequestHandler::formResponse(WebClient* client)
{
	Response* response = client->getResponse();
	Request* request = client->getRequest();
//	LocationContext location = config->getLocationContext(client->getIp(), client->getPort(), request->getHeader("Host"), request->getUri()); // TODO:: get location
	response->setProtocol("HTTP/1.1");

	if (isBadRequest(request))
		setResponseWithError(response, "400 Bad Request");
	else if (!isProtocolSupported(request->getProtocol()))
		setResponseWithError(response, "505 HTTP Version Not Supported");
	else if (!_location.getAllowedMethods().count(request->getMethod()))
		setResponseWithError(response, "405 Method Not Allowed");
	else if (request->getMethod() == "GET")
		doGet(_location, request, response);
	else if (request->getMethod() == "POST")
		doPost(request, response);
	else if (request->getMethod() == "DELETE")
		doDelete(request, response);
	else if (request->getMethod() == "PUT")
		doPut(request, response);
	else
		setResponseWithError(response, "501 Not Implemented");
	fillHeaders(response);
}

void RequestHandler::doPost(Request* request, Response* response) { (void)request, (void)response; }

void RequestHandler::doGet(const LocationContext& location, Request* request, Response* response)
{
	string pathToFile = location.getRoot() + request->getUri();

	// Check files extension
	// CGI
	// создать свой фаайл записать в него результат компиляции файла и прописать путь в path to file
	string path = pathToFile;
	CGI cgi(*request, *config, _env);
	if (cgi.isFileShouldBeHandleByCGI(path)) {
		cout << "CGIIIIII" << endl;
		CGIModel cgiResult = cgi.getPathToFileWithResult(path);
		if (cgiResult.isSuccess) {
			readfile(response, cgiResult.pathToFile);
		} else {
			setResponseWithError(response, "500 Server Error");
		}
	} else {
		if (!isFileExists(pathToFile)) {
			//		readfile(response, "resources/html_data/errorPages/index.html");
			setResponseWithError(response, "404 Not Found");
		} else if (isDirectory(pathToFile)) {
			if (pathToFile.back() != '/') {
				pathToFile.append("/");
			}
			if (!fillBodyFromIndexFile(response, pathToFile) && !autoindex) {
				setResponseWithError(response, "403 Forbidden");
			} else if (autoindex) {
				folderContents(response, pathToFile, request->getUri());
			}
			response->setContentType(mimeType(".html"));
		} else {
			readfile(response, pathToFile);
		}
	}
}

bool RequestHandler::fillBodyFromIndexFile(Response* response, const string& pathToFile)
{
	//	struct stat file;

	for (int i = 0, size = _location.getIndex().size(); i < size; ++i) {
		string indexFile = pathToFile + _location.getIndex()[i];
		if (isFileExists(indexFile) && !isDirectory(indexFile)) {
			//			if (stat(indexFile.c_str(), &file) == -1 || file.st_mode & S_IRGRP) {
			//				cout << "Нет прав на чтение" << endl;
			//				return false;
			//			}
			readfile(response, indexFile);
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

void RequestHandler::setResponseWithError(Response* response, string errorMessage)
{
	string body = "<html>\n"
				  "<head>\n"
				  "    <title>Error "
		+ errorMessage
		+ "</title>\n"
		  "    <link href=\"https://fonts.googleapis.com/css2?family=Lato:wght@300&display=swap\" rel=\"stylesheet\">\n"
		  "    <link rel=\"stylesheet\" href=\"./errorPages/style.css\">\n"
		  "</head>\n"
		  "<body>\n"
		  "<div id=\"main\">\n"
		  "    <div class=\"msg\">\n"
		  "        <h1>"
		+ errorMessage
		+ "</h1>\n"
		  "    </div>\n"
		  "</div>\n"
		  "</body>\n"
		  "</html>\n";

	response->setBody(body);
	response->setContentType(mimeType(".html"));
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
