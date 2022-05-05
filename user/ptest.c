//Blake Robson
//The print statement before calling fork returns a process count of three.
//After calling fork, it returns a process count of four. This makes sense,
//since fork() would create a new process.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  printf("%d", pcount());
  if(fork() > 0) {
    sleep(5);
    printf("%d", pcount());
  }
  exit(0);
}
