#include "Server.hpp"

Server::Server(Config* config)
	: _config(config) { }

Server::~Server()
{
	delete _config;
}

void Server::createSockets()
{
	Socket* socket = new Socket(AF_INET, SOCK_STREAM, 0);

	socket->setAddressReuseMode();
	socket->setNonblockMode();
	socket->bindToAddress("127.0.0.1", "8080");
	socket->startListening(SOMAXCONN);
	_sockets.push_back(socket);
	_pollfds.push_back(fillPollfd(socket->getSockfd(), POLLIN));
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
	if (poll(&(_pollfds.front()), _pollfds.size(), -1) < 0)
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
			bool keepAlive = true;
			WebClient* client = _clients[i - socksCount];
			if (_pollfds[i].revents & POLLIN) {
				keepAlive = receiveRequest(client);
			} else if (_pollfds[i].revents & POLLOUT) {
				keepAlive = sendResponse(client);
			}
			if (!keepAlive) {
				_pollfds.erase(_pollfds.begin() + i);
				_clients.erase(_clients.begin() + i - socksCount);
				delete client;
				break;
			}
			_pollfds[i].events = client->getStatus();
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
		WebClient* client = new WebClient(client_fd, socket->getSockfd(), _pollfds.back().events);
		_clients.push_back(client);
	}
}

bool Server::receiveRequest(WebClient* client)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytesRead;

	cout << "User listens:" << endl;
	bytesRead = recv(client->getFd(), buffer, sizeof(buffer), 0);

	if (bytesRead <= 0) {
		cout << "Client ended the _userfd!" << client->getFd() << endl;
		close(client->getFd());
		return false;
	}

	client->getRequest()->setBuffer(string(buffer, bytesRead));
	_parser.processRequest(client);

	return true;
}

bool Server::sendResponse(WebClient* client)
{
	if (!client)
		return false;
	_handler.formResponse(client);
	if (!client->getResponse()->toSend.empty()) {
		string buffer = client->getResponse()->toSend;
		ssize_t sendBytes = send(client->getFd(), buffer.c_str(), buffer.size(), 0);

		if (sendBytes <= 0) {
			cout << "Client ended the _userfd!" << client->getFd() << endl;
			close(client->getFd());
			return false;
		}

		client->getResponse()->toSend = buffer.substr(sendBytes);
		std::cout << "Sent " << sendBytes << " bytes to fd: " << client->getFd() << std::endl;
		if (client->getResponse()->toSend.empty()) {
			client->update();
		}
		client->setStatus(POLLIN);
	}
	return true;
}
