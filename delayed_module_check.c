#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int start_delayed_module_init(void)
{
	char filename[] = "a.txt";
	int ret = 0;
	int fd = open(filename, O_RDWR);
	if(fd == -1) {
		error(-1, errno, "%s open failed.", filename);
		return -1;
	}
	lseek(fd, 0, SEEK_SET);
	ret = write(fd, "1", 1);
	if(fd == -1) {
		error(-1, errno, "%s write failed.", filename);
		return -1;
	}
	close(fd);
	return 0;
}

int wait_for_delayed_module_complete(void)
{
	char filename[] = "a.txt";
	char buf[5] = {0,};
	int ret = 0;
	int fd = open(filename, O_RDONLY);
	int retry_count = 100000 * 30;
	if(fd == -1) {
		error(-1, errno, "%s open failed.", filename);
		return -1;
	}
	ret = read(fd, buf, 4);
	if(ret == -1) {
		error(-1, errno, "%d read failed.", filename);
		return -1;
	}
	buf[ret] = '\0';
	close(fd);
	while(retry_count-- && atoi(buf)) {
		usleep(10);
		fd = open(filename, O_RDONLY);
		if(fd == -1) {
			error(-1, errno, "%s open failed.", filename);
			return -1;
		}
		ret = read(fd, buf, 4);
		if(ret == -1) {
			error(-1, errno, "%d read failed.", filename);
			close(fd);
			return -1;
		}
		buf[ret] = '\0';
		close(fd);
	}
	if(retry_count)
		return 0;
	else
		return -1;
}

int main()
{
	start_delayed_module_init();
	wait_for_delayed_module_complete();
	return 0;
}
