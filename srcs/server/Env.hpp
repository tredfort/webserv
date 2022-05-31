#ifndef ENV_HPP
#define ENV_HPP

#include "../utils/usings.hpp"
#include "../utils/utils.hpp"

class Env {

private:
	vector<string>		_paths;
	map<string, string>	_arguments;

public:
	Env(char** env);
	~Env();

	vector<string>&	getPaths();
	map<string, string>& getArguments();

private:
	void	parseEnv(char** env);
	void	handleAgrument(string argument);
};

#endif