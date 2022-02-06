#include <arpa/inet.h>
#include <sys/poll.h>
#include "Server.hpp"

Server::Server(Config* config)
	: _socket(NULL)
	, _config(config)
    , _repository(NULL)
{
	_address.sin_family = AF_INET;
	_address.sin_port = 8080;
	if (inet_aton("127.0.0.1", &_address.sin_addr) == 0) {
		cout << "ERROR: Invalid ip address!" << endl;
		exit(EXIT_FAILURE);
	}
}

Server::~Server()
{
	delete _socket;
	delete _config;
	delete _repository;
}

void Server::createSocket()
{
	_socket = new Socket(AF_INET, SOCK_STREAM, 0);

	_socket->setAddressReuseMode();
	_socket->setNonblockMode();
	_socket->bindToAddress(reinterpret_cast<struct sockaddr*>(&_address));
	_socket->startListening(SOMAXCONN);
}

/**
 * Start server. There is infinite loop, which handle the request
 */
void Server::start()
{
	createSocket();
	_pollfds.push_back(fillPollfd(_socket->getSockfd(), POLLIN));

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

void Server::polling()
{
	if (poll(&(_pollfds.front()), _pollfds.size(), POLL_TIMEOUT) < 0)
		cerr << "  poll() failed: " << strerror(errno) << endl;
}

void Server::handleEvents()
{
    for (pollfdType::iterator it = _pollfds.begin(), ite = _pollfds.end(); it != ite; ++it) {
        if (it->revents == 0)
            continue;
        else if (it->revents & POLLIN) {
            if (it->events == POLLIN)
                acceptNewClients();
            else if (!receiveRequest()) {
                _pollfds.erase(it);
                break;
            }
        }
        else if (it->revents & POLLOUT && !sendResponse()) {
            _pollfds.erase(it);
            break;
        }
    }
}

void Server::acceptNewClients()
{
	int userfd;

	while (true) {
		userfd = accept(_socket->getSockfd(), NULL, NULL);
		if (userfd <= 0)
			break;
		_pollfds.push_back(fillPollfd(userfd, POLLIN | POLLOUT));
		_repository->save(new WebClient(userfd, _address.sin_port));
	}
}

bool Server::receiveRequest(WebClient* client)
{
	char buffer[IRC_MSG_LEN];
	ssize_t bytesRead;

	cout << "User listens:" << endl;
	bytesRead = recv(client->getFd(), buffer, sizeof(buffer), 0);

	if (bytesRead <= 0) {
		cout << "Client ended the _userfd!" << client->getFd() << endl;
		close(client->getFd());
		_repository->deleteById(client->getFd());
		return false;
	}

	if (bytesRead > 0) {
		buffer[bytesRead] = '\0';
        client->getRequest()->setBuffer(string(buffer, bytesRead));
	}

	return true;
}

bool Server::sendResponse(WebClient *client) {

}
