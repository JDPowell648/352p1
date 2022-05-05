#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/pstat.h"

int
main(void)
{
  if(fork() > 0){
	sleep(40);
  	struct pstat stats;
  	getpstat(&stats);
	for(int i = 0; i < sizeof(stats.inuse)/sizeof(int); i++){
        	if(stats.inuse[i] != 0){
        		printf("Slot: %d, ", i);
        		printf("Is Active: %d, ", stats.inuse[i]);
        		printf("PID: %d, ", stats.pid[i]);
       			printf("Nice Value:%d, ", stats.nice[i]);
        		printf("Stride Value:%d, ", stats.stride[i]);
  	      		printf("Pass Value:%d, ", stats.pass[i]);
  	      		printf("Current Runtime:%d\n", stats.runtime[i]);
  	      }
  	}
  	printf("\n");
	exit(0);
  }else{
  	if(fork() > 0){
		nice(14);
	}else{
		nice(19);
	}
	while(1);
  }
  exit(0);
}

