#ifndef SERVER_HPP
#define SERVER_HPP

#include "../config/Config.hpp"
#include "../constants/constants.hpp"
#include "../model/WebClient.hpp"
#include "../requestHandler/RequestHandler.hpp"
#include "../requestParser/RequestParser.hpp"
#include "../utils/utils.hpp"
#include "Env.hpp"
#include "Socket.hpp"
#include <cstring>
#include <iostream>
#include <sys/poll.h>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::vector;

/**
 * Starting WEB server,
 * checks for new connections
 */
class Server {

private:
	Config* _config;
	RequestParser _parser;
	RequestHandler _handler;
	vector<Socket*> _sockets;
	vector<WebClient*> _clients;
	vector<struct pollfd> _pollfds;
	Env _env;

public:
	explicit Server(Config* config, Env& env);

	~Server();

	void createSockets();

	void start();

	void stop();

	void polling();

	void acceptNewClients(Socket* socket);

	void handleEvents();

	void receiveRequest(WebClient* client, short& events);

	void sendResponse(WebClient* client, short& events);

	void closeConnection(WebClient*, size_t);

private:
	Server& operator=(Server const&); // Don't implement.
	Server(Server const&); // Don't Implement.
};

#endif
