#include "logger.h"
#include <fstream>
#include <ctime>

void log_access(const std::string &method, const std::string &path)
{
    std::ofstream log("access.log", std::ios::app);
    if (!log.is_open())
        return;

    time_t now = time(nullptr);
    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&now));

    log << "[" << timebuf << "] " << method << " " << path << std::endl;
}