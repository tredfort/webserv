#include "Response.hpp"

Response::Response()
{

	//    file.open(fileName, std::ios::binary | std::ios::ate);
	//    try {
	//        fileSize = file.tellg();
	//    } catch (std::exception& e) {
	//        std::cout << e.what() << std::endl;
	//    }
	//    file.seekg(0, std::ios::beg);
}

Response::~Response() { }

std::string Response::toString() const {
	return toSend;
}
