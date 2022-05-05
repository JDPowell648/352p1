#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//Gets nice command from user and executes it accordingly
int main(int argc, char *argv[]) {
	char *amt = argv[1];
	char *toExecute = argv[2];
	char **args;
	args = &argv[2];
	int niceVal = atoi(amt);

	int isValid = nice(niceVal); 
	if(isValid == -1) {
		printf("Nice value should be between -20 and 19, but you input %d, please input new number\n", niceVal);
		exit(0);
	}
	exec(toExecute, args);

	exit(0);
}
