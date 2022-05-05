/** 
 * Adapted from https://github.com/remzi-arpacidusseau/ostep-
projects/tree/master/scheduling-xv6-lottery 
 **/ 
#ifndef _PSTAT_H_ 
#define _PSTAT_H_ 
 
#include "param.h" 
 
struct pstat { 
  int inuse[NPROC]; // whether this slot of the process table is in use (1 or 0) 
  int pid[NPROC];   // the PID of each process 
  int nice[NPROC]; // the nice value of the process 
  
  int runtime[NPROC];
  int stride[NPROC];
  int pass[NPROC];
}; 
 
#endif // _PSTAT_H_
