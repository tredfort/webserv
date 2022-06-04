//
// Created by Twycross Redfort on 5/5/22.
//

#include <iostream>
#include <sys/poll.h>
#include <vector>

using std::cout;
using std::endl;

int main() {
	char	*str = (char*)"POST /webserv/resources/html_data/index.html?_ijt=u1u2d48k60b13cv6v8uvhj9j4b&_ij_reload=RELOAD_ON_SAVE HTTP/1.1\n"
				"Host: localhost:63342\n"
				"Connection: keep-alive\n"
				"Content-Length: 189\n"
				"Cache-Control: max-age=0\n"
				"sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"98\", \"Google Chrome\";v=\"98\"\n"
				"sec-ch-ua-mobile: ?0\n"
				"sec-ch-ua-platform: \"macOS\"\n"
				"Upgrade-Insecure-Requests: 1\n"
				"Origin: http://localhost:63342\n"
				"Content-Type: multipart/form-data; boundary=----WebKitFormBoundarylC238GmssyVoMlMf\n"
				"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36\n"
				"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n"
				"Sec-Fetch-Site: same-origin\n"
				"Sec-Fetch-Mode: navigate\n"
				"Sec-Fetch-User: ?1\n"
				"Sec-Fetch-Dest: document\n"
				"Referer: http://localhost:63342/webserv/resources/html_data/index.html?_ijt=u1u2d48k60b13cv6v8uvhj9j4b&_ij_reload=RELOAD_ON_SAVE\n"
				"Accept-Encoding: gzip, deflate, br\n"
				"Accept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\n"
				"Cookie: Clion-3c4b553=0bde946c-307c-4324-8f45-ae1feed147d9";

	char* str1 = (char*) "\"POST / HTTP/1.1\\r\\nHost: localhost:8080\\r\\nConnection: keep-alive\\r\\nContent-Length: 57720\\r\\nCache-Control: max-age=0\\r\\nsec-ch-ua: \\\" Not A;Brand\\\";v=\\\"99\\\", \\\"Chromium\\\";v=\\\"98\\\", \\\"Google Chrome\\\";v=\\\"98\\\"\\r\\nsec-ch-ua-mobile: ?0\\r\\nsec-ch-ua-platform: \\\"macOS\\\"\\r\\nUpgrade-Insecure-Requests: 1\\r\\nOrigin: http://localhost:8080\\r\\nContent-Type: multipart/form-data; boundary=----WebKitFormBoundaryqAq2ay1BNItc1uAP\\r\\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/98.0.4758.102 Safari/537.36\\r\\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\\r\\nSec-Fetch-Site: same-origin\\r\\nSec-Fetch-Mode: navigate\\r\\nSec-Fetch-User: ?1\\r\\nSec-Fetch-Dest: document\\r\\nReferer: http://localhost:8080/\\r\\nAccept-Encoding: gzip, deflate, br\\r\\nAccept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\\r\\nCookie: Clion-3c4b553=0bde946c-307c-4324-8f45-ae1feed147d9\\r\\n\\r\\n------WebKitFormBoundaryqAq2ay1BNItc1uAP\\r\\nContent-Disposition: form-data; name=\\\"photo\\\"; filename=\\\"Screen Shot 2022-05-04 at 10.20.22 PM.png\\\"\\r\\nContent-Type: image/png\\r\\n\\r\\n\\x89PNG\\r\\n\\U0000001a\\n\"";
//	cout << POLLHUP << endl;

	std::string s;
	cout << "isEmpty: " << (s.empty() == true ? "true" : "false") << endl;
	return 0;
}