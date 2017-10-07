/*
 * atomic_append.c
 *
 *  Created on: Oct 7, 2017
 *      Author: zhangbo
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int open_flag = O_RDWR|O_CREAT;
	if(argv[3][0] == 'x')
		open_flag |= O_APPEND;

	int fd = open(argv[1], open_flag, 0644);
	if(fd == -1) {
		perror("open failed.");
		exit(-1);
	}

	off_t offset;
	int ret;
	int len = atoll(argv[2]);
	while((offset = lseek(fd, 0, SEEK_END)) != -1) {
		usleep(rand()%2);
		ret = write(fd, "t", 1);
		if(ret == -1) {
			perror("write failed.");
			exit(-1);
		}
		len--;
		if(len == 0)
			break;
	}
	close(fd);
}
