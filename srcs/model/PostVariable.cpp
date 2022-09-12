#include "PostVariable.hpp"

/*
 * example:
------WebKitFormBoundaryAFS3exYuAEvTsOZ8
Content-Disposition: form-data; name="file6"; filename=""
Content-Type: application/octet-stream

 "\r\nContent-Disposition: form-data; name=\"file1\"; filename=\"test.txt\"\r\nContent-Type: text/plain\r\n\r\n123\r\n456\r\n789\r\n"
*/
PostVariable::PostVariable(string value)
{
	const string delim = "\r\n\r\n";
	_body = getStringAfterTarget(value, delim);
	size_t pos = value.find(delim);
	vector<string> headers = ft_split(value.substr(0, pos), "\r\n");
	for (vector<string>::iterator it = headers.begin(); it != headers.end(); ++it) {
		vector<string> headersInString = ft_split(*it, ";");

		for (vector<string>::iterator it1 = headersInString.begin(), ite1 = headersInString.end(); it1 != ite1; ++it1) {
			vector<string> splitByEqual = ft_split(*it1, "=");
			vector<string> splitByCommon = ft_split(*it1, ":");
			if (splitByCommon.size() > 1) {
				_headings[trim(splitByCommon[0], "\t\n\v\f\r \"")] = trim(splitByCommon[1], "\t\n\v\f\r \"");
			} else if (splitByEqual.size() > 1) {
				_headings[trim(splitByEqual[0], "\t\n\v\f\r \"")] = trim(splitByEqual[1], "\t\n\v\f\r \"");
			} else {
				// I'm not sure that we can have variable names without values, just skip it
			}
		}
	}
}

map<string, string> PostVariable::getHeadings() { return _headings; }

string PostVariable::getBody() { return _body; }

string PostVariable::getHeader(string header) { return _headings[header]; }
