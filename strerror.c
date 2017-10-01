#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc <= 1) {
		printf("Please input errno\nUsage: %s errno\n", argv[0]);
		return -1;
	}

	int num = atoi(argv[1]);
	printf("%s\n", strerror(num));

	return 0;
}

