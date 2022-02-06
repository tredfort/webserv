#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <vector>
# include <fstream>
# include <iostream>
# include "../interfaces/IResponse.hpp"

#define SIZE (1024)


class Response : public IResponse {

public:
//    std::string fileName;
//    std::ifstream file;
//    long fileSize;
//    bool headersSent;
	std::string toSend;

public:
    Response();
    ~Response();

	std::string toString() const;

private:
    Response(const Response&);
    Response &operator=(const Response&);

};


#endif
