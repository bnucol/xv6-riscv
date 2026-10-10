struct rusage {
	uint cputime;
};

#ifndef PROCSTATE_DEFINED // PA2 TASK 1 - checks if procstate was not defined; if so, define procstate here; otherwise, skip the lines until the endif
#define PROCSTATE_DEFINED // PA2 TASK 1 - declare procstate flag so other pstat.h and proc.h files do not attempt to define it again
enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
#endif // PA2 TASK 1 - closes the ifndef statement

struct pstat { // PA2 TASK 1 - add pstat struct
	int pid; // Process ID
	enum procstate state; // Process state
	uint64 size; // Size of process memory (bytes)
	int ppid; // Parent process ID
	char name[16]; // Parent command name
};
