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

#define FILE_SIZE (4096*10000)

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "argc must be 2\n");
		exit(-1);
	}

	int ret = 0;

	char *file = argv[1];

	int fd2 = open(file, O_RDWR | O_CREAT, S_IRWXU);
	if (fd2 == -1) {
		error(-1, errno, "file(%s) open failed.", file);
	}

	ret = ftruncate(fd2, FILE_SIZE);
	if (ret == -1) {
		error(-1, errno, "ftruncate file(%s) failed.", file);
	}

	char *addr2 =
	    (char *)mmap(NULL, FILE_SIZE, PROT_WRITE, MAP_SHARED, fd2, 0);
	if (addr2 == MAP_FAILED) {
		error(-1, errno, "mmap file(%s) failed.", file);
	}

	ret = madvise(addr2, FILE_SIZE, MADV_DONTNEED);
	if(ret == -1) {
		error(-1, errno, "madvise failed.");
	}

	int i;
	for(i = 0; i < FILE_SIZE; i++) {\
		addr2[i] = i%255;
	}

	ret = msync(addr2, FILE_SIZE, MS_SYNC);
	if (ret == -1) {
		error(-1, errno, "msync addr2 failed.");
	}

	ret = munmap(addr2, FILE_SIZE);
	if (ret == -1) {
		error(-1, errno, "munmap addr2 failed.");
	}

	ret = close(fd2);
	if (ret == -1) {
		error(-1, errno, "close fd2 failed.");
	}

	return 0;
}
