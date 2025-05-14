#include "router.h"

static std::map<std::string, std::function<std::string()>> route_table;

void register_route(const std::string &path, const std::function<std::string()> &handler)
{
    route_table[path] = handler;
}

bool has_route(const std::string &path)
{
    return route_table.find(path) != route_table.end();
}

std::string handle_route(const std::string &path)
{
    return route_table[path](); // 이미 존재한다고 가정
}
