#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <map>
#include <functional>

void register_route(const std::string &path, const std::function<std::string()> &handler);
bool has_route(const std::string &path);
std::string handle_route(const std::string &path);

#endif