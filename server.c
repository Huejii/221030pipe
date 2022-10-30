#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



int main()
{
    // 추가필요: child fork하기

    int fd;
    int fd2;

    // FIFO path
    char * fifo1 = "/tmp/fifo1";
    char * fifo2 = "/tmp/fifo2";

    // Creating the named FIFP
    // mkfifo(<pathname>,<permission>)
    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    while (1)
    {
        // if(fd = open(fifo1, O_RDONLY) < 0)
        // {
        //     perror("open error : ");
        //     exit(0);
        // }
        // if(fd2 = open(fifo2, O_WRONLY) < 0)
        // {
        //     perror("open error : ");
        //     exit(0);
        // }
        fd = open(fifo1,O_RDONLY);

        char filename[512] = "";
        char rwType[512] = "";
        char writeString[512] = "";
        char readByteSize[512] = "";
        char writeByte[512]="";
        char getFileString[512]="";

        // First open in read only and read
        //fd = open(fifo1,O_RDONLY);
        //read(fd, filename, 512);
        //printf("file name: %s\n", filename);

        read(fd, filename, 512);
        printf("read file name: %s\n", filename);


        read(fd, rwType, 512);
        printf("read rw Type: %s\n", rwType);

        FILE *fp;
        if(strcmp(rwType,"R")==0)
        {
            read(fd, readByteSize, 512);
            printf("read Byte Size: %s\n",readByteSize);
            close(fd);
            fd2 = open(fifo2,O_WRONLY);
	        fp = fopen(filename, "r");
            //아래 추가: 바이트 수 만큼 파일읽고 읽은 것 write하기
            fread(getFileString, atoi(readByteSize), 1, fp);
            printf("Success get String: %s", getFileString);
            printf("\n");
            write(fd2, getFileString,  strlen(getFileString)+1);

        } 
        else if(strcmp(rwType,"W")==0)
        {
            read(fd, writeString, 512);
            printf("Write String: %s\n", writeString);
            close(fd);
            fd2 = open(fifo2,O_WRONLY);
	        fp = fopen(filename, "w");
	        fputs(writeString, fp);
            //아래 추가: write string 바이트 write하기
            sprintf(writeByte, "%ld", strlen(writeString));
            printf("%c\n", writeByte[0]);
            printf("%c\n", writeByte[1]);
            printf("%c\n", writeByte[2]);
            printf("%ld\n", strlen(writeByte));
            writeByte[strlen(writeByte)] = '\0';
            printf("%ld\n", strlen(writeByte));
            printf("Write String Byte Size: %s\n", writeByte);
            write(fd2, writeByte,  strlen(writeByte)+1); //수정필요

            char temp[512] = "okay";
            write(fd2, temp, strlen(temp)+1);\
        } else
        {
            printf("오류\n");
        }
            fclose(fp);
            close(fd2);
        // Now open in write mode and write
        // string taken from user.
        //fd2 = open(fifo2,O_WRONLY);
    }
    return 0;
}