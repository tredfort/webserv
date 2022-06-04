#include "config/Config.hpp"
#include "server/Server.hpp"

/**
 * Parse args, if they are okay to execute server
 * @return
 */
int main(int argc, char** argv)
{
	string configPath = "config.conf";

	if (argc > 2) {
		cout << "usage: ./webserv [config file]" << endl;
		return 0;
	} else if (argc == 2)
		configPath = argv[1];

	Config* config = new Config(configPath);
	config->printConfig();
//	LocationContext* lc = config->getLocationContext("127.0.0.1", "8080", "class", "/");
//	cout << "founded Location" << endl;
//	if (lc == NULL)
//		cout << "not Location" << endl;
//	else {
//		lc->printConfig();
//		return 0;
//	}

	Server* app = new Server(config);

	app->start();
	delete app;
	return 0;
}
