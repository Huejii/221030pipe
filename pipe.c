#include <stdio.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h> 

#define MAXLINE 4096 
#define STDOUT_FILENO 1 
#define FIFO1 "/tmp/fifo.1" 
#define FIFO2 "/tmp/fifo.2" 
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) 

void client(int, int), server(int, int);

int main(int argc, char* argv[])
{
	int readfd, writefd;
	pid_t childpid;

	if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST))
	{
		printf("can't create %s", FIFO1);
		exit(1);
	}
	if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST))
	{
		unlink(FIFO1);
		printf("can't create %s", FIFO2);
		exit(1);
	}
	if ((childpid = fork()) == 0)
	{ /* child */
		readfd = open(FIFO1,O_RDONLY, 0);
		writefd = open(FIFO2,O_WRONLY, 0);

		server(readfd, writefd);
		exit(0);
	}

	/* parent */
	writefd = open(FIFO1, O_WRONLY, 0);
	readfd = open(FIFO2, O_RDONLY, 0);

	client(readfd, writefd);

	waitpid(childpid, NULL, 0); /* wait for child to terminate */


	close(readfd);
	close(writefd);

	unlink(FIFO1);
	unlink(FIFO2);
	exit(0);
}

void client(int readfd, int writefd)
{
	size_t len;
	size_t n;
	char buff[MAXLINE];

	/* read pathname */
	fgets(buff, MAXLINE, stdin);
	len = strlen(buff);
	if (buff[len - 1] == '\n')
		len--;

	fwrite(writefd, buff);

	while ((n = fread(readfd, buff, MAXLINE)) > 0)
		fwrite(STDOUT_FILENO, buff);
}

void server(int readfd, int writefd)
{
	int fd;
	size_t n;
	char buff[MAXLINE + 1];

	if ((n = fread(readfd, buff, MAXLINE)) == 0)
	{
		printf("end-of-file");
		exit(0);
	}
	buff[n] = '\0';

	if ((fd = open(buff, O_RDONLY)) < 0)
	{
		snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n", strerror(errno));
		n = strlen(buff);
		fwrite(writefd, buff);
	}
	else
	{
		while ((n = fread(fd, buff, MAXLINE)) > 0)
			fwrite(writefd, buff);
		close(fd);

	}
}