#include "Config.hpp"

/**
 * Fill the vector of string "lineWords" with parsed directive words
 * @param fileStream
 * @param isMainContext
 * @param lineWords
 * @param CONTEXT_DIRECTIVE
 * @return -1 in case if there is no more input, or
 */
ssize_t Config::getParsedLine(ifstream* fileStream, bool isMainContext, vector<string>* lineWords, const string* CONTEXT_DIRECTIVE)
{
	string line;

	if (getline(*fileStream, line)) {
		line = removeAfter(rtrim(trim(line), ";"), '#');
		if (line.empty() || line[0] == '#') {
			return Config::getParsedLine(fileStream, isMainContext, lineWords, CONTEXT_DIRECTIVE); // if it is empty line just call function one more time
		}
		if (!isMainContext && line == "}") {
			return -1;
		}
		*lineWords = ft_split(line, " ");
		// main context doesn't have any directives containing one word
		if (lineWords->size() < 2) {
			fatalError("Key doesn't have value!", 11);
		}
		string key = (*lineWords)[0];
		int stringIndex = getStringIndexFromArray(key, CONTEXT_DIRECTIVE);
		if (stringIndex < 0)
			fatalError(string("Failed to parse config. unknown directive: ") + key, 13);
		else
			return stringIndex;
	}
	return -1;
}

Config::Config(const string& filename)
{
	ifstream fileStream;
	string line;
	vector<string> lineWords;
	ssize_t directiveIndex;

	fileStream.open(filename);
	if (!fileStream.is_open()) {
		fatalError("Failed to open file", 10);
	}
	while ((directiveIndex = Config::getParsedLine(&fileStream, true, &lineWords, MAIN_CONTEXT_DIRECTIVES)) != -1) {
		switch (directiveIndex) {
		case 0: // error_page codeInInt pathToFile
			Config::addErrorPage(lineWords, getErrorPages());
			break;
		case 1: // server
			if (lineWords.size() != 2 && trim(lineWords[1]) != "{") {
				fatalError("Failed to parse server", 12);
			}
			_servers.push_back(new ServerContext(&fileStream));
			break;
		case 2: // root
			Config::parseRoot(lineWords, _root);
			break;
		case -1: // not found
		default:
			fatalError("Unexpected value in switch!", 13);
			break;
		}
	}
	fileStream.close();
	checkDefaultValues();
	setDefaultDirectives();
}

void Config::checkDefaultValues()
{
	if (_servers.empty())
		fatalError("Please fill the config file with at least one server directive!", 10);
}

Config::~Config()
{
	_errorPages.clear();
	_servers.clear();
}

/**
 * Validate and add errorPage, in case of the error quit with error.
 * Common method for all error pages in main, server and location contexts
 * @param errorPage string which contain error code in int and substring with pathToFile
 */
void Config::addErrorPage(const vector<string>& lineWords, vector<pair<int, std::string> >& errorPages)
{
	if (lineWords.size() != 3) {
		fatalError("Value of key _error_page isn't correct!", 20);
	}

	int code = stoi(lineWords[1]);
	if (code < 400 || code > 600) {
		fatalError("Unsupported code error page!", 21);
	}

	string pathToFile = lineWords[2];
	errorPages.push_back(std::make_pair(code, pathToFile));
}

vector<pair<int, string> >& Config::getErrorPages() { return _errorPages; }

void Config::setErrorPages(vector<pair<int, string> >& errorPages) { _errorPages = errorPages; }

void Config::parseClientMaxBodySize(string toParseValue, size_t* value)
{
	size_t idx, result;
	try {
		result = stoul(toParseValue, &idx);
	} catch (const std::invalid_argument& ia) {
		fatalError("Failed to parse client_max_body_size directive!", 23);
	} catch (const std::out_of_range& oor) {
		fatalError("Failed to parse client_max_body_size directive!", 24);
	}
	if (toParseValue.size() > idx + 1 || (toParseValue.size() == idx + 1 && toParseValue[idx] != 'm')) {
		fatalError("Failed to parse client_max_body_size directive!", 25);
	}
	if (toParseValue[idx] == 'm') {
		result *= 1024;
	}
	*value = result;
}
void Config::parseIndex(const vector<string>& lineWords, vector<string>& index)
{
	for (size_t i = 1; i < lineWords.size(); ++i) {
		index.push_back(lineWords[i]);
	}
}

void Config::parseRoot(const vector<string>& lineWords, string& root)
{
	if (lineWords.size() != 2) {
		fatalError("Failed to parse root directive invalid number of arguments root!", 26);
	}
	root = lineWords[1];
}

void Config::printConfig()
{
	cout << "GLOBAL CONFIG" << endl;
	cout << endl << "Error pages:" << endl;
	for (vector<pair<int, string> >::iterator it = _errorPages.begin(); it != _errorPages.end(); ++it) {
		cout << it->first << " " << it->second << endl;
	}
	cout << endl;
	for (size_t i = 0; i < _servers.size(); ++i) {
		_servers[i]->printConfig();
	}
}

// This method returns unique values from all listeners.
set<pair<string, int> > Config::getVirtualServersAddresses()
{
	set<pair<string, int> > result;
	for (vector<ServerContext*>::iterator it = this->_servers.begin(), ite = this->_servers.end(); it != ite; ++it) {
		for (vector<pair<string, int> >::const_iterator itL = (*it)->getListeners().begin(), iteL = (*it)->getListeners().end(); itL != iteL; ++itL) {
			result.insert(pair<string, int>(itL->first, itL->second));
		}
	}
	return result;
}

ServerContext* Config::getServersByIpPortAndHost(const string& ip, const int& port, const string& host)
{
	vector<ServerContext*> possibleResults;
	for (vector<ServerContext*>::iterator it = _servers.begin(), ite = _servers.end(); it != ite; ++it) {
		const vector<pair<string, int> > listeners = (*it)->getListeners();
		for (vector<pair<string, int> >::const_iterator itListeners = listeners.begin(), iteListeners = listeners.end(); itListeners != iteListeners; ++itListeners) {
			if ((itListeners->first.empty() || itListeners->first == "*" || itListeners->first == ip) && itListeners->second == port) {
				possibleResults.push_back(*it);
			}
		}
	}

	for (vector<ServerContext*>::iterator it = possibleResults.begin(), ite = possibleResults.end(); it != ite; ++it) {
		vector<string> serverNames = (*it)->getServerNames();
		for (vector<string>::iterator itServerName = serverNames.begin(), iteServerName = serverNames.end(); itServerName != iteServerName; ++itServerName) {
			if (*itServerName == host)
				return *it;
		}
	}

	// if server wasn't found nginx just use first server
	return possibleResults.empty() ? (*_servers.begin()) : possibleResults[0];
}

// TODO:: сдлеать так чтобы не было взаимоисключающих полей, решить на этапе парсинга
/**
 * Get single location by ip, port, host and uri
 * @param ip
 * @param port
 * @param host
 * @param uri
 * @return location
 */
LocationContext* Config::getLocationContext(const string& ip, const int& port, const string& host, const string& uri)
{
	ServerContext* server = getServersByIpPortAndHost(ip, port, host);

	vector<LocationContext*> locations = server->getLocationContexts(uri);
	LocationContext* result = *locations.begin();
	for (vector<LocationContext*>::iterator it = locations.begin(), ite = locations.end(); it != ite; ++it) {
		if (result->getLocation().length() < (*it)->getLocation().length())
			result = *it;
	}
	return result;
}

/**
 * set default directives
 * @param location
 * @param server
 */
void Config::setDefaultDirectives()
{
	// set default values for root
	if (_root.empty())
		_root = ROOT_DEFAULT;
	// set default values for servers
	for (vector<ServerContext*>::iterator it = _servers.begin(), ite = _servers.end(); it != ite; ++it) {
		if ((*it)->getRoot().empty())
			(*it)->setRoot(_root);
		(*it)->setErrorPagesFromConfigContext(_errorPages);
		// set default values for location
		(*it)->setDefaultDirectives();
	}
}
