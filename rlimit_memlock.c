#include <sys/mman.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <error.h>

int main(int argc, char *argv[])
{
	int ret = 0;

	struct rlimit limit = {
		.rlim_cur = 4096*10,
		.rlim_max = 4096*10,
	};
	ret = setrlimit(RLIMIT_MEMLOCK, &limit);
	if(ret == -1) {
		error(-1, errno, "setrlimit failed.");
	}

	ret = mlockall(MCL_CURRENT);
	if(ret == -1) {
		error(-1, errno, "mlockall failed.");
	}

	ret = munlockall();
	if(ret == -1) {
		error(-1, errno, "munlockall failed.");
	}
	return 0;
}
