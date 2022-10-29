#include <stdio.h>
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

        // First open in read only and read
        fd = open(fifo1,O_RDONLY);
        read(fd, filename, 512);
        printf("read1: %s\n", filename);

        read(fd, rwType, 512);
        printf("read2: %s\n", rwType);

        read(fd, writeString, 512);
        printf("read3: %s\n", writeString);

        FILE *fp;
	    fopen_s(&fp, filename, "w");
	    fputs(writeString, fp);
        fclose(fp);
        
        close(fd);

        // Now open in write mode and write
        // string taken from user.
        fd2 = open(fifo2,O_WRONLY);
       

        char temp[512] = "okay";
        write(fd2, temp, strlen(temp)+1);
        close(fd2);
    }
    return 0;
}