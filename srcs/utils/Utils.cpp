#include "../include/utils.hpp"
#include <map>

void print_timestamped(void)
{
    std::time_t currentTime = std::time(NULL);

    struct std::tm* timeinfo = std::localtime(&currentTime);

    char timeString[80];
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeinfo);

    std::cout << "["  << timeString << "] ";
}

std::string &obtain_ipaddress(sockaddr_in user_addr)
{
    std::string *ret_str =  new std::string();

    ret_str->assign(inet_ntoa(user_addr.sin_addr));

    return (*ret_str);
}

int findCarriageReturn(std::string&  line)
{
    int index = -1;
    
    while (line[++index] && static_cast<size_t>(index) < line.size())
    {
        if (line[index] == '\r' && line[index + 1] == '\n')
            return (index + 1);
    }
    return (-1);
}

bool isKnownCommand(std::string command)
{
    if (command == "PASS"
        || command == "NICK"
        || command == "USER"
        || command == "PING"
        || command == "JOIN"
        || command == "INVITE"
        || command == "PART"
        || command == "NOTICE"
        || command == "PRIVMSG"
        || command == "QUIT"
        || command == "KICK"
        || command == "NAMES"
        || command == "LIST"
        || command == "TOPIC"
        || command == "MODE"
        || command == "BOT")
        return (true);
    return (false);
}
