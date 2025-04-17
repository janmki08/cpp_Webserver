#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>     // POSIX 시스템 함수
#include <netinet/in.h> // sockaddr_in, IPPROTO_TCP 등 네트워크 구조체

#define PORT 8080

using namespace std;

int main()
{
    int server_fd;  // 연결 기다리는 소켓(listen), 파일 디스크립터
    int new_socket; // 연결 시 생성 소켓(accept)
    long valread;
    struct sockaddr_in address;    // 바인딩될 주소 정보 구조체
    int addrlen = sizeof(address); // 구조체 크기(accept 함수에 필요)

    // 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("소켓 연결 실패");
        exit(EXIT_FAILURE);
    }

    // 주소/포트 재사용 옵션
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // 주소 바인딩(bind)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("바인드 실패");
        exit(EXIT_FAILURE);
    }

    // 요청 대기(listen)
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "리스닝 포트: " << PORT << "...\n";

    // 클라이언트 연결 수락(accept)
    // 실질적 데이터 송수신 담당
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // 요청 받기(read)
    char buffer[30000] = {0};
    valread = read(new_socket, buffer, 30000);
    cout << "받은 요청: " << buffer << "\n";

    // 응답 작성
    string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, World!";

    // 응답 보내기
    write(new_socket, response.c_str(), response.length());
    cout << "응답 보냄\n";

    // 연결 종료
    close(new_socket);
    close(server_fd);

    return 0;
}