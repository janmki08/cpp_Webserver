#include "request_parser.h"
#include "decode.h"

#include <sstream>

using namespace std;

// 요청 라인 파싱
void request_line_parsing(const string &request, string &method, string &path, string &query, string &version)
{
    // request 라인 파싱(메서드, URI, version)
    istringstream line_stream(request);
    string uri;
    line_stream >> method >> uri >> version;

    size_t query_pos = uri.find('?');
    if (query_pos != string::npos)
    {
        path = uri.substr(0, query_pos);
        query = uri.substr(query_pos + 1);
    }
    else
    {
        path = uri;
        query = "";
    }
}

// 쿼리 파싱
void query_parsing(const string &query, map<string, string> &mapset)
{
    istringstream query_stream(query);
    string pair;

    while (getline(query_stream, pair, '&'))
    {
        size_t equal_pos = pair.find('=');
        if (equal_pos != string::npos)
        {
            string key = decode(pair.substr(0, equal_pos));
            string value = decode(pair.substr(equal_pos + 1));
            mapset[key] = value;
        }
    }
}

// POST 요청 body 분리
string post_body(const string &request)
{
    size_t header_end = request.find("\r\n\r\n");
    if (header_end != string::npos)
    {
        return request.substr(header_end + 4);
    }
    return "";
}

// body 파싱(쿼리와 동일)
void body_parsing(const string &body, map<string, string> &mapset)
{
    query_parsing(body, mapset);
}