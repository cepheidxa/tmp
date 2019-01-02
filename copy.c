#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>

#define BUFFER_SIZE 4096
#define MAX_PROP_LEN 92

#define LOG()

static char buffer[BUFFER_SIZE];

#define PARTITIOM_PATH_MAX_LEN 50

/*
char partition_paths[][PARTITIOM_PATH_MAX_LEN] = {
	"/dev/block/bootdevice/by-name/splash",
	"/dev/block/bootdevice/by-name/splash_a",
	"/dev/block/bootdevice/by-name/splash_b",
};
*/
char partition_paths[][PARTITIOM_PATH_MAX_LEN] = {
	"/dev/sdb1",
	"/dev/block/bootdevice/by-name/splash_a",
	"/dev/block/bootdevice/by-name/splash_b",
};

char filepath[MAX_PROP_LEN] = "a.txt";

int write_partition(char *filepath, char *partition_path)
{
	int ret;
	int filefd, partitionfd;

	filefd = open(filepath, O_RDONLY);
	if(filefd == -1) {
		error(0, errno, "open file %s failed", filepath);
		return -1;
	}

	partitionfd = open(partition_path, O_RDWR);
	if(partitionfd == -1) {
		error(0, errno, "open file %s failed", partition_path);
		close(filefd);
		return -1;
	}

	while(1) {
		ret = read(filefd, buffer, BUFFER_SIZE);
		if(ret > 0) {
			int wret = write(partitionfd, buffer, ret);
			if(wret == -1) {
				error(0, errno, "write file %s failed", partition_path);
				ret = wret;
				goto err;
			}
		} else if(ret == -1){
			error(0, errno, "read file %s error", filepath);
			goto err;

		}

		if(ret == 0) {
			break;
		}
	}

err:
	close(filefd);
	close(partitionfd);
	return ret;

}

int main()
{
	int ret_splash = write_partition(filepath, partition_paths[0]);
	if(ret_splash != 0) {
		int ret_splasha = write_partition(filepath, partition_paths[1]);
		int ret_splashb = write_partition(filepath, partition_paths[2]);
		if(ret_splasha != 0 || ret_splashb != 0)
			return -1;
	}

	return 0;
}
