
#include "FileReader.hpp"

std::string FileReader::readFile(const std::string& fileName)
{

	std::ifstream inFile(fileName, std::ios::binary);
	// if (!inFile) {
	// 	throw FileNotFoundException(fileName);
	// }
	std::string result;
	inFile.seekg(0, std::ios::end);
	//	TODO: Разобраться с resize()
	try {
		result.resize(inFile.tellg());
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	inFile.seekg(0, std::ios::beg);
	inFile.read((char*)result.data(), result.size());

	/*int size = inFile.seekg(0, std::ios_base::end).tellg();
	inFile.seekg(0);
	char *buffer = new char [size + 1];
	inFile.read(buffer, size);
	buffer[size] = 0;*/
	//	std::cout << buffer << std::endl;
	inFile.close();
	//	std::istringstream result (result);
	return result;
}

FileReader::~FileReader() { }

FileReader::FileReader() { }

FileReader::FileNotFoundException::FileNotFoundException(const std::string& fileName)
	: fileName(fileName)
{
}

const char* FileReader::FileNotFoundException::what() const noexcept
{
	std::string* msg = new std::string(fileName + " could not be opened for reading!");
	return msg->c_str();
}
