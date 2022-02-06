#ifndef SERVER_HPP
#define SERVER_HPP

#include "../config/Config.hpp"
#include "../model/WebClient.hpp"
#include "../repository/Repository.hpp"
#include "../utils/utils.hpp"
#include "../utils/constants.hpp"
#include "Socket.hpp"
#include <iostream>
#include <vector>

using std::cerr;
using std::pair;
using std::cout;
using std::endl;
using std::string;
using std::vector;

/**
 * Starting WEB server,
 * checks for new connections
 */
class Server {

private:
	typedef vector<struct pollfd> pollfdType;
	Socket* _socket;
    Config* _config;
    Repository* _repository;
	struct sockaddr_in _address;
	pollfdType _pollfds;

public:
	explicit Server(Config* config);
	~Server();

	void createSocket();
	void start();
	void polling();
	void acceptNewClients();
	void handleEvents();
	bool receiveRequest(WebClient* client);
	bool sendResponse(WebClient* client);

private:
	Server(Server const&); // Don't Implement.
	void operator=(Server const&); // Don't implement.
};

#endif
