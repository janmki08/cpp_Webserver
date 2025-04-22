### C++로 웹서버 구현하기
- VSCode
- C++17
- WSL Ubuntu-20.04

```
빌드 및 실행
cd cpp_webserver
make
./server
```
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
- 경로 추출 후 html 파일 내용을 읽어와서 나타냄(오류 발생으로 수정 중)
> 해결: 경로 추출 시 -1을 안해서 공백이 함께 입력됨... 기초적인 실수인데 안보여서 한참 찾음..

![image](https://github.com/user-attachments/assets/7a795dbf-d7c2-40d0-84a7-309d27ded742)
![image](https://github.com/user-attachments/assets/6f0849e7-ee6c-4ea2-8abb-88474b1da3ff)

- 이미지와 같이 깨진 폰트로 나옴(아마 UTF-8 때문인 것을 추측)
> 해결: <meta charset= "UTF-8"> 추가

![image](https://github.com/user-attachments/assets/fca7b160-8d68-449b-a409-a17b82da5f1e)
![image](https://github.com/user-attachments/assets/73d7dd1b-b810-48b8-abf7-599b37435b4c)

### 요청의 쿼리 분리, 파싱
1. "?"문자 뒤의 쿼리(키-값)를 분리
2. map함수로 키-값 쌍을 매핑

![image](https://github.com/user-attachments/assets/29056998-64bb-4b52-9ad4-195132e62694)
![image](https://github.com/user-attachments/assets/c9d112b0-a6b6-47ef-b995-3f053994d67d)

> 경고 처리 및 Makefile로 관리

### POST 요청 처리
![image](https://github.com/user-attachments/assets/0592adb2-99e9-4794-b99b-8cf3b89a6d86)
- 헤더 끝 부분 뒤에 "\r\n\r\n"로 body 부분과 분리
- Content-Length만큼 body를 읽음
![image](https://github.com/user-attachments/assets/9e6711e0-ed25-41b5-9542-4f834e842576)

- 박상훈과 나이를 전송하니 보내진 것을 확인
- 다만, 아래처럼 데이터가 인코딩 되어 보인다. 디코딩도 해봐야한다.
![image](https://github.com/user-attachments/assets/e079d111-e3fc-451b-bb5b-832aa9599631)


## TODO
1. ~~현재 요청(request)를 공백 문자로만 구분해서 파싱함 -> 쿼리도 있고 잘못된 요청이 왔을 때 구분 못하기 때문~~
2. POST 요청 처리 추가
3. MIME 타입 응답 수정 -> 현재는 단순해서 부정확한 정보를 담고 있고 확장자를 매핑해서 기능 확장 필요
4. 보안 -> static 이외 폴더 접근 막기
 
---
## 참고 사이트 및 자료
- 운영체제(이형봉 저)
- 시스템 프로그래밍(이형봉 저)
- 아장아장 C언어(이형봉 저)
- https://recipes4dev.tistory.com/153 --> 소켓 프로그래밍 이론
- https://www.it-note.kr/122 --> setsockopt 함수
- https://gdngy.tistory.com/186 , https://engineerer.tistory.com/126 --> 소켓 구현 참고
- https://codingfarm.tistory.com/536 --> bind 함수
- https://softtone-someday.tistory.com/20 --> Big Endian, Little Endian
- https://blockdmask.tistory.com/322 --> 파일 읽기 함수(fstream 라이브러리)
