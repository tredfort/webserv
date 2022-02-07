#include "server/Server.hpp"
#include "config/Configurator.hpp"

/**
 * Parse args, if they are okay to execute server
 * @return
 */
int main(int argc, char **argv) {
    Configurator configurator;
    string configPath = "config.conf";;

    if (argc > 2) {
        cout << "./webserv config.conf" << endl;
        return 0;
    } else if (argc == 2)
        configPath = argv[1];

    Config *config = configurator.createConfig(configPath);
    Server *app = new Server(config);

    app->start();
    delete app;
    return 0;
}
