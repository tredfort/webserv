#ifndef FILEREADER_HPP
#define FILEREADER_HPP

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
#ifdef UNIX_OS
		~FileNotFoundException();
		const char* what() const noexcept override;
#else
		~FileNotFoundException() _NOEXCEPT;
		const char* what() const _NOEXCEPT;
#endif
	};
};

#endif
