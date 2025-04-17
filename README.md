### C++로 웹서버 구현하기
- VSCode
- C++17
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
- pthread와 std::thread.. 두 방법 중 후자를 택함(다만, g++로 컴파일 시 -pthread 인자 추가)

>>> 이유: 완전히 낮은 수준 제어를 할 필요가 없고, 일반적인 C++ 애플리케이션이기 때문...

- 흐름
```
서버 실행 -> 무한 루프에서 accept()
       ↳ 클라이언트 연결 수락 → *새 스레드로 요청 처리 함수 실행*
       ↳ 서버는 다시 accept()로 돌아가 다른 요청 기다림
```
![image](https://github.com/user-attachments/assets/ed490d06-5999-4e6b-b488-39ed8a0fa612)

## 4. 정적 파일 응답
- 여기서부터 서치가 어려워서 힘들었다. 다행히 대학 시절 파일 입출력 스트림을 다룬 파트를 참고했다.
- GPT에게 물어보니 정적 파일 내용을 문자열로 읽어와서 HTML 문자열로 반환하는 함수를 만들라고 한다..
```
ifstream
stringstream
.rdbuf()
등을 사용하여 파일 내용을 스트림에 저장하여 string으로 반환
```


---
## 참고 사이트 및 자료
- 운영체제(이형봉 저)
- 마이크로프로세서(이형봉 저)
- 아장아장 C언어(이형봉 저)
- https://recipes4dev.tistory.com/153 --> 소켓 프로그래밍 이론
- https://www.it-note.kr/122 --> setsockopt 함수
- https://gdngy.tistory.com/186 , https://engineerer.tistory.com/126 --> 소켓 구현 참고
- https://codingfarm.tistory.com/536 --> bind 함수
- https://softtone-someday.tistory.com/20 --> Big Endian, Little Endian
- https://blockdmask.tistory.com/322 --> 파일 읽기 함수(fstream 라이브러리)
