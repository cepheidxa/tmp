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

#define FILE_SIZE (4096*1000)

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "argc must be 2\n");
		exit(-1);
	}

	int ret = 0;

	char *file = argv[1];

	int fd = open(file, O_RDWR | O_CREAT, S_IRWXU);
	if (fd == -1) {
		error(-1, errno, "file(%s) open failed.", file);
	}

	ret = ftruncate(fd, FILE_SIZE);
	if (ret == -1) {
		error(-1, errno, "ftruncate file(%s) failed.", file);
	}

	char *addr =
	    (char *)mmap(NULL, FILE_SIZE, PROT_WRITE, MAP_SHARED|MAP_LOCKED, fd, 0);
	if (addr == MAP_FAILED) {
		error(-1, errno, "mmap file(%s) failed.", file);
	}

	//ret = madvise(addr, FILE_SIZE, MADV_DONTNEED);
	//ret = madvise(addr, FILE_SIZE, MADV_WILLNEED);
	//if(ret == -1) {
	//	error(-1, errno, "madvise failed.");
	//}

	//ret = munlock(addr, FILE_SIZE + 10000);
	//if(ret == -1) {
	//	error(-1, errno, "mlock failed.");
	//}

	printf("addr = %pF\n", addr);

	//usleep(1000000000);

	int i;
	for(i = 0; i < FILE_SIZE; i++) {\
		//usleep(1);
		addr[i] = i%255;
	}
	//ret = madvise(addr, FILE_SIZE, MADV_DONTNEED);
	//if(ret == -1) {
	//	error(-1, errno, "madvise failed.");
	//}
	//usleep(1000000000);

	ret = msync(addr, FILE_SIZE, MS_SYNC);
	if (ret == -1) {
		error(-1, errno, "msync addr failed.");
	}
	//usleep(1000000000);

	ret = munmap(addr, FILE_SIZE);
	if (ret == -1) {
		error(-1, errno, "munmap addr failed.");
	}

	ret = close(fd);
	if (ret == -1) {
		error(-1, errno, "close fd failed.");
	}

	return 0;
}
