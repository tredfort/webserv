#include "LocationContext.hpp"

LocationContext::LocationContext(const vector<string>& lineLocation, std::ifstream* fileStream)
{
	string line;

	if (lineLocation.size() == 4) {
		_modificator = lineLocation[1];
		_location = lineLocation[2];
	} else if (lineLocation.size() == 3) {
		_location = lineLocation[1];
	}
	// set default values
	_redirect.first = 0;
	_redirect.second = "";

	while (getline(*fileStream, line)) {
		line = trim(line);
		if (line.empty() || line[0] == '#') {
			continue;
		}
		if (line == "}") {
			// TODO:: check if is fulfilled and done
			return;
		}
		vector<string> lineWords = ft_split(line, " ;");
		// TODO:: im' not sure
		if (lineWords.size() < 2) {
			fatalError("Key doesn't have value! Server context", 11);
		}
		string key = lineWords[0];
		int stringIndex = getStringIndexFromArray(key, LOCATION_CONTEXT_DIRECTIVES);
		switch (stringIndex) {
		case 0: // allowedMethod not exists in nginx, just read values like GET POST e.t.c. The most similar from nginx
				// https://nginx.org/en/docs/http/ngx_http_core_module.html#limit_except
			parseAllowedMethods(lineWords);
			break;
		case 1: // autoindex
			if (lineWords.size() != 2 || (lineWords[1] != "on" && lineWords[1] != "off"))
				fatalError("Failed to parse autoindex directive!", 23);
			setAutoIndex(lineWords[1] == "on");
			break;
		case 2: // cgiExtension
			if (lineWords.size() != 2)
				fatalError("Failed to parse cgiExtension directive!", 24);
			setCgiExtension(lineWords[1]);
			break;
		case 3: // cgiPath
			if (lineWords.size() != 2)
				fatalError("Failed to parse cgiExtension directive!", 24);
			setCgiPath(lineWords[1]); // TODO:: check if it possible to reach or don't check it? What if it is remote cgi?
			break;
		case 4: // client_max_body_size
			if (lineWords.size() > 2 || lineWords[1].find_first_not_of("0123456789m") != std::string::npos)
				fatalError("failed to parse client_max_body_size directive", 24);
			Config::parseClientMaxBodySize(lineWords[1], &_clientMaxBodySize);
			break;
		case 5: // error_page
			Config::addErrorPage(lineWords, _errorPages);
			break;
		case 6: // index
			Config::parseIndex(lineWords, _index);
			break;
		case 7: // redirect the most similar in nginx return https://nginx.org/en/docs/http/ngx_http_rewrite_module.html#return
			if (_redirect.first != 0)
				fatalError("Failed to parse redirect directive! Already parsed!", 25);
			if (lineWords.size() == 2) {
				_redirect.first = 302;
				_redirect.second = lineWords[1];
			} else if (lineWords.size() == 3) {
				try {
					_redirect.first = std::stoi(lineWords[1]);
				} catch (const std::invalid_argument& ia) {
					fatalError("Failed to parse redirect directive!", 26);
				} catch (const std::out_of_range& oor) {
					fatalError("Failed to parse redirect directive!", 27);
				}
				_redirect.second = lineWords[2];
			} else {
				fatalError("Failed to parse redirect directive! ");
			}
			break;
		case 8: // root
			if (!isFileExists(lineWords[1]))
				fatalError("Failed to find file specified in root directive!", 28);
			_root = lineWords[1];
			break;
		case 9: // upload_path doesn't exist in nginx.
			// From subject :Make the route able to accept uploaded files and configure where they should be saved.
			if (!isFileExists(lineWords[1]))
				fatalError("Failed to find file specified in root directive!", 28);
			_uploadPath = lineWords[1];
			break;
		case -1:
		default:
			fatalError(string("Failed to parse config. unknown directive: ") + key, 13);
			break;
		}
	}
}

LocationContext::~LocationContext() { }

void LocationContext::parseAllowedMethods(vector<string> lineWords)
{
	if (!_allowedMethods.empty())
		fatalError("Failed to fill allowedMethods, already set!", 39);
	if (std::find(lineWords.begin(), lineWords.end(), "GET") != lineWords.end())
		_allowedMethods.push_back("GET");
	if (std::find(lineWords.begin(), lineWords.end(), "POST") != lineWords.end())
		_allowedMethods.push_back("POST");
	if (std::find(lineWords.begin(), lineWords.end(), "PUT") != lineWords.end())
		_allowedMethods.push_back("PUT");
	if (std::find(lineWords.begin(), lineWords.end(), "DELETE") != lineWords.end())
		_allowedMethods.push_back("DELETE");
}
bool LocationContext::isAutoIndex() const { return _autoIndex; }
void LocationContext::setAutoIndex(bool autoIndex) { _autoIndex = autoIndex; }
const string& LocationContext::getCgiExtension() const { return _cgiExtension; }
void LocationContext::setCgiExtension(const string& cgiExtension) { _cgiExtension = cgiExtension; }
const string& LocationContext::getCgiPath() const { return _cgiPath; }
void LocationContext::setCgiPath(const string& cgiPath) { _cgiPath = cgiPath; }

void LocationContext::printConfig()
{
	cout << "LOCATION CONFIG" << endl << endl;
	cout << "location: " << _location << endl;
	cout << "location modificator: " << _modificator << endl;
	cout << "AutoIndex is enabled? :" << (_autoIndex ? "true" : "false") << endl;
	cout << "CgiExtension :" << _cgiExtension << endl;
	cout << "CgiPath :" << _cgiPath << endl;
	cout << "client_max_body_size :" << _clientMaxBodySize << endl;
	cout << "index:" << endl;
	cout << "redirect: " << _redirect.first << " " << _redirect.second << endl;
	cout << "root: " << _root << endl;
	cout << "upload path: " << _uploadPath << endl;

	cout << "Allowed methods:" << endl;
	printStringVector(_allowedMethods);

	cout << endl << "Error pages:" << endl;
	for (vector<pair<int, string> >::iterator it = _errorPages.begin(); it != _errorPages.end(); ++it) {
		cout << it->first << " " << it->second << endl;
	}
	cout << endl;

	cout << "Index: " << endl;
	printStringVector(_index);
}
