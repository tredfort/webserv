
#include "FileReader.hpp"

std::string FileReader::readFile(const std::string& fileName)
{
	std::ifstream inFile(fileName, std::ios::binary);
	std::string result;

	inFile.seekg(0, std::ios::end);
	try {
		result.resize(inFile.tellg());
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	inFile.seekg(0, std::ios::beg);
	inFile.read((char*)result.data(), result.size());
	inFile.close();
	return result;
}

FileReader::~FileReader() { }

FileReader::FileReader() { }
#ifdef UNIX_OS
FileReader::FileNotFoundException::~FileNotFoundException() { }

const char* FileReader::FileNotFoundException::what() const throw()
{
	std::string* msg = new std::string(fileName + " could not be opened for reading!");
	return msg->c_str();
}
#else
FileReader::FileNotFoundException::~FileNotFoundException() throw() { }

const char* FileReader::FileNotFoundException::what() const throw()
{
	std::string* msg = new std::string(fileName + " could not be opened for reading!");
	return msg->c_str();
}

#endif
