#include "config/Config.hpp"
#include "server/Server.hpp"

/**
 * Parse args, if they are okay to execute server
 * @return
 */
int main(int argc, char** argv, char **env)
{
	string configPath = "config.conf";

	if (argc > 2) {
		cout << "usage: ./webserv [config file]" << endl;
		return 0;
	} else if (argc == 2)
		configPath = argv[1];

	Config config(configPath);
	config.printConfig();
	Env enviroment(env);
	Server* app = new Server(nullptr, enviroment);

	app->start();
	delete app;
	return 0;
}
