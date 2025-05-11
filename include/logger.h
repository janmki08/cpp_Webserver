#ifndef LOGGER_H
#define LOGGER_H

#include <string>

void log_access(const std::string &method, const std::string &path);

#endif