#include "../include/utils.hpp"
void print_timestamped(void)
{
    std::time_t currentTime = std::time(NULL);

    struct std::tm* timeinfo = std::localtime(&currentTime);

    char timeString[80];
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeinfo);

    std::cout << "["  << timeString << "] ";
}
