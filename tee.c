/*
 * tee.c
 *
 *  Created on: Oct 7, 2017
 *      Author: zhangbo
 */


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[])
{
        int append_flag = 0;
        int opt;
        while((opt = getopt(argc, argv, "a")) != -1) {
                switch(opt) {
                case 'a':
                        append_flag = 1;
                        break;
                default:
                        printf("Usage: %s [-a] file\n", argv[0]);
                        exit(-1);
                }
        }

        if(argc < 2) {
            printf("Usage: %s [-a] file\n", argv[0]);
            exit(-1);
        }
        char *file_name = argv[argc -1];

        int open_flag = O_RDWR | O_CREAT;
        if(append_flag)
        	open_flag |= O_APPEND;
		int fd = open(file_name, open_flag, 0644);
        if(fd == -1) {
        	perror("open failed.");
        	exit(-1);
        }

        while(1) {
        	char buffer[BUFFER_SIZE];
        	int ret = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        	if(ret == -1) {
        		perror("read failed.");
        		break;
        	} else if(ret == 0)
        		break;

        	write(fd, buffer, ret);
        }
        close(fd);
        return 0;
}
