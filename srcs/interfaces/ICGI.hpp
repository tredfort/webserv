#ifndef ICGI_HPP
#define ICGI_HPP

#include "../utils/usings.hpp"

class ICGI {
public:
	virtual ~ICGI() { };
	virtual string	getPathToFileWithResult() const = 0;
};

#endif