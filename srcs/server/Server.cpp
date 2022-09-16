#include "Server.hpp"

Server::Server(Config* config, Env& env)
	: _config(config)
	, _handler(RequestHandler(config, env))
	, _env(env)
{
}

Server::~Server() { delete _config; }

void Server::createSockets()
{
	set<pair<string, int> > addresses = _config->getVirtualServersAddresses();
	for (set<pair<string, int> >::const_iterator it = addresses.begin(), ite = addresses.end(); it != ite; ++it) {
		Socket* socket = new Socket(AF_INET, SOCK_STREAM, 0);
		socket->setAddressReuseMode();
		socket->setNonblockMode();
		//		socket->bindToAddress("127.0.0.1", "8080");
		socket->bindToAddress(it->first, it->second);
		socket->startListening(SOMAXCONN);
		_sockets.push_back(socket);
		_pollfds.push_back(fillPollfd(socket->getSockfd(), POLLIN));
	}
}

/**
 * Start server. There is infinite loop, which handle the request
 */
void Server::start()
{
	createSockets();

	while (true) {
		try {
			polling();
			handleEvents();
		} catch (std::bad_alloc& e) {
			cout << "Allocation failed " << e.what() << endl;
		} catch (std::exception& e) {
			cout << "Unexpected error " << e.what() << endl;
		}
	}
}

void Server::stop() { }

void Server::polling()
{
	if (poll(_pollfds.data(), _pollfds.size(), -1) < 0)
		cerr << "  poll() failed: " << strerror(errno) << endl;
}

void Server::handleEvents()
{
	for (size_t i = 0, socksCount = _sockets.size(), fdsCount = _pollfds.size(); i < fdsCount; ++i) {
		if (_pollfds[i].revents == 0)
			continue;
		if (i < socksCount && _pollfds[i].revents & POLLIN) {
			acceptNewClients(_sockets[i]);
			continue;
		} else if (i >= socksCount) {
			WebClient* client = _clients[i - socksCount];
			if (_pollfds[i].revents & POLLHUP) {
				closeConnection(client, i);
				break;
			}
			if (_pollfds[i].revents & POLLIN) {
				receiveRequest(client, _pollfds[i].events);
			}
			if (_pollfds[i].revents & POLLOUT) {
				sendResponse(client, _pollfds[i].events);
			}
		}
	}
}

void Server::acceptNewClients(Socket* socket)
{
	while (true) {
		int client_fd = accept(socket->getSockfd(), NULL, NULL);
		if (client_fd <= 0) {
			break;
		}
		_pollfds.push_back(fillPollfd(client_fd, POLLIN));
		WebClient* client = new WebClient(client_fd, socket->getIp(), socket->getPort());
		_clients.push_back(client);
	}
}

void Server::receiveRequest(WebClient* client, short& events)
{
	char buffer[BUFFER_SIZE];

	bzero(buffer, BUFFER_SIZE);
	cout << "User listens:" << endl;
	ssize_t bytesRead = recv(client->getFd(), buffer, BUFFER_SIZE, 0);

	if (bytesRead < 0) {
		cout << "Error during request receipt from fd: " << client->getFd() << "." << endl;
		events = POLLHUP;
	} else if (bytesRead == 0) {
		cout << "Client with fd: " << client->getFd() << " ended session." << endl;
		events = POLLHUP;
	} else {
		client->getRequest()->appendBuffer(string(buffer, bytesRead));
		_parser.parseRequest(client->getRequest());
		if (_parser.isReadyRequest(client->getRequest())) {
			printRequest(client->getRequest());
			events = POLLOUT;
		}
	}
}

void Server::sendResponse(WebClient* client, short& events)
{
	if (client->getResponse()->getBuffer().empty()) {
		_handler.formResponse(client);
	}
	if (!client->getResponse()->getBuffer().empty()) {
		string buffer = client->getResponse()->getBuffer();
		ssize_t sendBytes = send(client->getFd(), buffer.c_str(), buffer.size(), 0);

		if (sendBytes < 0) {
			cout << "Client ended the userfd!" << client->getFd() << endl;
		}

		client->getResponse()->setBuffer(buffer.substr(sendBytes));
		if (client->getResponse()->getBuffer().empty()) {
			printResponse(client->getResponse());
			client->clear();
			events = POLLIN;
		}
	}
}

void Server::closeConnection(WebClient* client, size_t number)
{
	close(client->getFd());
	_pollfds.erase(_pollfds.begin() + number);
	_clients.erase(_clients.begin() + number - _sockets.size());
	delete client;
}
