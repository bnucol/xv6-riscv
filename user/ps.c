#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  struct pstat uproc[NPROC];
  int nprocs;
  int i;
  char *state;
  static char *states[] = {
    [SLEEPING]  "sleeping",
    [RUNNABLE]  "runnable",
    [RUNNING]   "running ",
    [ZOMBIE]    "zombie  "
  };

  nprocs = getprocs(uproc);
  if (nprocs < 0)
    exit(-1);

  int currentTime = uptime(); // PA2 TASK 3 - retrieve current time to calculate a process's age

  printf("pid\tstate\t\tsize\tppid\tname\tpriority\tage\n"); // PA2 TASK 2 - add priority column to displayed proc stats
  for (i=0; i<nprocs; i++) {
    state = states[uproc[i].state];

    int age = 0; // PA2 TASK 3 - add age variable to store calculated

    if(uproc[i].state == RUNNABLE) { // PA2 TASK 3 - check if the current process is runnable; if so, calculate the age
      age = currentTime - uproc[i].readytime;
    }

    printf("%d\t%s\t%ld\t%d\t%s\t%d\t\t%d\n", uproc[i].pid, state,
                   uproc[i].size, uproc[i].ppid, uproc[i].name, uproc[i].priority, age); // PA2 TASK 2 - add priority information to display with the rest of the proc stats // PA2 TASK 3 - add age information to display with the rest of the proc stats
  }

  exit(0);
}
