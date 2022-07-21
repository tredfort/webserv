#ifndef SERVERCONTEXT_HPP
#define SERVERCONTEXT_HPP

#include "../utils/utils.hpp"
#include "LocationContext.hpp"
#include <cstring>
#include <fstream>
#include <utility>
#include <vector>

using std::ifstream;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

const string SERVER_CONTEXT_DIRECTIVES[] = { //
	"server_name", "listen", "autoindex", "client_max_body_size", "error_page", "index", "location", "root", ""
};

class LocationContext;

class ServerContext {
public:
	ServerContext(std::ifstream* fileStream);
	~ServerContext();

	bool isValid();

	const vector<string>& getServerNames() const;
	const vector<pair<string, int> >& getListeners() const;
	void setServerNames(const vector<string>& serverNames);
	vector<string> getServerNames();
	bool isAutoIndex() const;
	void setAutoIndex(bool autoIndex);
	void addPortListener(string value);
	void addAddressPortListener(string value);
	void addAddressListener(string value);
	map<int, string>& getErrorPages();
	vector<string>& getIndex();
	void printConfig();
	vector<LocationContext*> getLocationContexts();
	vector<LocationContext*> getLocationContexts(const string&);
	const string& getRoot();
	void setRoot(string);

	void setDefaultDirectives();

	void setErrorPagesFromConfigContext(map<int, string>& vector1);
	string getErrorPage(int code);
	void setDefaultAllowedMethods();

private:
	bool _autoIndex; // false by default
	size_t _clientMaxBodySize; // client_max_body_size default 1m which is 10 ^ 6 bytes
	map<int, string> _errorPages; // specify default page from our server
	vector<string> _index;
	vector<pair<string, int> > _listenes;
	vector<string> _serverNames;
	string _root;
	vector<LocationContext*> _locations;
	void checkDefaultValues();
};

#endif
