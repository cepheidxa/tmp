#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void print_size(size_t size)
{
	if(size / 1024 == 0) {
		printf("malloc %d Bytes success\n", size);
		return;
	} 
	size /= 1024;
	if(size / 1024 == 0) {
		printf("malloc %d K success\n", size);
		return;
	} 
	size /= 1024;
	if(size / 1024 == 0) {
		printf("malloc %d M success\n", size);
		return;
	} 
	size /= 1024;
	if(size / 1024 == 0) {
		printf("malloc %d G success\n", size);
		return;
	} 
	size /= 1024;
	printf("malloc %d T success\n", size);
}

int malloc_test()
{
	size_t step = 1024;
	for(size_t i = 0; ; i+=step) {
		void *p = malloc(i);
		if(p == NULL) {
			perror("malloc failed.");
			exit(-1);
		} else {
			print_size(i);
			free(p);
		}

		if(i / step > 1024)
			step *= 1024;
	}
}

int main(int argc, char *argv[])
{
	malloc_test();
}
