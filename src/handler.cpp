#include "handler.h"
#include "request_parser.h"
#include "response.h"
#include "getfile.h"
#include "decode.h"
#include "mime.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;

void handle_client(int csocket)
{
    // 요청 받기(read)
    char buffer[30000] = {0};
    read(csocket, buffer, 30000);
    string request(buffer); // 버퍼 내용 문자열 변환

    // 요청 정보 파싱
    string method, path, query, version, body;
    map<string, string> query_parse;

    request_line_parsing(request, method, path, query, version);
    query_parsing(query, query_parse);

    // POST 요청이면 body 파싱
    if (method == "POST")
    {
        body = post_body(request);
        body_parsing(body, query_parse); // 디코딩 포함
    }

    // 경로 처리
    if (path == "/")
        path = "/index.html";
    string file_path = "./static" + path;

    // 내용 가져오기
    string content = get_file(file_path);

    // POST 응답 테스트 용
    if (path == "/post.html" && method == "POST")
    {
        content = post_html(query_parse);
    }

    // 응답
    string response;
    if (!content.empty())
    {
        string mime = get_mime(file_path);
        response = html_response(content, mime);
    }
    else
    {
        response = NOT_FOUND_response();
    }

    // 응답 전송
    send(csocket, response.c_str(), response.length(), 0);
    close(csocket);
}