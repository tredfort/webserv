#include "Server.hpp"

Server::Server(Config* config)
	: _socket(NULL)
	, _config(config)
	, _clientsRepo(new Repository())
{
	_address.sin_family = AF_INET;
	_address.sin_port = getValidPort("8080");
	if (inet_aton("127.0.0.1", &_address.sin_addr) == 0) {
		cout << "ERROR: Invalid ip address!" << endl;
		exit(EXIT_FAILURE);
	}
}

Server::~Server()
{
	delete _socket;
	delete _config;
	delete _clientsRepo;
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
	if (poll(&(_pollfds.front()), _pollfds.size(), -1) < 0)
		cerr << "  poll() failed: " << strerror(errno) << endl;
}

void Server::handleEvents()
{
	for (ssize_t i = 0, countFd = _pollfds.size(); i < countFd; ++i) {
		if (_pollfds[i].revents == 0)
			continue;
		// TODO: Добавить условие для сокета сервера
		if (i < 1 && _pollfds[i].events) {
			_pollfds[i].revents = 0;
			acceptNewClients();
			continue;
		}
		cout << "До " << _pollfds[i].events << endl;
		WebClient* client = reinterpret_cast<WebClient*>(_clientsRepo->findById(_pollfds[i].fd));
		if (_pollfds[i].revents & POLLIN && !receiveRequest(client)) {
			_pollfds.erase(_pollfds.begin() + i);
			break;
		}
		cout << "После " << _pollfds[i].events << endl;
		if (_pollfds[i].revents & POLLOUT && !sendResponse(client)) {
			_pollfds.erase(_pollfds.begin() + i);
			break;
		}
		_pollfds[i].revents = 0;
	}
}

void Server::acceptNewClients()
{
	int userfd;

	while (true) {
		userfd = accept(_socket->getSockfd(), NULL, NULL);
		if (userfd <= 0)
			break;
		_pollfds.push_back(fillPollfd(userfd, POLLIN));
		IEntity* client = reinterpret_cast<IEntity*>(new WebClient(userfd, ntohs(_address.sin_port), _pollfds.back().events));
		_clientsRepo->save(userfd, client);
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
		delete client->getResponse();
		delete client->getRequest();
		_clientsRepo->deleteById(client->getFd());
		return false;
	}

	//	buffer[bytesRead] = '\0';
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
			delete client->getResponse();
			delete client->getRequest();
			_clientsRepo->deleteById(client->getFd());
			return false;
		}

		client->getResponse()->toSend = buffer.substr(sendBytes);
		std::cout << "Sent " << sendBytes << " bytes to fd: " << client->getFd() << std::endl;
		if (client->getResponse()->toSend.empty())
			client->update();
		client->setStatus(POLLIN);
	}
	return true;
}
