#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// TASK 4 - implement sys_wait2()
uint64 sys_wait2(void) {
  uint64 p;
  uint64 rusage; // add variable for the rusage parameter

  argaddr(0, &p);
  argaddr(1, &rusage); // get the rusage argument from the parameters

  return kwait2(p, rusage); // pass the addresses of the exit status and rusage to wait2 and return the result (child pid or error code)

}

// PA2 TASK 1 - implement sys_getprocs()
uint64 sys_getprocs(void) {
  uint64 p; // declare the pstat array address argument from the user

  argaddr(0, &p); // get the user pointer from the pstat argument
  return kgetprocs(p); // call the kernel operation for getprocs()

}

// PA2 TASK 2 - implement sys_getpriority()
uint64 sys_getpriority(void) {
  struct proc *currProcess = myproc(); // get the process that called this syscall
  int currProcessPriority; // declare variable that will store the current process's priority

  acquire(&currProcess->lock); // lock the process info so it doesn't change when being read
  currProcessPriority = currProcess->priority; // store the current process's priority into the variable
  release(&currProcess->lock); // unlock the process info since it is not being read anymore

  return currProcessPriority; // return the priority of the current process
}

// PA2 TASK 2 - implement sys_setpriority()
uint64 sys_setpriority(void) {
  struct proc *currProcess = myproc(); // get the process that called this syscall
  int inputPriority; // declare variable that will store the inputted priority

  argint(0, &inputPriority); // read the value of the input priority that was passed into the syscall

  if(!(inputPriority >= 0 && inputPriority <= 49)) { // check if the input priority is not in the range of priorities (0-49 in the instructions); if so, return -1 since this value is out of range
    return -1;
  }

  acquire(&currProcess->lock); // lock the current process's info so it doesn't change when being read
  currProcess->priority = inputPriority; // set the current process's priority to the input value
  release(&currProcess->lock); // unlock the current process's info since it is not being read anymore

  return 0; // return 0 since the operation was successful
}
