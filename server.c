#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
    /* fifo descriptor */
    int fd;
    int fd2;
    /* client와 IPC할 Read, write할 FIFO 경로 2개 초기화 */
    char * fifo1 = "/tmp/fifo1";
    char * fifo2 = "/tmp/fifo2";
    pid_t pid;

    /* FIFO 경로에 FIFO 생성 (성공시 0 반환, 실패시 -1 반환) */
    if (mkfifo(fifo1, 0666) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (mkfifo(fifo2, 0666) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);


    /* IPC 과정 반복 실행 */
    while (1)
    {
        /* FIFO open. server는 fifo1에서 read only, fifo2에서 write only */
        fd = open(fifo1,O_RDONLY);
        fd2 = open(fifo2,O_WRONLY);

        /* 성공시 0 반환, 실패시 -1 반환 */
        if (fd == -1) {
            fprintf(stderr, "Pipe Failed");
            return 1;
        }
        if (fd2 == -1) {
            fprintf(stderr, "Pipe Failed");
            return 1;
        }



        /* 생성할 파일의 pointer 선언 */
        FILE *fp;

        /* read, write로 보낼 문자열 선언 */
        char filename[512] = "";      // 파일명
        char rwType[512] = "";        // Read/Write 타입
        char writeString[512] = "";   // 사용자가 write할 데이터 string
        char writeByte[512]="";       // 사용자가 Write힌 데이터 byte 수 
        char readByteSize[512] = "";  // 사용자가 read 요청한 데이터 byte 수
        char getFileString[512]="";   // 사용자가 read 요청한 데이터 string
        char temp[512] = "";



        /* client가 사용자로부터 입력받은 파일명 read */
        read(fd, filename, 512);
        /* 파일명이 없을 시 while문 break */
        if(strcmp(filename, "")==0)
        {
            break;
        }
        printf("read file name: %s\n", filename); // read한 파일명 출력

        /* client가 사용자로부터 입력받은 파일 엑세스 타입(R/W) read */
        read(fd, rwType, 512);
        printf("read rw Type: %s\n", rwType);

        /* child fork */
        pid = fork();

        /* fork 실패 */
        if (pid < 0) {
            fprintf(stderr, "fork Failed");
            return 1;
        }
        /*child 수행*/
        else if(pid == 0)
        {
            printf("Child pid: %d\n", getpid()); // child processid 출력

            if(strcmp(rwType,"R")==0)
            {
                /* 파일 엑세스 타입이 Read일 때 */
                read(fd, readByteSize, 512);   // 읽을 데이터 바이트 수 read
                printf("read Byte Size: %s\n",readByteSize);   // read한 읽을 데이터 바이트 수 출력
                }
                if ( (fp = fopen(filename, "r")) == NULL) {
                    fprintf(stderr, "존재하지 않는 파일입니다.\n");
                    exit(1);
                    exit(1);
                }
                fread(getFileString, atoi(readByteSize), 1, fp);   // 파일로부터 readByteSize만큼 데이터 읽어 getFileString에 입력
                printf("Success get String: %s\n", getFileString); // 읽기에 성공한 데이터 string 출력
                write(fd2, getFileString,  strlen(getFileString)+1); // client에 보낼 데이터 string write
            } 
            else if(strcmp(rwType,"W")==0)
            {
                /* 파일 엑세스 타입이 Write일 때 */
                read(fd, writeString, 512); // 파일에 쓸 데이터 string read
                printf("Write String: %s\n", writeString); //read한 데이터 string 출력
	            fp = fopen(filename, "w");  // write 용으로 파일 open
	            fputs(writeString, fp);    // 파일에 write
                
                sprintf(writeByte, "%ld", strlen(writeString)); // 파일에 write한 데이터 byte 수 writeByte에 입력
                printf("Write String Byte Size: %s\n", writeByte); // writeByte size 출력
                write(fd2, writeByte,  strlen(writeByte)+1); // client로 보낼 writeByte size write
                write(fd2, temp, strlen(temp)+1); 
            }
            else
            {
                /* R/W 입력 오류 처리 */
                printf("R/W Type 입력 오류\n");
                exit(1);
            }
            
            /*파일 close*/
            fclose(fp);

            /*FIFO close*/
            close(fd);
            close(fd2);
        }
        // parent 수행(wait child)
        else
        {
            int status; // child 종료 상태
            printf("Parent pid: %d\n", getpid()); // parent processid 출력
            wait(&status); // child 종료 wait
            printf("status %d\n", status); // status 출력
            printf("Child Complete\n");   // child 수행 및 응답 완료 메세지 출력
            unlink(fifo1);
            unlink(fifo2);
        }
    return 0;
}