#ifndef IRESPONSE_HPP
#define IRESPONSE_HPP

#include <iostream>

class IResponse {
public:
	virtual std::string toString() const = 0;
    virtual ~IResponse() {};
};

#endif
