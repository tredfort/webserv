#include "RequestHandler.hpp"

RequestHandler::RequestHandler(Config* config, Env& env)
	: config(config)
	, _env(env)
{
	_cgiFileFormats["py"] = "python3";
	_cgiFileFormats["php"] = "php";
	fillTypes(_types);
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
	string fileExtension = path.substr(path.find_last_of('.') + 1);

	response->setContentType(mimeType(extension));
	try {
		response->setBody(FileReader::readFile(path));
		response->setStatusCode(200);
	} catch (FileReader::FileNotFoundException& ex) {
		response->setBody(getErrorPageBody("ДОБРО ПОЖАЛОВАТЬ НА СТРАНИЦУ 404!"));
		response->setStatusCode(404);
	}
}

bool RequestHandler::isBadRequest(Request* request) const { return request->getMethod().empty() || request->getUri().empty() || request->getProtocol().empty(); }

bool RequestHandler::isProtocolSupported(const string& protocol) const { return protocol == "HTTP/1.1"; }

void RequestHandler::formResponse(WebClient* client)
{
	Response* response = client->getResponse();
	Request* request = client->getRequest();
	LocationContext* location = config->getLocationContext(client->getIp(), client->getPort(), request->getHost(), request->getUri());
	response->setProtocol("HTTP/1.1");
	//	cout << "*location info*" << endl;
	//	location->printConfig();

	if (isBadRequest(request)) {
		response->setStatusCode(400);
	} else if (!isProtocolSupported(request->getProtocol())) {
		response->setStatusCode(505);
	} else if (location->getRedirect().first) {
		response->setStatusCode(location->getRedirect().first);
	} else if ((request->isPostMethod() || request->isDeleteMethod()) && !location->getUploadPath().empty()) {
		// Don't do anything
	} else if (!location->getAllowedMethods().empty() && !location->getAllowedMethods().count(request->getMethod())) {
		response->setStatusCode(405);
	} // TODO: GET метод нельзя запретить и он всегда должен быть в разрешенных методах!
	string path = getPathFromUri(request->getUri(), location);
	if (isFileShouldBeHandleByCGI(path)) {
		CGI cgi(*request, path, _env);
		cout << "process CGI..." << endl;
		CGIModel cgiResult = cgi.getPathToFileWithResult();
		if (cgiResult.isSuccess) {
			readfile(response, cgiResult.pathToFile);
		} else {
			response->setStatusCode(500);
		}
	} else {
		if (request->isGetMethod()) {
			doGet(request, response, location);
		} else if (request->isPostMethod()) {
			doPost(request, response, location);
		} else if (request->isPutMethod()) {
			// TODO:: implement doPut function
			doPost(request, response, location);
			// doPut(request, response, location);
		} else if (request->isDeleteMethod()) {
			doDelete(request, response, location);
		} else {
			response->setStatusCode(501);
		}
	}
	setStatusLine(response);
	if (response->getStatusCode()) {
		setBodyForStatusCode(response, location);
	}
	fillHeaders(response, location);
}

void RequestHandler::setStatusLine(Response* response)
{
	switch (response->getStatusCode()) {
	case 301:
		response->setStatusLine("301 Moved Permanently");
		break;
	case 302:
		response->setStatusLine("302 Found");
		break;
	case 307:
		response->setStatusLine("307 Temporary Redirect");
		break;
	case 308:
		response->setStatusLine("308 Permanent Redirect");
		break;
	case 400:
		response->setStatusLine("400 Bad Request");
		break;
	case 403:
		response->setStatusLine("403 Forbidden");
		break;
	case 404:
		response->setStatusLine("404 Not Found");
		break;
	case 405:
		response->setStatusLine("405 Method Not Allowed");
		break;
	case 500:
		response->setStatusLine("500 Server Error");
		break;
	case 501:
		response->setStatusLine("501 Not Implemented");
		break;
	case 505:
		response->setStatusLine("505 HTTP Version Not Supported");
		break;
	default:
		response->setStatusLine(toString(response->getStatusCode()));
		break;
	}
}

void RequestHandler::setBodyForStatusCode(Response* response, LocationContext* location)
{
	if (response->getStatusCode() >= 300 && response->getStatusCode() < 400) {
		response->setBody(getRedirectPageBody(location->getRedirect()));
		response->setContentType(mimeType(".html"));
	} else if (response->getStatusCode() > 400) {
		string pathToErrorPage = location->getErrorPage(response->getStatusCode());
		if (isFileExists(pathToErrorPage) && !isDirectory(pathToErrorPage)) {
			readfile(response, pathToErrorPage);
		} else {
			response->setBody(getErrorPageBody(response->getStatusLine()));
			response->setContentType(mimeType(".html"));
		}
	} else {
		// TODO:: here should be okay in this section of code looks bad, I think there are some dublicates
	}
}

void RequestHandler::doGet(Request* request, Response* response, LocationContext* location)
{
	const string pathToFile = getPathFromUri(request->getUri(), location);
	if (isDirectory(pathToFile)) {
		if (location->isAutoIndex()) {
			folderContents(response, pathToFile, request->getUri());
		} else {
			response->setStatusCode(403);
		}
	} else {
		readfile(response, pathToFile);
	}
}

// TODO:: handle request without upload directive
void RequestHandler::doPost(Request* request, Response* response, LocationContext* location)
{
	vector<PostVariable*> postVariables = request->getPostVariables();
	bool isAdded = false;
	string uploadPath = location->getUploadPath();
	cout << "post variables count: " << postVariables.size() << endl;

	for (vector<PostVariable*>::iterator it = postVariables.begin(); it != postVariables.end(); ++it) {
		string filename = (*it)->getHeader("filename"); // getFilename
		string filePath;

		if (startsWith(uploadPath, "/")) {
			filePath = "" + uploadPath + (getLastSymbol(uploadPath) == '/' ? "" : "/") + filename;
		} else {
			filePath = "./" + uploadPath + (getLastSymbol(uploadPath) == '/' ? "" : "/") + filename;
		}

		if (!filename.empty()) {
			cout << "creating file -> " << filePath << endl;
			std::ofstream out(filePath);
			out << (*it)->getBody();
			out.close();

			if (out.bad()) // bad() function will check for badbit
			{
				cout << "Writing to file failed" << endl;
			} else {
				isAdded = true;
			}
		}
	}
	if (isAdded) {
		response->setStatusCode(201);
		response->setBody("Some of files was created or rewritten");
	} else {
		response->setStatusCode(303);
		response->setBody("Any of files not created");
		// TODO:: add location header here
		//			response->pushHeader("Location: /\r\n");
	}
}

// TODO:: implement
void RequestHandler::doPut(Request* request, Response* response, LocationContext* location)
{
	(void)request;
	(void)response;
	(void)location;
}

void RequestHandler::folderContents(Response* response, const std::string& path, const string& uri)
{
	DIR* dp;
	struct dirent* di_struct;
	struct stat file_stats;
	time_t lastModified;
	string body;

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
	response->setStatusLine("200 OK");
}

void RequestHandler::doDelete(Request* request, Response* response, LocationContext* location)
{
	string pathToFile = location->getUploadPath() + getStringAfterTarget(request->getUri(), location->getUploadPath());

	if (access(pathToFile.c_str(), W_OK) != 0) {
		response->setStatusCode(403);
	} else if (std::remove(pathToFile.c_str()) != 0) {
		response->setStatusCode(500);
	} else {
		response->setStatusCode(200);
		string body = "<html>\n"
					  "  <body>\n"
					  "    <h1>File deleted.</h1>\n"
					  "  </body>\n"
					  "</html>";
		response->setBody(body);
	}
}

void RequestHandler::fillHeaders(Response* response, LocationContext* location)
{
	time_t currentTime = time(0);
	char* time = ctime(&currentTime);
	pair<int, string> redirect = location->getRedirect();
	response->pushHeader(response->getProtocol() + " " + response->getStatusLine() + "\r\n");
	response->pushHeader("Server: webserv/2.0\r\n");
	response->pushHeader("Date: " + string(time, strlen(time) - 1) + "\r\n");
	if (!response->getContentType().empty()) {
		response->pushHeader("Content-Type: " + response->getContentType() + "\r\n");
	}
	response->pushHeader("Content-Length: " + std::to_string(response->getBody().size()) + "\r\n");
	if (redirect.first != 0) {
		response->pushHeader("Location: " + redirect.second + "\r\n");
	}
	// TODO:: do we have to use it?
	response->pushHeader("Connection: keep-alive\r\n\r\n");
	response->setBuffer(response->getHeaders() + response->getBody());
}

string RequestHandler::getPathFromUri(const string& uri, LocationContext* location) const
{
	string path = location->getRoot() + "/" + uri;
	if (isFileExists(path)) {
		if (isDirectory(path)) {
			if (path.back() != '/') {
				path.append("/");
			}
			for (vector<string>::const_iterator it = location->getIndex().begin(), ite = location->getIndex().end(); it != ite; ++it) {
				string pathToIndexFile;
				if ((*it).front() == '/') {
					pathToIndexFile = location->getRoot() + *it;
				} else {
					pathToIndexFile = path + *it;
				}
				if (isFileExists(pathToIndexFile) && !isDirectory(pathToIndexFile) && !access(pathToIndexFile.c_str(), W_OK)) {
					return pathToIndexFile;
				}
			}
		}
		return path;
	}
	return string();
}

bool RequestHandler::isFileShouldBeHandleByCGI(string& pathToFile) const { return _cgiFileFormats.find(getFileFormat(pathToFile)) != _cgiFileFormats.end(); }
