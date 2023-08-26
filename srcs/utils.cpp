#include "../include/utils.hpp"

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
