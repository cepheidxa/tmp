#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

void *wait_thread(void *arg)
{
	int status;
	int ret = wait(&status);
	if (ret == -1) {
		perror("wait");
		exit(EXIT_FAILURE);
	}
	printf("child process %d exit with %d\n", ret, status);

	exit(0);

}

int write_pipe(int fd, int n, int size)
{
	char *p = malloc(size);
	if (p == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	int i;
	for (i = 0; i < n; i++) {
		int ret = write(fd, p, size);
		if (ret == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
	free(p);
	return 0;
}

int read_pipe(int fd, int n, int size)
{
	char *p = malloc(size);
	if (p == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	int i;
	for (i = 0; i < n; i++) {
		int ret = read(fd, p, size);
		if (ret == -1) {
			perror("read");
			exit(EXIT_FAILURE);
		}
	}
	free(p);
	return 0;
}

int main(int argc, char *argv[])
{
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	long n = atol(argv[1]);
	long size = atol(argv[2]);

	pid_t pid = fork();

	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid != 0) {
		struct timespec start, end;
		pthread_t tid;
		close(pipefd[0]);
		pthread_create(&tid, NULL, wait_thread, NULL);
		clock_gettime(CLOCK_MONOTONIC, &start);
		write_pipe(pipefd[1], n, size);
		clock_gettime(CLOCK_MONOTONIC, &end);
		close(pipefd[1]);
		int second = end.tv_sec - start.tv_sec;
		long nsecond = end.tv_nsec - start.tv_nsec;
		if (nsecond < 0) {
			second -= 1;
			nsecond += 1000000000L;
		}
		double width =
		    (double)n * size / 1024 / 1024 / (second +
						      (double)(nsecond /
							       1000000000.0));
		printf("write time: %d.%03d, width %d M\n", second,
		       (int)(nsecond / 1000000), (int)width);
		usleep(10000000);
	} else {
		struct timespec start, end;
		close(pipefd[1]);
		clock_gettime(CLOCK_MONOTONIC, &start);
		read_pipe(pipefd[0], n, size);
		clock_gettime(CLOCK_MONOTONIC, &end);
		close(pipefd[0]);
		int second = end.tv_sec - start.tv_sec;
		long nsecond = end.tv_nsec - start.tv_nsec;
		if (nsecond < 0) {
			second -= 1;
			nsecond += 1000000000L;
		}
		printf("read time: %d.%03d\n", second,
		       (int)(nsecond / 1000000));
	}
	return 0;
}
