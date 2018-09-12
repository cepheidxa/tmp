#include <sys/mman.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "argc must be 3\n");
		exit(-1);
	}

	char *sourcefile = argv[1];
	char *destfile = argv[2];

	int fd = open(sourcefile, O_RDONLY);
	if (fd == -1) {
		error(-1, errno, "file(%s) open failed.", sourcefile);
	}

	struct stat stat;
	int ret = fstat(fd, &stat);
	if (ret == -1) {
		error(-1, errno, "fstat failed.");
	}

	char *addr =
	    (char *)mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED) {
		error(-1, errno, "mmap file(%s) failed.", sourcefile);
	}

	int fd2 = open(destfile, O_RDWR | O_CREAT, S_IRWXU);
	if (fd == -1) {
		error(-1, errno, "file(%s) open failed.", destfile);
	}

	ret = ftruncate(fd2, stat.st_size);
	if (ret == -1) {
		error(-1, errno, "ftruncate file(%s) failed.", destfile);
	}

	char *addr2 =
	    (char *)mmap(NULL, stat.st_size, PROT_WRITE, MAP_SHARED, fd2, 0);
	if (addr2 == MAP_FAILED) {
		error(-1, errno, "mmap file(%s) failed.", destfile);
	}

	memcpy(addr2, addr, stat.st_size);

	ret = msync(addr2, stat.st_size, MS_SYNC);
	if (ret == -1) {
		error(-1, errno, "msync addr2 failed.");
	}

	ret = munmap(addr, stat.st_size);
	if (ret == -1) {
		error(-1, errno, "munmap addr failed.");
	}

	ret = munmap(addr2, stat.st_size);
	if (ret == -1) {
		error(-1, errno, "munmap addr2 failed.");
	}

	ret = close(fd);
	if (ret == -1) {
		error(-1, errno, "close fd failed.");
	}

	ret = close(fd2);
	if (ret == -1) {
		error(-1, errno, "close fd2 failed.");
	}

	return 0;
}
