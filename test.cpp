#include <iostream>

using std::string;
using std::cout;
using std::endl;

int hexToInt(const string& str) {
	char* endPtr;
	long number = std::strtol(str.c_str(), &endPtr, 16);

	if (*endPtr || number < INT32_MIN || number > INT32_MAX) {
		throw std::exception();
	}
	return number;
}

string replace(string input, const string& target, const string& replacement) {
	for (size_t pos = input.find(target); pos < input.length() && pos != string::npos; pos = input.find(target)) {
		input.replace(pos, target.size(), replacement);
	}
	return input;
}
char getLastSymbol(string s) { return s[s.length() - 1]; }

bool startsWith(const string& str, const string& pattern) { return str.find(pattern.c_str(), 0, pattern.length()) != string::npos; }


string getPathToFile(const string& pathToFile, string root, string location)
{
	if (startsWith(pathToFile, "/")) {
		return replace(root + '/' + pathToFile, "//", "/");
	} else
		return replace(root + '/' + location + '/' + pathToFile, "//", "/");
}

string createPath(const string& path1, const string& path2) {
	if (path1.back() == '/' && path2.front() == '/') {
		return path1 + path2.substr(1);
	} else if (path1.back() != '/' && path2.front() != '/') {
		return path1 + "/" + path2;
	}
	return path1 + path2;
}

int toUnderscore(int c){
	if (c == '-')
		return '_';
	return c;
}

std::string generateNewEnv(const std::string & key, const std::string & value){
	std::string buf(key);
	transform(buf.begin(), buf.end(), buf.begin(), ::toupper);
	transform(buf.begin(), buf.end(), buf.begin(), ::toUnderscore);
	buf = "HTTP_" + buf + "=" + value;
	return buf;
}

int main() {

	cout << "env: " << generateNewEnv("key", "value") << endl;

	return 0;
}