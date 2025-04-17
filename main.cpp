#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>     // POSIX 시스템 함수
#include <netinet/in.h> // sockaddr_in, IPPROTO_TCP 등 네트워크 구조체
#include <thread>
#include <fstream>
#include <sstream>

#define PORT 8080

using namespace std;

// 파일 내용 가져오기
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
    int valread = read(csocket, buffer, 30000);
    cout << "받은 요청: " << buffer << "\n";

    // TODO
    // 요청 경로 파싱
    // 파일 경로 구성
    // 파일 읽기

    // 응답 작성 ========== 수정 필요
    string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, World!";

    // 응답 보내기
    // write -> send로 수정, 소켓스럽게~~
    send(csocket, response.c_str(), response.length(), 0);
    cout << "응답 보냄\n";

    // 연결 종료
    close(csocket);
}

int main()
{
    int server_fd;                 // 연결 기다리는 소켓(listen), 파일 디스크립터
    int new_socket;                // 연결 시 생성 소켓(accept)
    struct sockaddr_in address;    // 바인딩될 주소 정보 구조체
    int addrlen = sizeof(address); // 구조체 크기(accept 함수에 필요)

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
    listen(server_fd, 10); // 백로그 큐(동시 접속 대기열) 10개, SOMAXCONN은 4096임.

    cout << "다중 스레드 서버 포트: " << PORT << "...\n";

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
        // detach(): 백그라운드 독립 실행, 대기X, 알아서 끝나게 둠
        // join(): 메인 스레드가 해당 스레드가 끝날 때까지 기다림, 결과O, 정확한 순서 필요
    }

    // 도달은 안 함.
    close(server_fd);

    return 0;
}