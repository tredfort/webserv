#ifndef ICGI_HPP
#define ICGI_HPP

#include "../utils/usings.hpp"
#include "../CGI/CGIModel.hpp"

class ICGI {
public:
	virtual ~ICGI() { };
	virtual CGIModel	getPathToFileWithResult() = 0;
};

#endif