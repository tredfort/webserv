#include "ServerContext.hpp"

ServerContext::~ServerContext() { }

// here we parse file stream, and throw error in bad case
ServerContext::ServerContext(std::ifstream* fileStream)
	: _autoIndex(false)
	, _clientMaxBodySize(1024 * 1024)
{
	string line;
	ssize_t directiveIndex;
	vector<string> lineWords;

	while ((directiveIndex = Config::getParsedLine(fileStream, false, &lineWords, SERVER_CONTEXT_DIRECTIVES)) != -1) {
		switch (directiveIndex) {
		case 0: // server_name https://nginx.org/en/docs/http/ngx_http_core_module.html#server_name
			for (size_t i = 1; i < lineWords.size(); ++i)
				_serverNames.push_back(lineWords[i]);
			break;
		case 1: // listen https://nginx.org/en/docs/http/ngx_http_core_module.html#listen
			if (lineWords.size() > 2)
				fatalError("Failed to parse listen directive!", 23);
			if (isOnlyDigits(lineWords[1])) // in this case it is only port provided
			{
				addPortListener(lineWords[1]);
			} else if (lineWords[1].find(':') != string::npos) // directive provide address and port
			{
				addAddressPortListener(lineWords[1]);
			} else { // else only address
				addAddressListener(lineWords[1]);
			}
			break;
		case 2: // autoindex https://nginx.org/en/docs/http/ngx_http_autoindex_module.html
			if (lineWords.size() != 2 || (lineWords[1] != "on" && lineWords[1] != "off"))
				fatalError("Failed to parse autoindex value!", 23);
			setAutoIndex(lineWords[1] == "on");
			break;
		case 3: // client_max_body_size https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
			// there is no any info about possible value...
			if (lineWords.size() > 2 || lineWords[1].find_first_not_of("0123456789m") != std::string::npos)
				fatalError("failed to parse client_max_body_size directive", 24);
			Config::parseClientMaxBodySize(lineWords[1], &_clientMaxBodySize);
			break;
		case 4: // error_page https://nginx.org/en/docs/http/ngx_http_core_module.html#error_page
			Config::addErrorPage(lineWords, getErrorPages());
			break;
		case 5: // index https://nginx.org/en/docs/http/ngx_http_index_module.html#index
			Config::parseIndex(lineWords, getIndex());
			break;
		case 6: // location
			if (lineWords.size() < 3 || lineWords.size() > 4 || lineWords[lineWords.size() - 1] != "{")
				fatalError("Failed to parse location directive!", 25);
			_locations.push_back(new LocationContext(lineWords, fileStream));
			break;
		case 7: // root
			Config::parseRoot(lineWords, _root);
			break;
		case -1:
		default:
			fatalError("Unexpected value in switch!", 29);
			break;
		}
	}
	checkDefaultValues();
}

void ServerContext::checkDefaultValues()
{
	if (_serverNames.empty()) {
		_serverNames.push_back("");
	}
	if (_listenes.empty()) {
		// TODO:: if it is root set -> *:8000
		_listenes.push_back(make_pair("*", 80));
	}
	if (_index.empty()) {
		_index.push_back("index.html");
	}
	// TODO:: what if there are no location contexts
}

const vector<string>& ServerContext::getServerNames() const { return _serverNames; }
const vector<pair<string, int> >& ServerContext::getListeners() const { return _listenes; }
void ServerContext::setServerNames(const vector<string>& serverNames) { _serverNames = serverNames; }
bool ServerContext::isAutoIndex() const { return _autoIndex; }
void ServerContext::setAutoIndex(bool autoIndex) { _autoIndex = autoIndex; }

void ServerContext::addPortListener(string value)
{
	int port = stoi(value);
	if (port > 0 && port <= 65535) {
		_listenes.push_back(make_pair("*", port));
	} else {
		fatalError("Failed to parse port from listen directive!", 14);
	}
}

void ServerContext::addAddressPortListener(string value)
{
	vector<string> result = ft_split(value, ":");
	if (result.size() != 2) {
		fatalError("Failed to parse address and port from listen directive!", 15);
	}
	int port = stoi(result[1]);
	if (port <= 0 || port > 65535) {
		fatalError("Failed to parse address and port from listen directive!", 15);
	}
	_listenes.push_back(make_pair(result[0], port));
}

void ServerContext::addAddressListener(string value)
{
	// TODO:: somehow validate it, or keep until bind
	_listenes.push_back(make_pair(value, 80));
}
map<int, string>& ServerContext::getErrorPages() { return _errorPages; }

string ServerContext::getErrorPage(int code)
{
	for (map<int, string>::iterator it = _errorPages.begin(), ite = _errorPages.end(); it != ite; ++it) {
		if (code == it->first) {
			return it->second;
		}
	}
	return "";
}

vector<string>& ServerContext::getIndex() { return _index; }

void ServerContext::printConfig()
{
	cout << "SERVER CONFIG" << endl << endl;
	cout << "AutoIndex is enabled? :" << (_autoIndex ? "true" : "false") << endl;
	cout << "client_max_body_size :" << _clientMaxBodySize << endl;

	cout << "Index: " << endl;
	printStringVector(_index);

	cout << "server_name: " << endl;
	printStringVector(_serverNames);

	cout << endl << "listen:" << endl;
	for (vector<pair<string, int> >::iterator it = _listenes.begin(); it != _listenes.end(); ++it) {
		cout << it->first << " " << it->second << endl;
	}
	cout << endl;

	cout << endl << "Error pages:" << endl;
	for (map<int, string>::iterator it = _errorPages.begin(); it != _errorPages.end(); ++it) {
		cout << it->first << " " << it->second << endl;
	}
	cout << endl;

	cout << "locations:" << endl;
	for (vector<LocationContext*>::iterator it = _locations.begin(); it != _locations.end(); ++it) {
		cout << endl;
		(*it)->printConfig();
	}
	cout << endl;
}
vector<string> ServerContext::getServerNames() { return _serverNames; }

vector<LocationContext*> ServerContext::getLocationContexts() { return _locations; }

vector<LocationContext*> ServerContext::getLocationContexts(const string& pattern)
{
	vector<LocationContext*> result;

	for (vector<LocationContext*>::iterator it = _locations.begin(), ite = _locations.end(); it != ite; ++it) {
		if (startsWith(pattern, (*it)->getLocation()))
			result.push_back(*it);
	}
	return result;
}

const string& ServerContext::getRoot() { return _root; }

void ServerContext::setRoot(string root) { _root = root; }

void ServerContext::setDefaultDirectives()
{
	for (vector<LocationContext*>::iterator it = _locations.begin(), ite = _locations.end(); it != ite; ++it) {
		if ((*it)->getRoot().empty())
			(*it)->setRoot(_root);

		(*it)->setErrorPagesFromServerContext(_errorPages);
	}
}

void ServerContext::setErrorPagesFromConfigContext(map<int, string>& configErrorPages)
{
	for (map<int, string>::iterator it = configErrorPages.begin(), ite = configErrorPages.end(); it != ite; ++it) {
		const string configErrorPage = it->second;
		if (this->getErrorPage(it->first).empty()) {
			this->_errorPages[it->first] = it->second;
		}
	}
}
