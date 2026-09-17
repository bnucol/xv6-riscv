#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h" // TASK 5 - include pstat.h so rusage struct can be used
int main(int argc, char *argv[])
{
        if(argc == 1) {
                printf("time: no commands provided\n");
                exit(1);
        }

        int startTime = uptime();

        int pid = fork(); // create child process and assign its pid to the pids variable
	struct rusage childusage; // declare rusage struct to tract cputime

        if(pid == 0) { // if the child pid is 0, notify user of unsuccessful exec run
                exec(argv[1], argv + 1);

                printf("exec %s run unsuccessful\n", argv[1]);
                exit(1);
        } else { // otherwise, call wait2 to get the cpuTime
                wait2(0, &childusage); // pass 0 in since status won't be stored, and the address of childusage
        }

        int endTime = uptime();
        int elapsedTime = endTime - startTime; // calculate elapsed time and print it
	int cpuTime = childusage.cputime; // calculate cputime from the struct

	int cpuPercentage = 0; // calculate the cpu percentage ((cpuTime * 100) / elapsedTime)
	if(elapsedTime == 0) { // handle case where elapsed time may be 0 (resulting in division by zero)
		cpuPercentage = 0;
	} else if(elapsedTime > 0) {
		cpuPercentage = ((cpuTime * 100) / elapsedTime);
	}
        printf("elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n", elapsedTime, cpuTime, cpuPercentage);// print the three times
        exit(0); // exit program
}
