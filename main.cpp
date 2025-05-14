#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>
#include "handler.h"
#include "router.h"

#define PORT 8080

using namespace std;

string hello_html()
{
    return "<html><body><h1>Hello from /hello</h1></body></html>";
}

string time_html()
{
    time_t now = time(nullptr);
    return "<html><body><h1>현재 시간: " + string(ctime(&now)) + "</h1></body></html>";
}

int main()
{
    int server_fd;
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 라우터
    register_route("/hello", hello_html);
    register_route("/time", time_html);

    // 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // IPv4, TCP(UDP는 SOCK_DGRAM), 기본 프로토콜
    if (server_fd == -1)
    {
        perror("socket 실패(:main.cpp, line 19)");
        exit(EXIT_FAILURE);
    }

    // 주소/포트 재사용 옵션
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
    {
        perror("setsockopt 실패(:main.cpp, line 28)");
        exit(EXIT_FAILURE);
    }

    // 주소 바인딩(bind)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // 호스트 바이트 오더 -> 네트웤 바이트 오더(엔디안 변환)

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("바인딩 실패(:main.cpp, line 39)");
        exit(EXIT_FAILURE);
    }

    // 리스닝
    if (listen(server_fd, 10) < 0) // 백로그 큐(동시 접속 대기열), SOMAXCONN은 4096임(안전)
    {
        perror("바인딩 실패(:main.cpp, line 39)");
        exit(EXIT_FAILURE);
    }

    cout << "서버가 포트 " << PORT << "에서 실행 중...\n";

    // 루프 형태: 다중 요청 처리
    while (true)
    {
        // 클라이언트 연결 수락(accept)
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            perror("accept(:main.cpp line 54)");
            continue;
        }

        // 새 스레드 생성, 클라이언트 소켓 넘김
        thread(handle_client, new_socket).detach();
    }

    close(server_fd);
    return 0;
}