#pragma once

#include <exception>
#include <fstream>
#include <iostream>

class FileReader {

public:
	static std::string readFile(const std::string&);

private:
	FileReader();
	virtual ~FileReader();

public:
	class FileNotFoundException : public std::exception {

	private:
		std::string fileName;

	public:
		explicit FileNotFoundException(const std::string&);
		const char* what() const noexcept override;
	};
};
