#include "response.h"

// 정상 응답
string html_response(const string &body)
{
    return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + to_string(body.size()) + "\r\n\r\n" + body;
}

// 404 응답
string NOT_FOUND_response()
{
    return "HTTP/1.1 404 NOT FOUND\r\nContent-Length: 0\r\n\r\n<h1>404 NOT FOUND</h1>";
}

// POST 요청 시 응답
string post_html(const map<string, string> &mapset)
{
    string html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>POST 처리</title></head><body>";
    html += "<h1>POST 데이터</h1>";
    for (const auto &pair : mapset)
    {
        html += "<p>" + pair.first + ": " + pair.second + "</p>";
    }
    html += "</body></html>";
    return html;
}