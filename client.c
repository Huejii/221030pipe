#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    /* fifo descriptor */
    int fd;
    int fd2;

    /* 서버와 IPC할 Read, write할 FIFO 경로 2개 초기화 */
    char * fifo1 = "/tmp/fifo1";
    char * fifo2 = "/tmp/fifo2";

    /* IPC 과정 반복 실행 */
    while (1)
    {
        /* FIFO open. client는 fifo1에서 write only, fifo2에서 read only */
        fd = open(fifo1, O_WRONLY);
        fd2 = open(fifo2, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "Pipe Failed");
            return 1;
        }
        char getFileString[512]="";
        char temp[512];

        /* 파일명 입력 받기 */
        printf("Filename:");
        fgets(filename, 512, stdin);
        filename[strlen(filename) - 1] = '\0'; // 개행문자 제거
        write(fd, filename, strlen(filename)+1); // server에 보낼 파일명 write
        
        /* 파일명이 없을 시 while문 break */
        if(strcmp(filename, "")==0)
        {
            printf("filename 입력 오류\n");
            break;
        }

        /*read write 여부 입력 받기*/
        printf("R/W:");
        fgets(rwType, 512, stdin);
        rwType[strlen(rwType) - 1] = '\0'; // 개행문자 제거
        write(fd, rwType, strlen(rwType)+1); // server에 보낼 rw type write

        if (strcmp(rwType,"R")==0)
        {
            /* 파일 엑세스 타입이 Read일 때 */
            printf("Byte Size:");
            fgets(readByteSize, 512, stdin); // 사용자로부터 읽을 데이터 바이트 수 입력 받기
            write(fd, readByteSize, strlen(readByteSize)+1); // 읽을 데이터 바이트 수 write
            read(fd2, getFileString, strlen(getFileString)+1); // 서버가 write한 데이터 string read
            printf("Success get String: %s", getFileString); // getFileString 출력
            read(fd2, temp, sizeof(temp));
            printf("%s\n", temp); 
        } 
        else if (strcmp(rwType,"W")==0)
        {
            /* 파일 엑세스 타입이 Write일 때 */
            printf("Write String:");
            fgets(writeString, 512, stdin); // 사용자로부터 작성할 데이터 string 입력 받기
            write(fd, writeString, strlen(writeString)+1); // 서버로 보낼 피일에 쓸 데이터 string write
            read(fd2, writeByte,  strlen(writeByte)+1); // 파일에 쓴 데이터 byte수 read
            printf("Success Write Byte Size: %s", writeByte); // 파일에 쓴 데이터 byte수 출력

            read(fd2, temp, sizeof(temp)); //
            printf("%s\n", temp); //
        }else
        {
            /* R/W 입력 오류 처리 */
            printf("RWtype 오류");
            exit(1);
        }

        /*FIFO close*/
        close(fd);
        close(fd2);
    }