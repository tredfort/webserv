//
// Created by Twycross Redfort on 5/27/22.
//

//#include "../srcs/requestHandler/RequestHandler.hpp"
#include "../srcs/requestParser/RequestParser.hpp"

int main()
{
	RequestParser parser;
//	RequestHandler handler(nullptr);
//	Response* response = new Response();
	Request* request = new Request();

	char* buffer = (char*)"POST / HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nContent-Length: 247\r\nCache-Control: max-age=0\r\nsec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"101\", \"Google Chrome\";v=\"101\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"macOS\"\r\nUpgrade-Insecure-Requests: 1\r\nOrigin: http://localhost:8080\r\nContent-Type: multipart/form-data; boundary=----WebKitFormBoundarywraZbZh6ABBWOreo\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.64 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\nSec-Fetch-Site: same-origin\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-User: ?1\r\nSec-Fetch-Dest: document\r\nReferer: http://localhost:8080/\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: ru-RU,ru;q=0.9\r\n\r\n------WebKitFormBoundarywraZbZh6ABBWOreo\r\nContent-Disposition: form-data; name=\"file\"; filename=\"Dockerfile\"\r\nContent-Type: application/octet-stream\r\n\r\nFROM nginx\nCOPY index.html /usr/share/nginx/html\n\r\n------WebKitFormBoundarywraZbZh6ABBWOreo--\r\n";

	request->setBuffer(buffer);
	parser.processRequest(request);
//	handler.formResponse(request, response);

}