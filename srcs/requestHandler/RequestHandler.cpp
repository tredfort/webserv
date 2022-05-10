#include "RequestHandler.hpp"

#define BUFFERSIZE 256
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

void RequestHandler::readfile(const std::string& path)
{
	string exеtention = path.substr(path.find_last_of("/\\") + 1);
	string fileExеtention = path.substr(path.find_last_of(".") + 1);

	_header_fields["Content-Type"] = mimeType(exеtention);
	try {
		body = FileReader::readFile(path);
	} catch (FileReader::FileNotFoundException& ex) {
		body = FileReader::readFile("resources/errorPages/404.html");
	}
	if (!body.empty()) {
		content_lengh = body.size();
		_header_fields["Content-Length"] = std::to_string(content_lengh);
	}
	_header_fields["Status"] = "200 OK";
}

bool RequestHandler::isBadRequest(Request* request) const { return request->getMethod() == UNKNOWN_METHOD || request->getUri().empty() || request->getProtocol().empty(); }

void RequestHandler::formResponse(Request* request, Response* response)
{
	if (isBadRequest(request)) {
		// TODO: Bad Request
	} else if (request->getMethod() == POST)
		doPost(request, response);
	else if (request->getMethod() == GET)
		doGet(request, response);
	else if (request->getMethod() == PUT)
		doPut(request, response);
	else if (request->getMethod() == DELETE)
		doDelete(request, response);
}

void RequestHandler::doPost(Request* request, Response* response) { (void)request, (void)response; }

void RequestHandler::doGet(Request* request, Response* response)
{
	string pathToFile = locationPath + request->getUri();
	toSend = "";

	std::string path;
	std::string location = "resources/html_data/"; // TODO link with config

	if (!isFileExists(pathToFile)) {
		formResponseBody(404, "Not Found");
//		status_code = 404;
//		readfile("resources/errorPages/404.html");
	} else if (isDirectory(pathToFile)) {
		if (pathToFile.back() != '/')
			pathToFile += '/';
		if (!fillBodyFromIndexFile(pathToFile)) {
			status_code = 403;
			readfile("resources/errorPages/404.html");
		}
		if (autoindex) {

		} else {
			folderContents(pathToFile, request->getUri());
			_header_fields["Content-Type"] = mimeType(".html");
			_header_fields["Content-Length"] = std::to_string(body.size());
			_header_fields["Status"] = "200 OK";
		}
	} else {
		path = pathToFile;
		std::cout << "path = " << path << std::endl;
		status_code = 200;
	}
//	readfile(path);
	toSend.append("HTTP/1.1 ");
	toSend.append(std::to_string(status_code));
	if (status_code != CGICODE) {
		toSend.append("\r\n");
		for (std::map<std::string, std::string>::iterator it = _header_fields.begin(); it != _header_fields.end(); it++) {
			toSend.append((*it).first);
			toSend.append(": ");
			toSend.append((*it).second);
			toSend.append("\r\n");
		}
	}
	if (body.size()) {
		toSend.append("\r\n");
		toSend.append(body);
		toSend.append("\r\n");
	}
	response->toSend = toSend;
}

bool RequestHandler::fillBodyFromIndexFile(const string& pathToFile)
{
	time_t lastModified;
	struct stat file;

	for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++) {
		string indexFile = pathToFile + *it;
		if (checkWhatsThere(indexFile, &lastModified) == REGFILE) {
			if (stat(indexFile.c_str(), &file) == -1 || file.st_mode & S_IRGRP) {
				cout << "Нет прав на чтение" << endl;
				return false;
			}
			readfile(indexFile);
			status_code = 200;
			return true;
		}
	}
	return false;
}

void RequestHandler::folderContents(const std::string& path, const string& uri)
{
	DIR* dp;
	struct dirent* di_struct;
	struct stat file_stats;
	time_t lastModified;

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
				string date = string(std::ctime(&lastModified));
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
}

void RequestHandler::doPut(Request* request, Response* response) { (void)request, (void)response; }

void RequestHandler::doDelete(Request* request, Response* response) { (void)request, (void)response; }

void RequestHandler::formResponseBody(int errorCode, string errorMessage)
{
	string _body = "<html>\n"
				"<head><title>Error "
		+ std::to_string(errorCode) + ". (" + errorMessage + ")"
		+ "</title><link rel=\"stylesheet\" href=\"../errorPages/style.css\">"
		+ "</head>\n<_body>\n"
		+ "<div id=\"main\">\n"
			"    \t<div class=\"fof\">\n"
			"        \t\t<h1>Error " + std::to_string(errorCode) + ". " + errorMessage + "</h1>\n"
			"    \t</div>\n</div><h1>\n</_body>\n</html>\n</_body>\n</html>\n";

	body = _body;
	_header_fields["Content-Type"] = mimeType(".html");
	_header_fields["Content-Length"] = std::to_string(body.size());
	_header_fields["Status"] = "200 OK";
//	response->setBody(_body);
//	toSend = "HTTP/1.1 " + std::to_string(errorCode) + "\r\n";
//	toSend += "Content-Type: " + mimeType(".html") + "\r\n";
//	toSend += "Content-Length: " + std::to_string(_body.size()) + "\r\n\r\n";
//	toSend += _body;
//	toSend += "\r\n";
//	response->setHeader("HTTP/1.1 " + std::to_string(errorCode) + "\r\n");
//	response->setHeader("Content-Type: " + mimeType(".html") + "\r\n");
//	response->setHeader("Content-Length: " + std::to_string(_body.size()) + "\r\n\r\n");
}
