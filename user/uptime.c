#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  // call uptime and print its result, then exit the program
  int numTicks = uptime();
  printf("up %d clock ticks\n", numTicks);
  exit(0);
}
