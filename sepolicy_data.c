#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc < 2)
		return -1;
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		error(-1, errno, "%s open failed.", argv[1]);
	}

	int size = lseek(fd, 0, SEEK_END);
	char *buf = (char *)malloc(size);
	if(!buf)
		error(-1, errno, "malloc faild.");
	int ret = read(fd, buf, size);
	ret = lseek(fd, 0, SEEK_SET);
	if(ret == -1)
		error(-1, errno, "%s lseek failed.", argv[1]);
	ret = read(fd, buf, size);
	if(ret != size)
		error(-1, errno, "%s read failed.", argv[1]);
	printf("unsigned char zte_sepolicy_data[] = {\n");
	unsigned char *ch = buf;
	int i;
	for(i = 0; i < size - 1; i++) {
		printf("0x%02x,", *ch);
		ch++;
		if((i+1) % 20 == 0)
			printf("\n");
	}
	printf("0x%02x,", *ch);
	printf("};");
	return 0;
}
