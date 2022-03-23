#ifndef SERVERCONTEXT_HPP
#define SERVERCONTEXT_HPP

#include "../utils/utils.hpp"
#include "LocationContext.hpp"
#include <fstream>
#include <string>
#include <utility>
#include <vector>

using std::ifstream;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

const string SERVER_CONTEXT_DIRECTIVES[] = { //
	"server_name", "listen", "autoindex", "client_max_body_size", "error_page", "index", "location", ""
};

class LocationContext;

class ServerContext {
public:
	ServerContext(std::ifstream* fileStream);
	~ServerContext();

	bool isValid();

	const vector<string>& getServerNames() const;
	void setServerNames(const vector<string>& serverNames);
	bool isAutoIndex() const;
	void setAutoIndex(bool autoIndex);
	void addPortListener(string value);
	void addAddressPortListener(string value);
	void addAddressListener(string value);
	vector<pair<int, string> >& getErrorPages();
	vector<string>& getIndex();
	void printConfig();

private:
	bool _autoIndex; // false by default
	size_t _clientMaxBodySize; // client_max_body_size default 1m which is 10 ^ 6 bytes
	vector<pair<int, string> > _errorPages; // specify default page from our server
	vector<string> _index;
	vector<pair<string, int> > _listenes;
	vector<string> _serverNames;
	vector<LocationContext*> _locations;
};

#endif
