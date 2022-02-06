#ifndef IREQUESTHEDEARS_HPP
#define IREQUESTHEDEARS_HPP

#include <iostream>

class IHeader {

public:
	virtual ~IHeader() {};

	virtual std::string getHeader(const std::string&) const = 0;
	virtual bool emptyHeader() const = 0;

};

#endif
