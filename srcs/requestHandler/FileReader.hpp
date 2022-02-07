#pragma once

#include <iostream>
#include <fstream>
#include <exception>

class FileReader {

public:
	static std::string readFile(const std::string& );

private:
	FileReader();
	virtual ~FileReader();


public:
	class FileNotFoundException : public std::exception {

	private:
		std::string fileName;

	public:
		explicit FileNotFoundException(const std::string& ) _NOEXCEPT;
		~FileNotFoundException() _NOEXCEPT;
		const char* what() const _NOEXCEPT;
	};
};
