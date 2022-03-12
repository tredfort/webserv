#include "ServerContext.hpp"

ServerContext::~ServerContext() { }

// here we parse file stream, and throw error in bad case
ServerContext::ServerContext(std::ifstream* fileStream)
	: _autoIndex(false)
	, _clientMaxBodySize(1024 * 1024)
{
	string line;
	// TODO:: set default
	// TODO:: listen *:80 | *:8000
	// TODO:: index index.html

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
		int stringIndex = getStringIndexFromArray(key, SERVER_CONTEXT_DIRECTIVES);
		switch (stringIndex) {
		case 0: // server_name https://nginx.org/en/docs/http/ngx_http_core_module.html#server_name
			for (size_t i = 1; i < lineWords.size(); ++i)
				_serverNames.push_back(lineWords[i]); // TODO:: should it be validated?
			break;
		case 1: // listen https://nginx.org/en/docs/http/ngx_http_core_module.html#listen
			if (lineWords.size() > 2)
				fatalError("Failed to parse listen directive!", 23);
			if (isDigits(lineWords[1])) // in this case it is only port provided
			{
				addPortListener(lineWords[1]);
			} else if (lineWords[1].find(":") != string::npos) // directive provide address and port
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
		case -1:
		default:
			fatalError(string("Failed to parse config. unknown directive: ") + key, 13);
			break;
		}
	}
}

const vector<string>& ServerContext::getServerNames() const { return _serverNames; }
void ServerContext::setServerNames(const vector<string>& serverNames) { _serverNames = serverNames; }
bool ServerContext::isAutoIndex() const { return _autoIndex; }
void ServerContext::setAutoIndex(bool autoIndex) { _autoIndex = autoIndex; }

void ServerContext::addPortListener(string value)
{
	int port = stoi(value);
	if (port > 0 && port <= 65535) {
		_listenes.push_back(make_pair<string, int>("*", port));
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
	int port = stoi(value);
	if (port <= 0 || port > 65535) {
		fatalError("Failed to parse address and port from listen directive!", 15);
	}
	_listenes.push_back(make_pair<string, int>(result[1], port));
}

void ServerContext::addAddressListener(string value)
{
	// TODO:: somehow validate it, or keep until bind
	_listenes.push_back(make_pair(value, 80));
}
vector<pair<int, string> >& ServerContext::getErrorPages() { return _errorPages; }

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
	for (vector<pair<int, string> >::iterator it = _errorPages.begin(); it != _errorPages.end(); ++it) {
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
