#include "utils.hpp"

std::vector<string> ft_split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos;
	do {
		pos = str.find(delim, prev);
		if (pos == std::string::npos)
			pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty())
			tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

std::string ltrim(std::string str, const std::string& chars)
{
	str.erase(0, str.find_first_not_of(chars));
	return str;
}

std::string rtrim(std::string str, const std::string& chars)
{
	str.erase(str.find_last_not_of(chars) + 1);
	return str;
}

std::string trim(std::string str, const std::string& chars) { return ltrim(rtrim(str, chars), chars); }

void sendMessage(int fd, string msg)
{
	cout << "sending message to " << fd << ": " << msg << endl;
	send(fd, msg.c_str(), msg.size(), 0);
}

int isChannelName(string channelName)
{
	if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
		return 0;
	return 1;
}

void eraseSpacesInFront(string& cmd)
{
	int i = 0;
	while (cmd[i] && cmd[i] == ' ')
		i++;
	cmd.erase(0, i);
}

struct pollfd fillPollfd(int sd, short events)
{
	struct pollfd fd;
	fd.fd = sd;
	fd.events = events;
	return fd;
}

bool isChannel(string receiver)
{
	if (receiver[0] == '#' || receiver[0] == '&') {
		return (true);
	}
	return (false);
}

bool isMaskMatch(const string& str, const string& mask) { return isMaskMatch(str.c_str(), mask.c_str()); }

bool isMaskMatch(const char* str, const char* mask)
{
	if (!str || !mask)
		return 0;
	do {
		if (StarCmp(str, mask))
			return 1;
	} while (*str++);
	return 0;
}

// StarCompare() helper function
bool StarCmp(const char* str, const char* mask)
{
	if (*mask == '\0')
		return 1;
	if (*mask == '*') {
		if (*str) {
			// advance str and use the * again
			if (StarCmp(str + 1, mask))
				return 1;
		}
		// let * match nothing and advacne to the next pattern
		return StarCmp(str, mask + 1);
	}
	if (*mask == *str) {
		return StarCmp(str + 1, mask + 1);
	}
	return 0;
}

void stringToLowerCase(string& str) { std::transform(str.begin(), str.end(), str.begin(), tolower); }

in_port_t getValidPort(const string& port)
{
	char* endPtr;
	long _port = std::strtol(port.c_str(), &endPtr, 10);

	if (*endPtr || _port <= 0 || _port > USHRT_MAX) {
		cout << "ERROR: Invalid port!" << endl;
		exit(EXIT_FAILURE);
	}
	return htons(_port);
}

void fatalError(const string& errorMessage, int errorCode)
{
	std::cerr << errorMessage << std::endl;
	exit(errorCode);
}

int getStringIndexFromArray(const string& str, const string* array)
{
	int i = 0;
	while (!array[i].empty()) {
		if (str == array[i])
			return i;
		++i;
	}
	return -1;
}

bool isDigits(const std::string& str) { return str.find_first_not_of("0123456789") == std::string::npos; }

bool isFileExists(string& pathToFile)
{
	std::ifstream file(pathToFile);
	return file.is_open();
}

bool isDirectory(string& pathToFile)
{
	struct stat file;
	return stat(pathToFile.c_str(), &file) != -1 && S_ISDIR(file.st_mode);
}

void printStringVector(const vector<string>& v)
{
	for (vector<string>::const_iterator it = v.begin(); it != v.end(); ++it) {
		cout << *it << endl;
	}
	cout << endl;
}

string removeAfter(string s, char c)
{
	size_t pos = s.find(c);
	if (pos == string::npos) {
		return s;
	}
	return s.substr(0, pos);
}

string getWholeFile(const string pathToFile)
{
	std::ifstream file(pathToFile.c_str());
	std::ostringstream sstr;
	sstr << file.rdbuf();
	return sstr.str();
}
