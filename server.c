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
    int fd;
    int fd2;

    // FIFO path
    char * fifo1 = "/tmp/fifo1";
    char * fifo2 = "/tmp/fifo2";
    pid_t pid;

    // Creating the named FIFO
    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    while (1)
    {
        fd = open(fifo1,O_RDONLY);
        fd2 = open(fifo2,O_WRONLY);
        if (fd == -1) {
            fprintf(stderr, "Pipe Failed");
            return 1;
        }
        if (fd2 == -1) {
            fprintf(stderr, "Pipe Failed");
            return 1;
        }
        FILE *fp;

        char filename[512] = "";
        char rwType[512] = "";
        char writeString[512] = "";
        char readByteSize[512] = "";
        char writeByte[512]="";
        char getFileString[512]="";
        char temp[512] = "";

        read(fd, filename, 512);
        if(strcmp(filename, "")==0)
        {
            break;
        }
        printf("read file name: %s\n", filename);


        read(fd, rwType, 512);
        printf("read rw Type: %s\n", rwType);

        //child fork
        pid = fork();

        if (pid < 0) {
            fprintf(stderr, "fork Failed");
            return 1;
        }
        else if(pid == 0)
        {
            printf("Child pid: %d\n", getpid());
            if(strcmp(rwType,"R")==0)
            {
                read(fd, readByteSize, 512);
                printf("read Byte Size: %s\n",readByteSize);
	            fp = fopen(filename, "r");
            //아래 추가: 바이트 수 만큼 파일읽고 읽은 것 write하기
                fread(getFileString, atoi(readByteSize), 1, fp);
                printf("Success get String: %s\n", getFileString);
                write(fd2, getFileString,  strlen(getFileString)+1);
            } 
            else if(strcmp(rwType,"W")==0)
            {
                read(fd, writeString, 512);
                printf("Write String: %s\n", writeString);
	            fp = fopen(filename, "w");
	            fputs(writeString, fp);
                //아래 추가: write string 바이트 write하기
                sprintf(writeByte, "%ld", strlen(writeString));
                printf("Write String Byte Size: %s\n", writeByte);
                write(fd2, writeByte,  strlen(writeByte)+1); //수정필요
                write(fd2, temp, strlen(temp)+1);
            }
            else
            {
                printf("R/W Type 입력 오류\n");
                exit(1);
            }

            fclose(fp);

            close(fd);
            close(fd2);
        }
        else
        {
            int status;
            printf("Parent pid: %d\n", getpid());
            wait(&status);
            printf("status %d\n", status);
            printf("Child Complete\n");
        }
    }
    return 0;
}