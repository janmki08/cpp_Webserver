# 목차
1. 웹브라우저 띄우기
2. 다중 요청 처리 + 루프(다회용)
3. 멀티스레딩
4. 정적 파일 응답
    ↳ 4-1. 쿼리 분리 및 POST 처리
### C++로 웹서버 구현하기
- VSCode
- C++17
- WSL Ubuntu-20.04

```
빌드 및 실행
cd cpp_webserver
make
./build/server
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
> 해결: 경로 추출 시 -1을 안해서 공백이 함께 입력됨... 실수인데 안보여서 한참 찾음..

![image](https://github.com/user-attachments/assets/7a795dbf-d7c2-40d0-84a7-309d27ded742)
![image](https://github.com/user-attachments/assets/6f0849e7-ee6c-4ea2-8abb-88474b1da3ff)

- 이미지와 같이 깨진 폰트로 나옴(아마 UTF-8 때문인 것을 추측)
> 해결: <meta charset= "UTF-8"> 추가
> 응답 함수(response.cpp)에서 응답시 UTF-8로 가게 함.

![image](https://github.com/user-attachments/assets/fca7b160-8d68-449b-a409-a17b82da5f1e)
![image](https://github.com/user-attachments/assets/73d7dd1b-b810-48b8-abf7-599b37435b4c)

#### 요청의 쿼리 분리, 파싱
1. "?"문자 뒤의 쿼리(키-값)를 분리
2. map함수로 키-값 쌍을 매핑

![image](https://github.com/user-attachments/assets/29056998-64bb-4b52-9ad4-195132e62694)
![image](https://github.com/user-attachments/assets/c9d112b0-a6b6-47ef-b995-3f053994d67d)

> 경고 처리 및 Makefile로 관리

#### POST 요청 처리
![image](https://github.com/user-attachments/assets/0592adb2-99e9-4794-b99b-8cf3b89a6d86)
- 헤더 끝 부분 뒤에 "\r\n\r\n"로 body 부분과 분리
- Content-Length만큼 body를 읽음

![image](https://github.com/user-attachments/assets/9e6711e0-ed25-41b5-9542-4f834e842576)

- 박상훈과 나이를 전송하니 보내진 것을 확인
- 다만, 아래처럼 데이터가 인코딩 되어 보인다. 디코딩도 해봐야한다.

![image](https://github.com/user-attachments/assets/e079d111-e3fc-451b-bb5b-832aa9599631)

---
#### URL 디코딩
```
박: %EB%B0%95
상: %EC%83%81
훈: %ED%9B%88
```
1. 추가적인 테스트 결과 영어는 그대로 나오고 공백 문자는 "+"로 나옴 -> + 가 나오면 공백으로 바꾸면 됨
2. %16진수 문자로 이루어져있음 -> 16진수를 10진수로 바꿔서 char로 바꾸면 될 듯

![image](https://github.com/user-attachments/assets/6cfbb9cf-4f60-49af-91dc-29fc51c9fc42)
> 추가로 필요한 사항이 있으면 진행하며 추가

## 5. MIME 타입 자동 처리
확장자 별로 브라우저가 처리 할 수 있게 함.
![image](https://github.com/user-attachments/assets/2df2f151-3e03-4807-8892-fc63e334cc0a)

## 6. 디렉토리 트레버설(Directory Traversal) 대응
- 공격 원리: GET, POST 요청 방법을 통해 브라우저에서 입력 값을 받음 -> 파라미터 요청 값을 상대(절대) 경로를 변조함 -> 원하는 파일 접근
- 대응 방안
    1. 디렉토리 목록 비활성화✅
    2. 기본 페이지(index.html) 사용✅
    3. 액세스 제어 메커니즘✅
    4. WAF(웹 애플리케이션 방화벽) --- 외부 프로그램
    5. 정기적 업데이트로 최신 상태 유지 --- 외부 프로그램
    6. 구성 및 접근 로그를 정기적 모니터링✅
1번: static외 경로 차단
3번: 특정 경로 접근 제한
6번: 로그 함수 구현

테스트 결과

![image](https://github.com/user-attachments/assets/5024b0e4-73b1-4307-b693-41f4344b1727)

![image](https://github.com/user-attachments/assets/1263fd2f-5a7f-4d21-bb79-44ad9a9d0b2a)

static 외 접근은 로그도 남지 않음

브라우저 자체에서 ../ 을 허용하지 않기에 다른 방법으로 시도

curl 명령어로 시도하자 404 에러(정상 작동)

## 7. 라우팅 시스템 구현
- 구현 이유: if문 분기로 인해 다수의 페이지가 생성되면 if문이 너무 많아짐
- 방법
    1. 요청 파싱 → method, path 추출
    2. path가 "/hello"처럼 등록된 라우트인지 확인
    3. 있으면 route_table에서 함수 실행
    4. 없으면 static 파일 경로로 처리

 - 문제: /hello로 접근해도 없는 페이지로 뜸...
> static 페이지로 넘어가는 것이였음 -> 조건문으로 동적인지 정적인지 구분하여 접속

> mime 타입 설정 때문에 application/octet-stream로 인식하여 파일을 다운로드 해버림 -> text/html로 처리하게 강제함(임시)

![image](https://github.com/user-attachments/assets/2b45c613-9c9d-4770-b5cd-ca7e3f333e5d)
예시로 현재시각을 띄워봄

## TODO
1. ~~현재 요청(request)를 공백 문자로만 구분해서 파싱함 -> 쿼리도 있고 잘못된 요청이 왔을 때 구분 못하기 때문~~
2. ~~POST 요청 처리 추가~~
3. ~~MIME 타입 응답 수정 -> 현재는 단순해서 부정확한 정보를 담고 있고 확장자를 매핑해서 기능 확장 필요~~
4. ~~보안(디렉토리 트레버설) -> static 이외 폴더 접근 막기~~
5. 클라이언트 IP까지 출력하는 것 고려..
6. 라우터 시스템 확장 및 JSON 응답
7. 웹소켓 구현
 
---
## 참고 사이트 및 자료
- 운영체제(이형봉 저)
- 시스템 프로그래밍(이형봉 저)
- 아장아장 C언어(이형봉 저)
- https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/MIME_types --> MIME 타입
- https://recipes4dev.tistory.com/153 --> 소켓 프로그래밍 이론
- https://www.it-note.kr/122 --> setsockopt 함수
- https://gdngy.tistory.com/186 , https://engineerer.tistory.com/126 --> 소켓 구현 참고
- https://codingfarm.tistory.com/536 --> bind 함수
- https://softtone-someday.tistory.com/20 --> Big Endian, Little Endian
- https://blockdmask.tistory.com/322 --> 파일 읽기 함수(fstream 라이브러리)
