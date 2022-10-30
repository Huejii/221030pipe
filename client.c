#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int fd;
    int fd2;

    // FIFO path
    char * fifo1 = "/tmp/fifo1";
    char * fifo2 = "/tmp/fifo2";

    while (1)
    {
        // FIFO 쓰기 전용으로 열기 (서버로 보내기)
        // if(fd = open(fifo1, O_WRONLY) < 0)
        // {
        //     perror("open error : ");
        //     exit(0);
        // }
        // if(fd2 = open(fifo2, O_RDONLY) < 0)
        // {
        //     perror("open error : ");
        //     exit(0);
        // }
        fd = open(fifo1, O_WRONLY);
        fd2 = open(fifo2, O_RDONLY);
        /*
         TODO 유저에게 묻기로 수정 시작
            1. 파일명 묻기
            2. R || W 여부 묻기
            3. R일 경우 읽을 바이트 수 묻기 (int 형식으로 받는다)
            4. W일 경우 작성할 스트링 받기 (char* 형식으로 받는다)
        */

        char filename[512] = "";
        char rwType[512] = "";
        char writeString[512] = "";
        char readByteSize[512] = "";
        char writeByte[512]= "";
        char getFileString[512]="";

        printf("Filename:");
        fgets(filename, 512, stdin);
        filename[strlen(filename) - 1] = '\0';
        write(fd, filename, strlen(filename)+1);

        printf("R/W:");
        fgets(rwType, 512, stdin);
        rwType[strlen(rwType) - 1] = '\0';
        write(fd, rwType, strlen(rwType)+1);

        if (strcmp(rwType,"R"))
        {
            printf("Byte Size:");
            fgets(readByteSize, 512, stdin);
            write(fd, readByteSize, strlen(readByteSize)+1);
            
            read(fd2, getFileString,  strlen(getFileString)+1);
            printf("Success get String: %s\n", getFileString);
        } else if (strcmp(rwType,"W"))
        {
            printf("Write String:");
            fgets(writeString, 512, stdin);
            write(fd, writeString, strlen(writeString)+1);
            // 아래 추가: read 쓴 데이터 string길이 받기
            read(fd2, writeByte,  strlen(writeByte)+1);
            printf("Write Success Byte Size: %s\n", writeByte);
        
        }else
        {
            printf("오류");
        }
        char temp[512];
        read(fd2, temp, sizeof(temp));

        // Print the read message
        printf("Server sent: %s\n", temp);
        close(fd);
        close(fd2);
    }
    return 0;
}