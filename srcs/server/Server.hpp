#ifndef SERVER_HPP
#define SERVER_HPP

#include "../config/Config.hpp"
#include "../requestParser/RequestParser.hpp"
#include "../requestHandler/RequestHandler.hpp"
#include "../model/WebClient.hpp"
#include "../repository/Repository.hpp"
#include "../utils/utils.hpp"
#include "../utils/constants.hpp"
#include "Socket.hpp"
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <sys/poll.h>

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
    typedef vector<struct pollfd> pollVector;
    RequestParser _parser;
    RequestHandler _handler;
    Socket *_socket;
    Config *_config;
    Repository *_clientsRepo;
    struct sockaddr_in _address;
    pollVector _pollfds;

public:
    Server(Config *config);

    ~Server();

    void createSocket();

    void start();

    void polling();

    void acceptNewClients();

    void handleEvents();

    bool receiveRequest(WebClient *client);

    bool sendResponse(WebClient *client);

private:
    Server &operator=(Server const &); // Don't implement.
    Server(Server const &); // Don't Implement.
};

#endif
