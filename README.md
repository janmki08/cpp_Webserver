### C++로 웹서버 구현하기
- VSCode
- WSL Ubuntu-20.04
---
## 1. 일회용 웹브라우저 띄우기
![image](https://github.com/user-attachments/assets/4aac2eaf-a39e-4871-bc91-84aed8bde3ae)
![image](https://github.com/user-attachments/assets/86528396-70b7-4da1-8f0c-94c0802af4dc)

## 2. 다중 요청 처리 + 루프
- 흐름
```
while(1) {
  accept;
  read;
  write;
  close;
}
```
- 단순히 accept부터 close(new_socket)까지 루프문으로 구현

## 3. 멀티스레딩
- 현 시점 문제점: 루프문에서 클라이언트의 요청을 처리하고 대기하는 도중에 다른 클라이언트가 요청하면 서버가 처리할 수 없음
- 해결법: 클라이언트 하나 당 스레드 하나 할당해서 서버를 다시 accept할 수 있게 함
- pthread와 std::thread.. 두 방법 중 후자를 택함

>>> 이유: 완전히 낮은 수준 제어를 할 필요가 없고, 일반적인 C++ 애플리케이션이기 때문...

- 흐름
```
서버 실행 -> 무한 루프에서 accept()
       ↳ 클라이언트 연결 수락 → *새 스레드로 요청 처리 함수 실행*
       ↳ 서버는 다시 accept()로 돌아가 다른 요청 기다림
```
---
## 참고 사이트 및 자료
- 운영체제(이형봉 저)
- https://recipes4dev.tistory.com/153 --> 소켓 프로그래밍 이론
- https://www.it-note.kr/122 --> setsockopt 함수
- https://gdngy.tistory.com/186 , https://engineerer.tistory.com/126 --> 소켓 구현 참고
- https://codingfarm.tistory.com/536 --> bind 함수
- https://softtone-someday.tistory.com/20 --> Big Endian, Little Endian
