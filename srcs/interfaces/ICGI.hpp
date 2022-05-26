#ifndef ICGI_HPP
#define ICGI_HPP

#include "../utils/usings.hpp"

class ICGI {
public:
	virtual ~ICGI() { };
	virtual bool	getPathToFileWithResult(string & path) = 0;
};

#endif