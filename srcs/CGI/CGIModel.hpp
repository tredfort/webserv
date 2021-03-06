#ifndef CGIMODEL_HPP
#define CGIMODEL_HPP

#include "../utils/usings.hpp"

struct CGIModel {
	string	pathToFile;
	bool	isSuccess;
	int		code;
};

struct ExecveArguments {
	char	*pathToExecutable;
	char	**args;
	char	**env;

	ExecveArguments() : pathToExecutable(NULL), args(NULL), env(NULL) {}
	~ExecveArguments() { clearEverything(); }

private:
	void	clearEverything() {
		if (args) {
			for (int i = 0; args[i]; i++)
				delete[] args[i];
			delete[] args;
		}
		if (env) {
			for (int i = 0; env[i]; i++)
				delete[] env[i];
			delete[] env;
		}
		if (pathToExecutable) {
			delete[] pathToExecutable;
		}
	}
};

#endif