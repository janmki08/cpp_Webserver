#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <map>

using namespace std;

string html_response(const string &body, const string &content_type);
string NOT_FOUND_response();
string post_html(const map<string, string> &mapset);

#endif // RESPONSE_H