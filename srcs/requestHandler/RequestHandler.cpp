#include "RequestHandler.hpp"

RequestHandler::RequestHandler(Config* config)
	: config(config)
{
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
	response->setContentType(mimeType(path));
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
	//	cout << "*location info*" << endl;
	//	location->printConfig();
	// TODO:: rename path
	string pathToFile = getPathFromUri(location, request->getUri());

	if (isBadRequest(request)) {
		response->setStatusCode(400);
	} else if (!isProtocolSupported(request->getProtocol())) {
		response->setStatusCode(505);
	} else if (location->getRedirect().first) {
		response->setStatusCode(location->getRedirect().first);
	} else if (!location->getAllowedMethods().count(request->getMethod())) {
		response->setStatusCode(405);
	} else if (request->getContentLength() > location->getClientMaxBodySize()) {
		response->setStatusCode(413);
	} else if (isShouldBeHandleByCGI(location, pathToFile)) {
		doCGI(location, request, response, pathToFile);
	} else if (request->getMethod() == "GET") {
		doGet(location, request, response, pathToFile);
	} else if (request->getMethod() == "POST" || request->getMethod() == "PUT") {
		doPost(location, request, response, pathToFile);
	} else if (request->getMethod() == "DELETE") {
		doDelete(response, pathToFile);
	} else {
		response->setStatusCode(501);
	}
	setStatusLine(response);
	if (response->getStatusCode() < 200 || response->getStatusCode() >= 400) {
		setBodyForStatusCode(response, location);
	}
	fillHeaders(response, location);
}

void RequestHandler::setStatusLine(Response* response)
{
	switch (response->getStatusCode()) {
	case 200:
		response->setStatusLine("200 OK");
		break;
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
	case 413:
		response->setStatusLine("413 Payload Too Large");
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
	} else {
		string pathToErrorPage = location->getPathToFile(location->getErrorPage(response->getStatusCode()));
		if (isFileExists(pathToErrorPage) && !isDirectory(pathToErrorPage)) {
			readfile(response, pathToErrorPage);
		} else {
			response->setBody(getErrorPageBody(response->getStatusLine()));
			response->setContentType(mimeType(".html"));
		}
	}
}

void RequestHandler::doPost(LocationContext* location, Request* request, Response* response, string& pathToFile)
{
	string fullPathToFile;
	if (!location->getUploadPath().empty()) {
		fullPathToFile = replace(pathToFile, location->getRoot(), location->getUploadPath());
	} else {
		fullPathToFile = pathToFile;
	}
	if (!createFile(fullPathToFile, request->getBody())) {
		response->setStatusCode(500);
	} else {
		readfile(response, pathToFile);
	}
}

void RequestHandler::doGet(LocationContext* location, Request* request, Response* response, string& pathToFile)
{
	if (!isFileExists(pathToFile)) {
		response->setStatusCode(404);
	} else if (!isDirectory(pathToFile)) {
		readfile(response, pathToFile);
	} else {
		const vector<string> indexes = location->getIndexes();
		for (vector<string>::const_iterator it = indexes.begin(); it != indexes.end(); ++it) {
			string pathToIndexFile = ((*it).front() == '/') ? createPath(location->getRoot(), *it) : createPath(pathToFile, *it);
			if (isFileExists(pathToIndexFile) && !isDirectory(pathToIndexFile)) {
				if (isShouldBeHandleByCGI(location, pathToIndexFile)) {
					doCGI(location, request, response, pathToIndexFile);
					return;
				}
				readfile(response, pathToIndexFile);
				return;
			}
		}
		if (location->isAutoIndex()) {
			folderContents(response, pathToFile, request->getUri());
		} else {
			response->setStatusCode(404);
		}
	}
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
				"<head>"
				"<title>"
		+ title
		+ "</title>"
		  "    <style>\n"
		  "        td {\n"
		  "            padding-left: 15px;\n"
		  "            text-align: left;\n"
		  "        }\n"
		  "    </style>"
		  "</head>\n"
		  "<body>\n"
		  "<h1>"
		+ title
		+ "</h1>"
		  "<table style=\"width:80%\">"
		  "<tr><td><a href=\"../\">../</a></td></tr>\n");
	if ((dp = opendir(path.c_str())) != nullptr) {
		while ((di_struct = readdir(dp)) != nullptr) {
			if (strcmp(di_struct->d_name, ".") && strcmp(di_struct->d_name, "..")) {

				string tmp_path = createPath(path, di_struct->d_name);
				stat(tmp_path.data(), &file_stats);

				body.append("<tr><td><a href=\"" + createPath(uri, di_struct->d_name));
				if (S_ISDIR(file_stats.st_mode))
					body.append("/");
				body.append("\">" + string(di_struct->d_name));
				if (S_ISDIR(file_stats.st_mode))
					body.append("/");
				body.append("</a></td><td>");
				lastModified = getFileModificationDate(tmp_path);
				string date = string(ctime(&lastModified));
				date = date.substr(0, date.size() - 1);
				body.append(date + "</td><td>");
				if (S_ISDIR(file_stats.st_mode))
					body.append("-\n");
				else {
					body.append(std::to_string(static_cast<float>(file_stats.st_size)) + "\n");
				}
				body.append("</td></tr>");
			}
		}
		closedir(dp);
	}
	body.append("</table>\n</body>\n</html>\n");
	response->setBody(body);
	response->setStatusCode(200);
}

void RequestHandler::doDelete(Response* response, string& pathToFile)
{
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

void RequestHandler::doCGI(LocationContext* location, Request* request, Response* response, string& pathToFile)
{
	CGIHandler cgiHandler;
	int statusCode = cgiHandler.handle(location->getCgiPath(), pathToFile, request);
	string body = cgiHandler.getBody();
	response->setStatusCode(statusCode);
	response->setBody(body);
	response->setContentType(mimeType(".html"));
}

void RequestHandler::fillHeaders(Response* response, LocationContext* location)
{
	time_t currentTime = time(0);
	char* time = ctime(&currentTime);
	pair<int, string> redirect = location->getRedirect();
	response->pushHeader(response->getProtocol() + " " + response->getStatusLine() + "\r\n");
	response->pushHeader("Server: " + defaults::SERVER_NAME + "\r\n");
	response->pushHeader("Date: " + string(time, strlen(time) - 1) + "\r\n");
	if (!response->getContentType().empty()) {
		response->pushHeader("Content-Type: " + response->getContentType() + "\r\n");
	}
	response->pushHeader("Content-Length: " + std::to_string(response->getBody().size()) + "\r\n");
	if (redirect.first != 0) {
		response->pushHeader("Location: " + redirect.second + "\r\n");
	}
	if (response->getStatusCode() >= 200 || response->getStatusCode() < 400) {
		response->pushHeader("Connection: keep-alive\r\n");
	}
	response->setBuffer(response->getHeaders() + "\r\n" + response->getBody());
}

string RequestHandler::getPathFromUri(LocationContext* location, string uri) const
{
	uri.replace(uri.find(location->getLocation()), location->getLocation().size(), "");
	return createPath(location->getRoot(), uri);
}

bool RequestHandler::isShouldBeHandleByCGI(LocationContext* location, string& pathToFile) const
{
	return !location->getCgiPath().empty() && !location->getCgiExtension().empty() && location->getCgiExtension() == getFileFormat(pathToFile);
}
