#include "RequestHandler.hpp"

#define NOTEXIST 0
#define REGFILE 1
#define ISFOLDER 2

#define CGICODE -1

RequestHandler::RequestHandler()
{
	types["aac"] = "audio/aac";
	types["abw"] = "application/x-abiword";
	types["arc"] = "application/x-freearc";
	types["avi"] = "video/x-msvideo";
	types["azw"] = "application/vnd.amazon.ebook";
	types["bin"] = "application/octet-stream";
	types["bmp"] = "image/bmp";
	types["bz"] = "application/x-bzip";
	types["bz2"] = "application/x-bzip2";
	types["cda"] = "application/x-cdf";
	types["csh"] = "application/x-csh";
	types["css"] = "text/css";
	types["csv"] = "text/csv";
	types["doc"] = "application/msword";
	types["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	types["eot"] = "application/vnd.ms-fontobject";
	types["epub"] = "application/epub+zip";
	types["gz"] = "application/gzip";
	types["gif"] = "image/gif";
	types["htm"] = "text/html";
	types["html"] = "text/html";
	types["ico"] = "image/vnd.microsoft.icon";
	types["ics"] = "text/calendar";
	types["jar"] = "application/java-archive";
	types["jpeg"] = "image/jpeg";
	types["jpg"] = "image/jpeg";
	types["js"] = "text/javascript";
	types["json"] = "application/json";
	types["jsonld"] = "application/ld+json";
	types["mid"] = "audio/midi audio/x-midi";
	types["midi"] = "audio/midi audio/x-midi";
	types["mjs"] = "text/javascript";
	types["mp3"] = "audio/mpeg";
	types["mp4"] = "video/mp4";
	types["mpeg"] = "video/mpeg";
	types["mpkg"] = "application/vnd.apple.installer+xml";
	types["odp"] = "application/vnd.oasis.opendocument.presentation";
	types["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	types["odt"] = "application/vnd.oasis.opendocument.text";
	types["oga"] = "audio/ogg";
	types["ogv"] = "video/ogg";
	types["ogx"] = "application/ogg";
	types["opus"] = "audio/opus";
	types["otf"] = "font/otf";
	types["png"] = "image/png";
	types["pdf"] = "application/pdf";
	types["php"] = "application/x-httpd-php";
	types["ppt"] = "application/vnd.ms-powerpoint";
	types["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	types["rar"] = "application/vnd.rar";
	types["rtf"] = "application/rtf";
	types["sh"] = "application/x-sh";
	types["svg"] = "image/svg+xml";
	types["swf"] = "application/x-shockwave-flash";
	types["tar"] = "application/x-tar";
	types["tif"] = "image/tiff";
	types["tiff"] = "image/tiff";
	types["ts"] = "video/mp2t";
	types["ttf"] = "font/ttf";
	types["txt"] = "text/plain";
	types["vsd"] = "application/vnd.visio";
	types["wav"] = "audio/wav";
	types["weba"] = "audio/webm";
	types["webm"] = "video/webm";
	types["webp"] = "image/webp";
	types["woff"] = "font/woff";
	types["woff2"] = "font/woff2";
	types["xhtml"] = "application/xhtml+xml";
	types["xls"] = "application/vnd.ms-excel";
	types["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	types["xml"] = "application/xml if not readable from casual users";
	types["xul"] = "application/vnd.mozilla.xul+xml";
	types["zip"] = "application/zip";
	types["3gp"] = "video/3gpp audio/3gpp if it doesn't contain video";
	types["3g2"] = "video/3gpp2 audio/3gpp2 if it doesn't contain video";
	types["7z"] = "application/x-7z-compressed";

	//Временные переменные
	locationPath = "resources/html_data";
	autoindex = false;
	index.push_back("index");
	index.push_back("index.html");
}

RequestHandler::~RequestHandler() { }

const std::string& RequestHandler::mimeType(const std::string& uri)
{
	std::string::size_type found = uri.find_last_of('.');
	if (found == std::string::npos)
		return types["txt"];
	std::string extension = uri.substr(found + 1);
	if (types.find(extension) != types.end())
		return types[extension];
	return types["bin"];
}

int checkWhatsThere(std::string const& path, time_t* lastModified)
{
	struct stat file;

	if (stat(path.c_str(), &file) == -1) {
		return NOTEXIST;
	}
	if (S_ISREG(file.st_mode)) {
		if (lastModified)
			*lastModified = file.st_mtime;
		return REGFILE;
	}
	return ISFOLDER;
}

void RequestHandler::readfile(Response* response, const std::string& path)
{
	string exеtention = path.substr(path.find_last_of("/\\") + 1);
	string fileExеtention = path.substr(path.find_last_of(".") + 1);

	response->setContentType(mimeType(exеtention));
	try {
		response->setBody(FileReader::readFile(path));
	} catch (FileReader::FileNotFoundException& ex) {
		response->setBody(FileReader::readFile("resources/errorPages/404.html"));
	}
	response->setStatus("200 OK");
}

bool RequestHandler::isBadRequest(Request* request) const { return request->getMethod() == UNKNOWN_METHOD || request->getUri().empty() || request->getProtocol().empty(); }

void RequestHandler::formResponse(Request* request, Response* response)
{
	if (isBadRequest(request))
		setResponseWithError(response, "400 Bad Request");
	else if (request->getMethod() == POST)
		doPost(request, response);
	else if (request->getMethod() == GET)
		doGet(request, response);
	else if (request->getMethod() == PUT)
		doPut(request, response);
	else if (request->getMethod() == DELETE)
		doDelete(request, response);
	fillHeaders(response);
}

void RequestHandler::doPost(Request* request, Response* response) { (void)request, (void)response; }

void RequestHandler::doGet(Request* request, Response* response)
{
	string pathToFile = locationPath + request->getUri();
	response->setProtocol("HTTP/1.1");

	// Check files extension
	// CGI
	// создать свой фаайл записать в него результат компиляции файла и прописать путь в path to file
	string path = pathToFile;
			CGI cgi = CGI(pathToFile);
		if (cgi.isFileShouldBeHandleByCGI()) {
			cout << cgi.getPathToFileWithResult(pathToFile) << endl;
		}
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

bool RequestHandler::fillBodyFromIndexFile(Response* response, const string& pathToFile)
{
	time_t lastModified;
	//	struct stat file;

	for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++) {
		string indexFile = pathToFile + *it;
		if (checkWhatsThere(indexFile, &lastModified) == REGFILE) {
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
				checkWhatsThere(tmp_path, &lastModified);
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
		  "        <br>\n"
		  "        <img src=\"./errorPages/mem.gif\" height=\"413px\" width=\"504px\">\n"
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
	// cout << response->getBuffer() << endl;
}
