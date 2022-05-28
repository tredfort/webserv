#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../utils/utils.hpp"
#include "LocationContext.hpp"
#include "ServerContext.hpp"
#include <fstream>
#include <string>
#include <utility>
#include <vector>

using std::fstream;
using std::ifstream;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

const string MAIN_CONTEXT_DIRECTIVES[] = { "error_page", "server", "" };

class LocationContext;
class ServerContext;

// TODO:: comment after the directive crash the logic
/**
 * overview over config structure
 * _https://github.com/rxxxt/webserv/wiki/Architecture-overview#config-structure
 * keys and values fairly copied from here https://pohl.notion.site/Parser-060000caaff047509900210372d06f26
 */
class Config {
public:
	Config(const string&);
	~Config();

	void printConfig();

	// common methods for all context of the config
	static void addErrorPage(const vector<string>& lineWords, vector<pair<int, std::string> > errorPages);
	static void parseClientMaxBodySize(string, size_t*);
	static void parseIndex(const vector<string>& lineWords, vector<string>& index);
	static ssize_t getParsedLine(ifstream* fileStream, bool isMainContext, vector<string>* lineWords, const string* CONTEXT_DIRECTIVE);

	const vector<pair<int, string> >& getErrorPages() const;
	void setErrorPages(const vector<pair<int, string> >& errorPages);
	void printfConfig();
	LocationContext getLocationContext(const string& ip, const string& port, const string& host, const string& uri);

private:
	vector<pair<int, string> > _errorPages;
	vector<ServerContext> _servers;
	ssize_t getParseLine(
		ifstream* fileStream, bool isMainContext, vector<string>* wordsLine, const std::basic_string<char, std::char_traits<char>, std::allocator<char> >* CONTEXT_DIRECTIVE);
	void checkDefaultValues();
};

#endif
