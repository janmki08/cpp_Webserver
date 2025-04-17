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
## 참고 사이트 및 자료
- 운영체제(이형봉 저)
- https://recipes4dev.tistory.com/153 --> 소켓 프로그래밍 이론
- https://www.it-note.kr/122 --> setsockopt 함수
- https://gdngy.tistory.com/186 , https://engineerer.tistory.com/126 --> 소켓 구현 참고
- https://codingfarm.tistory.com/536 --> bind 함수
- https://softtone-someday.tistory.com/20 --> Big Endian, Little Endian
