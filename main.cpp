#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/socket.h>
#include <fcntl.h>

#define PORT 8080

using namespace std;

// 파일 내용 반환
string get_file(const string &path)
{
    ifstream file(path); // 파일 열기
    if (!file.is_open())
        return ""; // 열기 실패

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 쓰레드로 클라이언트 제어
void handle_client(int csocket)
{
    // 요청 받기(read)
    char buffer[30000] = {0};
    read(csocket, buffer, 30000);
    string request(buffer); // 버퍼 내용 문자열 변환

    // 요청 경로 파싱
    string path = "/";
    size_t first_space = request.find(" ");                // 없으면 npos 반환
    size_t end_space = request.find(" ", first_space + 1); // first_space 다음 위치부터 탐색

    // 경로만 추출
    if (first_space != string::npos && end_space != string::npos)
    {
        path = request.substr(first_space + 1, end_space - first_space - 1);
    }

    // 파일 경로 구성
    if (path == "/")
    {
        path = "/index.html";
    }
    string file_path = "./static" + path;
    // 파일 읽기
    string content = get_file(file_path);
    // 응답 작성 ----------- 추가 수정 필요
    string response;
    if (!content.empty())
    {
        response = "HTTP/1.1 200 OK\r\nContent-Length: " + to_string(content.size()) + "\r\n\r\n" + content;
    }
    else
    {
        response = "HTTP/1.1 404 NOT FOUND\r\nContent-Length: " + to_string(content.size()) + "\r\n\r\n" + "<h1>404 NOT FOUND</h1>";
    }

    // 응답 보내기
    send(csocket, response.c_str(), response.length(), 0);
    cout << "응답 보냄\n";

    // 연결 종료
    close(csocket);
}

int main()
{
    int server_fd;
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP(UDP는 SOCK_DGRAM), 기본 프로토콜

    // 주소/포트 재사용 옵션
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // 주소 바인딩(bind)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // 호스트 바이트 오더 -> 네트웤 바이트 오더(엔디안 변환)

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10); // 백로그 큐(동시 접속 대기열), SOMAXCONN은 4096임(안전)

    cout << "포트: " << PORT << "...\n";

    // 루프 형태: 다중 요청 처리
    while (true)
    {
        // 클라이언트 연결 수락(accept)
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            perror("accept");
            continue;
        }

        // 새 스레드 생성, 클라이언트 소켓 넘김
        thread client_thread(handle_client, new_socket);
        client_thread.detach();
    }

    // 도달은 안 함.
    close(server_fd);

    return 0;
}