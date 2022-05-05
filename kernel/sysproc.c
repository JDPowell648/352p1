#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "pstat.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
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

uint64
sys_pcount(void)
{
  uint counter = 0;
  for(int i = 0; i < NPROC; i++) {
    if(proc[i].state != UNUSED) {
      counter++;
    }
  }
  return counter;
}

//Sets the nice value of a process
uint64
sys_nice(void) {
	int n;
	argint(0, &n);
	if(n < -20 || n > 19) {
		return -1;
	}

	static const int nice_to_tickets[40] = { 
	 /* -20 */     88761,     71755,     56483,     46273,     36291, 
	 /* -15 */     29154,     23254,     18705,     14949,     11916, 
	 /* -10 */      9548,      7620,      6100,      4904,      3906, 
	 /*  -5 */      3121,      2501,      1991,      1586,      1277, 
	 /*   0 */      1024,       820,       655,       526,       423, 
	 /*   5 */       335,       272,       215,       172,       137, 
	 /*  10 */       110,        87,        70,        56,        45, 
	 /*  15 */        36,        29,        23,        18,        15, 
	};
	//printf("Setting stride to: %d\n", 1000000 / nice_to_tickets[n+20]);
	myproc()->stride = 1000000 / nice_to_tickets[n+20];
	myproc()->nice = n;
	return 0;
}

uint64 
sys_getpstat(void) { 
    uint64 result = 0; 
    struct proc *p = myproc(); 
    uint64 upstat; // the virtual (user) address of the passed argument struct pstat 
    struct pstat kpstat; // a struct pstat in kernel memory 
 
    // get the system call argument passed by the user program 
    if (argaddr(0, &upstat) < 0) 
        return -1; 
 
     
    // TODO: fill the arrays in kpstat (see the definition of struct pstat above).
    // The data to fill in the arrays comes from the process table array proc[]. 
    for(int i = 0; i < NPROC; i++){
    	if(proc[i].state != UNUSED){
	    kpstat.inuse[i] = 1;
	}else{
	    kpstat.inuse[i] = 0;
	}
	kpstat.pid[i] = proc[i].pid;
	kpstat.nice[i] = proc[i].nice;
	kpstat.stride[i] = proc[i].stride;
	kpstat.pass[i] = proc[i].pass;
    	kpstat.runtime[i] = proc[i].runtime;
    }
 
    // copy pstat from kernel memory to user memory 
    if (copyout(p->pagetable, upstat, (char *)&kpstat, sizeof(kpstat)) < 0) 
        return -1; 
 
    return result; 
} 
