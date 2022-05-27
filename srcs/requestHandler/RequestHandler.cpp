#include "RequestHandler.hpp"

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

void RequestHandler::doPost(Request* request, Response* response) {
	(void)request, (void)response;
//	std::string upload_dir = _server->GetRoot() + _response_location->GetUploadPath();
//	if (!isDirectory(upload_dir))
//		throw "500 no dir for upload in POST";
//
//	std::string boundary;
//	std::string boundary_end;
//	std::map<std::string, std::string>::iterator it_content = _requst_header.find("Content-Type");
//	if (it_content != _requst_header.end() &&
//		it_content->second.find("multipart/form-data") != std::string::npos)
//	{
//		if (it_content->second.find("boundary") == std::string::npos)
//			throw "400 REQUEST ERROR! multipart/form-data have no boundery";
//		std::string name_boundary = "boundary=";
//		int name_boundary_len = name_boundary.length();
//		int position_bounery_start = it_content->second.find(name_boundary) + name_boundary_len;
//		int position_bounery_end = it_content->second.find("\r\n", position_bounery_start);
//		boundary = "--" + it_content->second.substr(position_bounery_start, position_bounery_end - position_bounery_start);
//		boundary_end = boundary + "--";
//	}
//	else
//		throw "500 REQUEST ERROR! wrong form for upload";
//
//	std::string file_name;
//	std::string path_file;
//	if (_request_body.find("filename=", 0) == std::string::npos)
//		throw "400 REQUEST ERROR! no File name in request";
//	else
//	{
//		std::string tmp_name = "filename=\"";
//		int tmp_name_len = tmp_name.length();
//		int position_filename_start = _request_body.find(tmp_name) + tmp_name_len;
//		int position_filename_end = _request_body.find("\"", position_filename_start);
//		file_name = _request_body.substr(position_filename_start, position_filename_end - position_filename_start);
//		path_file = upload_dir + "/" + file_name;
//	}
//
//	_request_body = _request_body.substr(_request_body.find("\r\n\r\n") + 4);
//	_request_body = _request_body.substr(0, _request_body.find(boundary));
//
//	std::ofstream tmp_file(path_file);
//	if (!tmp_file.is_open())
//		throw "500 cannot creat file in POST";
//	tmp_file << _request_body;
//	tmp_file.close();
//	_CreateResponseBodyDirectory(upload_dir);
//	_status_code = "200";
}

void RequestHandler::doGet(Request* request, Response* response)
{
	string pathToFile = locationPath + request->getUri();
	response->setProtocol("HTTP/1.1");

	if (!isFileExists(pathToFile)) {
		setResponseWithError(response, "404 Not Found");
	} else if (isDirectory(pathToFile)) {
		if (pathToFile.back() != '/') {
			pathToFile.append("/");
		}
		if (!fillBodyFromIndexFile(response, pathToFile) && !autoindex) {
			setResponseWithError(response, "403 Forbidden");
		}
		if (response->getBody().empty() && autoindex) {
			folderContents(response, pathToFile, request->getUri());
		}
		response->setContentType(mimeType(".html"));
	} else {
		readfile(response, pathToFile);
	}
}

bool RequestHandler::fillBodyFromIndexFile(Response* response, const string& pathToFile)
{
	//	struct stat file;

	for (int i = 0, size = index.size(); i < size; ++i) {
		string indexFile = pathToFile + index[i];
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
}
