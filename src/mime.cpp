#include "mime.h"
#include <string>

using namespace std;

string get_mime(const string &path)
{
    size_t d = path.find_last_of(".");
    if (d == string::npos)
        return "application/octet-stream"; // 확장자 X

    string ext = path.substr(d + 1);
    if (ext == "html")
        return "text/html";
    if (ext == "htm")
        return "text/html";
    if (ext == "css")
        return "text/css";
    if (ext == "js")
        return "application/javascript";
    if (ext == "json")
        return "application/json";
    if (ext == "png")
        return "image/png";
    if (ext == "jpg" || ext == "jpeg")
        return "image/jpeg";
    if (ext == "gif")
        return "image/gif";
    if (ext == "ico")
        return "image/x-icon";
    if (ext == "svg")
        return "image/svg+xml";
    if (ext == "txt")
        return "text/plain";
    return "application/octet-stream"; // 기본값
}