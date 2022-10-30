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
        fd = open(fifo1,O_RDONLY);
        fd2 = open(fifo2,O_WRONLY);

        read(fd, filename, 512);
        printf("file name: %s\n", filename);

        read(fd, rwType, 512);
        printf("rw Type: %s\n", rwType);

        FILE *fp;
        if(rwType == "R")
        {
            read(fd, readByteSize, 512);
            printf("Byte Size: %s\n",readByteSize);
	        fp = fopen(filename, "R");
            //추가 필요: 바이트 수 만큼 파일읽고 읽은 것 write하기
            fread(getFileString, atoi(readByteSize), 1, fp);
            printf("Success get String: %s\n", getFileString);
            write(fd2, getFileString,  strlen(getFileString)+1);

        } else
        {
            read(fd, writeString, 512);
            printf("Write String: %s\n", writeString);
	        fp = fopen(filename, "w");
	        fputs(writeString, fp);
            //추가 필요: write string 바이트 write하기
            sprintf(writeByte, "%d", strlen(writeString));
            printf("Write String Byte Size: %s\n", writeByte);
            write(fd2, writeByte,  strlen(writeByte)+1); //수정필요

            char temp[512] = "okay";
            write(fd2, temp, strlen(temp)+1);
        }
            fclose(fp);
            close(fd);
            close(fd2);
        // Now open in write mode and write
        // string taken from user.
        //fd2 = open(fifo2,O_WRONLY);
    }
    return 0;
}