#include "handler.h"
#include "request_parser.h"
#include "response.h"
#include "getfile.h"
#include "decode.h"
#include "mime.h"
#include "logger.h"
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <unistd.h>
#include <sys/socket.h>
#include <limits.h>

using namespace std;

void handle_client(int csocket)
{
    // 요청 받기(read)
    char buffer[30000] = {0};
    read(csocket, buffer, 30000);
    string request(buffer); // 버퍼 내용 문자열 변환
    string response;

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

    // 기본 경로 처리
    if (path == "/")
        path = "/index.html";

    // ../ 경로 차단
    if (path.find("..") != string::npos)
    {
        response = NOT_FOUND_response();
        send(csocket, response.c_str(), response.length(), 0);
        close(csocket);
        return;
    }

    // 파일 경로 생성 및 경로 검사(static 외 차단)
    string file_path = "./static" + path;
    char resolved_path[PATH_MAX];
    if (realpath(file_path.c_str(), resolved_path) == nullptr)
    {
        response = NOT_FOUND_response();
        send(csocket, response.c_str(), response.length(), 0);
        close(csocket);
        return;
    }
    string real_path_str(resolved_path);
    if (real_path_str.find("/static/") == std::string::npos)
    {
        response = NOT_FOUND_response();
        send(csocket, response.c_str(), response.length(), 0);
        close(csocket);
        return;
    }

    // 내용 가져오기
    string content = get_file(file_path);

    // POST 응답 테스트 용
    if (path == "/post.html" && method == "POST")
    {
        content = post_html(query_parse);
    }

    // 응답
    if (!content.empty())
    {
        string mime = get_mime(file_path);
        response = html_response(content, mime);
    }
    else
    {
        response = NOT_FOUND_response();
    }

    // 로그 출력
    log_access(method, path);

    // 응답 전송
    send(csocket, response.c_str(), response.length(), 0);
    close(csocket);
}