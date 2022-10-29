#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    printf("Hello Client");
    int fd;

    // FIFO path
    char * fifo1 = "/tmp/fifo1";
    char * fifo2 = "/tmp/fifo2";

    char temp[512];
    while (1)
    {

        

        // FIFO 쓰기 전용으로 열기 (서버로 보내기)
        fd = open(fifo1, O_WRONLY);

        /*
         TODO 유저에게 묻기로 수정 시작
            1. 파일명 묻기
            2. R || W 여부 묻기
            3. R일 경우 읽을 바이트 수 묻기 (int 형식으로 받는다)
            4. W일 경우 작성할 스트링 받기 (char* 형식으로 받는다)
        */

        temp = "";

        printf("Filename:");
        fgets(temp, 512, stdin);
        write(fd, temp, strlen(temp)+1);
       
        temp = "";

        printf("R/W:");
        fgets(temp, 512, stdin);
        write(fd, temp, strlen(temp)+1);
   
        temp = "";

        printf("Byte size:");
        fgets(temp, 512, stdin);
        write(fd, temp, strlen(temp)+1);
     
        temp = "";
  
//쓰기전용으로 열었던 FIFO 닫기
        close(fd);



        // Open FIFO for Read only
        fd2 = open(fifo2, O_RDONLY);

        // Read from FIFO
        read(fd2, temp, sizeof(temp));

        // Print the read message
        printf("Server sent: %s\n", temp);
        close(fd2);
    }
    return 0;
}