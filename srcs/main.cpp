#include "Server/Server.hpp"
#include "Config/Configurator.hpp"

/**
 * Parse args, if they are okay to execute server
 * @return
 */
int main(int argc, char** argv)
{
    Configurator configurator;
    Config* config;
	string hint = "./webserv config.conf\n";

	if (argc > 2) {
		cout << hint;
		return 0;
	} else if (argc == 2)
        config = configurator.createConfig(argv[1]);
	else
        config = configurator.createConfig("config.conf");

	Server* app(config);

	app->start();
	delete app;
	return (0);
}
