#include "Config.hpp"

#include <utility>

// TODO:: set default values... looks like there are not any default values
Config::Config(const string& filename)
{
	ifstream fileStream;
	string line;

	fileStream.open(filename);
	if (!fileStream.is_open()) {
		fatalError("Failed to open fail", 10);
	}
	while (getline(fileStream, line)) {
		line = rtrim(trim(line), ";");
		if (line.empty() || line[0] == '#') {
			continue;
		}
		vector<string> lineWords = ft_split(line, " ");
		// main context doesn't have any directives containing one word
		if (lineWords.size() < 2) {
			fatalError("Key doesn't have value! Main context", 11);
		}
		string key = lineWords[0];
		int stringIndex = getStringIndexFromArray(key, MAIN_CONTEXT_DIRECTIVES);
		switch (stringIndex) {
		case 0: // _error_page codeInInt pathToFile
			Config::addErrorPage(lineWords, getErrorPages());
			break;
		case 1: // server
			if (lineWords.size() != 2 && trim(lineWords[1]) != "{") {
				fatalError("Failed to parse server", 12);
			}
			_servers.push_back(new ServerContext(&fileStream));
			break;
		case -1: // not found
		default:
			fatalError(string("Failed to parse config. unknown directive: ") + key, 13);
			break;
		}
	}

	fileStream.close();
}

// TODO:: free allocated memory
Config::~Config()
{
	_errorPages.clear();
	// TODO:: before clear should I free each server?
	_servers.clear();
}

/**
 * Validate and add errorPage, in case of the error quit with error.
 * Common method for all error pages in main, server and location contexts
 * @param errorPage string which contain error code in int and substring with pathToFile
 */
void Config::addErrorPage(const vector<string>& lineWords, vector<pair<int, std::string> > errorPages)
{
	if (lineWords.size() != 3) {
		fatalError("Value of key _error_page isn't correct!", 20);
	}

	int code = stoi(lineWords[1]);
	if (code < 400 || code > 600) {
		fatalError("Unsupported code error page!", 21);
	}

	string pathToFile = lineWords[2];
	std::ifstream errorPageFile(pathToFile);
	if (!errorPageFile.is_open()) {
		fatalError("Specified error page doesn't exist! not able to find a file with: " + pathToFile, 22);
	}
	errorPages.push_back(std::make_pair(code, pathToFile));
}

const vector<pair<int, string> >& Config::getErrorPages() const { return _errorPages; }

void Config::setErrorPages(const vector<pair<int, string> >& errorPages) { _errorPages = errorPages; }

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
		std::ifstream indexFile(lineWords[i]);
		if (!indexFile.is_open()) {
			fatalError("Failed to open file specified in index directive!", 26);
		}
		index.push_back(lineWords[i]);
	}
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
