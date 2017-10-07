/*
 * env.c
 *
 *  Created on: Oct 7, 2017
 *      Author: zhangbo
 */

#include <stdio.h>

extern char **environ;
int main(int argc, char *argv[])
{
	char **ep;
	for(ep = environ; *ep != NULL; ep++)
		printf("%s\n", *ep);

	return 0;
}
