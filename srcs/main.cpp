#include <iostream>
#include <cstdlib>
#include "../include/Server.hpp"

static bool isNumber(char *str)
{
    int index;

    index = -1;
    while (str[++index])
        if (!(str[index] >= '0' && str[index] <= '9'))
            return (false);
    if (!index)
        return (false);
    return (true);
}

int main(int argc, char **argv)
{
    int port;

    if (argc != 3)
        return (std::cerr << "[Error]: Invalid usage!\n"
            << "Proper usage: (./ircserv <PORT> <PASSWORD>)"
            << std::endl,
            EXIT_FAILURE);
    if (!isNumber(argv[1]))
        return (std::cerr << "[Error]: Port must be a number!\n"
                << "Proper usage: (./ircserv <PORT: Positive number> <PASSWORD: String>)"
                << std::endl,
                EXIT_FAILURE);
    port = atoi(argv[1]);
    if (port < 0)
        return (std::cerr << "[Error]: Invalid port!\n"
            << "Proper usage: (./ircserv <PORT: Positive number> <PASSWORD: String>)"
            << std::endl,
            EXIT_FAILURE);
    
    std::cout << "Port: "
        << port
        << "\nPassword: "
        << argv[2]
        << std::endl;

    Server	server(static_cast<size_t>(port), std::string(argv[2]));
    server.startServer();
    return (0);
}
