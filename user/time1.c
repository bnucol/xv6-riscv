#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	if(argc == 1) {
		printf("time1: no commands provided\n");
		exit(1);
	}

	int startTime = uptime();

	int pid = fork();

	if(pid == 0) {
		exec(argv[1], argv + 1);

		printf("exec %s run unsuccessful\n", argv[1]);
		exit(1);
	} else {
		wait(0); // pass 0 in since status won't be stored
	}

	int endTime = uptime();
	int elapsedTime = endTime - startTime; // calculate elapsed time and print it

	printf("elapsed time: %d ticks\n", elapsedTime);
	exit(0); // exit program
}
