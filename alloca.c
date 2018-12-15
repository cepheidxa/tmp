#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_size(size_t size)
{
	if(size / 1024 == 0) {
		printf("alloca %d Bytes success\n", size);
		return;
	} 
	size /= 1024;
	if(size / 1024 == 0) {
		printf("alloca %d K success\n", size);
		return;
	} 
	size /= 1024;
	if(size / 1024 == 0) {
		printf("alloca %d M success\n", size);
		return;
	} 
	size /= 1024;
	if(size / 1024 == 0) {
		printf("alloca %d G success\n", size);
		return;
	} 
	size /= 1024;
	printf("alloca %d T success\n", size);
}

void alloca_size(size_t size)
{
	void *p = alloca(size);
	if(p == NULL) {
		perror("alloca failed.");
		exit(-1);
	} else {
		print_size(size);
	}
}

int alloca_test(void)
{
	size_t step = 1024;
	for(size_t i = 0; ; i+=step) {
		alloca_size(i);
		if(i / step > 1024)
			step *= 1024;
	}
}

int main(int argc, char *argv[])
{
	alloca_test();
}
