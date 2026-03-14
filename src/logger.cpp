#include "logger.h"
#include <fstream>
#include <ctime>

void logCommand(const std::string &cmd)
{
    std::ofstream file("server.log", std::ios::app);

    time_t now = time(0);
    file << "[" << ctime(&now) << "] " << cmd << "\n";
}