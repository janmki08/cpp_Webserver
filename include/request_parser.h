#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <string>
#include <map>

using namespace std;

void request_line_parsing(const string &request, string &method, string &path, string &query, string &version);
void query_parsing(const string &query, map<string, string> &mapset);
void body_parsing(const string &body, map<string, string> &mapset);
string post_body(const string &request);

#endif // REQUEST_PARSER_H