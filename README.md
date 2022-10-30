# Assignment1: Named Pipe

1. 실행환경: 
- oracle VM virtual Box ubuntu에서 실행하였다.
- server와 client를 서로 다른 터미널에서 실행하였다.

2. 실행 명령어

아래의 순서로 실행한다.

(1) <server.c를 동작시킬 터미널>
gcc -o server server.c
./server

(2) <client.c를 동작시킬 터미널>
gcc -o client client.c
./client

![image](https://user-images.githubusercontent.com/94972402/198872839-916ec56d-8e29-4d58-95af-bef2af8a3d1c.png)
![image](https://user-images.githubusercontent.com/94972402/198872849-b54787b0-9fdc-45c9-89c5-406553c4dffe.png)

3. user 입력(client 터미널)
- Filename:
텍스트파일 확장자를 붙여 파일이름을 입력하면된다.
ex)A.txt
입력하지 않고 엔터를 누를 경우 종료된다.

- R/W
file을 read 하려면 R, write를 하려면 W를 입력한다.
입력하지 않고 엔터를 누를 경우 종료된다.

W로 동작했을 시
- Write String:
문자열을 입력한다. 스페이스를 사용하여도 된다.

R로 동작했을 시
- Byte Size:
읽어들일 파일의 데이터를 몇바이트 read할 것인지 숫자를 입력한다.(512 미만)
