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
	int ret;
	int fd;
	int size;
	unsigned char *buf;
	unsigned char *ch;
	int i;

	if (argc < 2)
		return -1;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		error(-1, errno, "%s open failed.", argv[1]);
	}

	size = lseek(fd, 0, SEEK_END);
	buf = (unsigned char *)malloc(size);
	if (!buf)
		error(-1, errno, "malloc faild.");
	ret = lseek(fd, 0, SEEK_SET);
	if (ret == -1)
		error(-1, errno, "%s lseek failed.", argv[1]);
	ret = read(fd, buf, size);
	if (ret != size)
		error(-1, errno, "%s read failed.", argv[1]);
	printf("unsigned char zte_sepolicy_data[] = {\n");
	ch = buf;
	for (i = 0; i < size - 1; i++) {
		printf("0x%02x,", *ch);
		ch++;
		if ((i + 1) % 20 == 0)
			printf("\n");
	}
	printf("0x%02x", *ch);
	printf("};");
	free(buf);
	return 0;
}
