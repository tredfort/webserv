#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <set>
#include <sstream>
#include <string>
#include <sys/poll.h>
#include <sys/stat.h>
#include <vector>

#include "usings.hpp"

/**
 * split string with delimiter. It skips empty strings
 * @param s source string
 * @param delim delimiter string
 * @return vector of strings
 */

std::vector<string> ft_split(const std::string& str, const std::string& delim);

/**
 * Create pollFdPointer with pollFd structure
 * @param fd
 * @return
 */

std::string ltrim(std::string str, const std::string& chars = "\t\n\v\f\r ");
std::string rtrim(std::string str, const std::string& chars = "\t\n\v\f\r ");
std::string trim(std::string str, const std::string& chars = "\t\n\v\f\r ");

bool isOnlyDigits(const std::string& str);

struct pollfd fillPollfd(int sd, short events);

template <class Container> typename Container::value_type getPopFront(Container c)
{
	if (c.empty())
		return typename Container::value_type();
	typename Container::value_type result = c.front();
	c.erase(c.begin());
	return result;
}

bool StarCmp(const char* str, const char* mask);

in_port_t getValidPort(const string& port);

void fatalError(const string& errorMessage, int errorCode = 1);

/**
 *
 * @param str
 * @param array the last element of the array SHOULD BE EMPTY string
 * @return
 */
int getStringIndexFromArray(const string& str, const string* array);

bool isFileExists(const string& pathToFile);

bool isDirectory(const string& pathToFile);

time_t getFileModificationDate(string& pathToFile);

// void printStringVector(const set<string>&);

template <class T> void printStringVector(T& v)
{
	for (typename T::const_iterator it = v.begin(); it != v.end(); ++it) {
		cout << *it << endl;
	}
	cout << endl;
}

string removeAfter(string, char);

bool isAccessRights(string& pathToFile);

bool startsWith(const string&, const string&);

/**
 * Create new iteratable class where each element return true by filterFunction
 * @tparam T container Type
 * @param c container
 * @return
 */
template <class T> T filter(T& c, bool (*function)(typename T::value_type))
{
	T result;
	for (typename T::const_iterator it = c.begin(); it != c.end(); ++it) {
		if (function(*it))
			result.push_back(*it);
	}
	return result;
}

string getErrorPageBody(string errorMessage);

string getRedirectPageBody(std::pair<int, string> redirect);

string getFileFormat(string& path);

string getFileName(const string& path);

int stringToInt(const string& str);

bool createFile(const string& pathToFile, const string& content);

string getParentFilePath(const string& pathToFile);

class CastToIntException : public std::exception {
public:
	const char *what() const throw();
};

class BadChunkedRequestException : public std::exception {
public:
	const char *what() const throw();
};

#endif
