#include "client.hpp"
#include "server.hpp"
#include "utils.h"


enum class Mode {
    null,
    client,
    server,
};

Mode parseMode(std::string const& s) {
    if (s == "client")
        return Mode::client;
    if (s == "server")
        return Mode::server;
    return Mode::null;
}

void runClient() {
    Client client;
    client.connectByInput();
    client.run();
    return;
}

void runServer() {
    Server server;
    server.setPort(8080);
    server.run();
    return;
}

int main(int argc, char **argv) {
    if (argc == 2) {
        Mode mode = parseMode(argv[1]);
        if (mode == Mode::client) {
            runClient();
        } else if (mode == Mode::server) {
            runServer();
        } else {
            std::cout << "Not a valid param." << std::endl;
            std::cout << "Example: \"./main server\" or \"./main client\"" << std::endl;
        }
    } else {
        std::cout << "Not a valid param." << std::endl;
        std::cout << "Example: \"./main server\" or \"./main client\"" << std::endl;
    }
    return 0;
};