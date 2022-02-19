#include "utils.hpp"

std::vector<string> ft_split(const string& s, const string& delimiter) { return ft_split<vector<string> >(s, delimiter); }

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
