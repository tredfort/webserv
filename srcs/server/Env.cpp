#include "Env.hpp"

Env::Env(char** env) {
	parseEnv(env);
}

Env::~Env() { }

void	Env::parseEnv(char** env) {

	for (int i = 0; env[i]; i++) {
		string	argument(env[i]);
		handleAgrument(argument);
	}
}

void	Env::handleAgrument(string argument) {

	string::size_type pos = argument.find('=');
	if (pos == string::npos) {
		return ;
	}
	const string key = argument.substr(0, pos);
	if (key == "PATH") {
		_paths = ft_split(argument.substr(pos + 1, argument.size()), ":");
	} else if (!key.empty()) {
		string value = argument.substr(pos + 1, argument.size());
		_arguments.insert(make_pair(key, value));
	}
}

vector<string>&	Env::getPaths() {
	return _paths;
}

map<string, string>& Env::getArguments() {
	return _arguments;
}