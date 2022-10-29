#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    printf("Hello Server");
    int fd1;

    // FIFO path
    char * fifo1 = "/tmp/fifo1";
    char * fifo2 = "/tmp/fifo2";

    // Creating the named FIFP
    // mkfifo(<pathname>,<permission>)
    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    char temp[512], str2[80];
    while (1)
    {
        // First open in read only and read
        fd1 = open(fifo1,O_RDONLY);
        read(fd1, temp, 512);

        //처음으로 받는게 파일네임
        temp = "";
        printf("read1: %s\n", temp);
        temp = "";
        printf("read2: %s\n", temp);
        temp = "";
        printf("read3: %s\n", temp);
        
        close(fd1);

        // Now open in write mode and write
        // string taken from user.
        fd2 = open(fifo2,O_WRONLY);
       

       temp = "okay";
        write(fd2, temp, strlen(temp)+1);
        close(fd2);
    }
    return 0;
}