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

bool RequestHandler::isBadRequest(Request* request) const
{
	return request->getMethod() == UNKNOWN_METHOD || request->getUri().empty() || request->getProtocol().empty();
}

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
	string locationPath = "resources/html_data";
//	bool autoindex = true;
	vector<string> index;
	index.push_back("index");
	index.push_back("index.html");

	toSend = "";

	time_t lastModified;
	std::string path;
	std::string location = "resources/html_data/"; // TODO link with config

	if (request->getUri() == "/") { // Root page
		for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++) {
			if (checkWhatsThere((*it), &lastModified) == REGFILE) {
				path = *it;
				status_code = 200;
				break;
			}
		}
	}
	else if (checkWhatsThere(location + request->getUri().substr(1), &lastModified) == REGFILE) { // Not root
		path = location;
		path += &request->getUri().c_str()[1];
		std::cout << "trying " << location + request->getUri().substr(1) << "\n path = " << path << std::endl;
		status_code = 200;
	}
	else { // Requested path not found
		std::cout << "404 for URL " << request->getUri() << std::endl;
		status_code = 404;
		path = "resources/errorPages/404.html";
	}
	readfile(path);
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

void RequestHandler::doPut(Request* request, Response* response) { (void)request, (void)response; }

void RequestHandler::doDelete(Request* request, Response* response) { (void)request, (void)response; }
