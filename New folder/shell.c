#include "shell.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>


void execute(char* const args[])
{
    int i;
    printf("*** Entered:");
    for (i = 0; args[i] != NULL; i++)
        printf(" %s", args[i]);
    printf(" (%d words)\n", i);
    if (i == 0) {
    	printf("No command was entered.\n");
    	return;
    }
	pid_t pid;
	pid = fork();
	int child_status;
	if (pid == 0) { // child runs program
		if (execvp(args[0], args) < 0) {
			printf("Command not found\n");
			exit(1);
		}
	}
	else if (pid < 0) {
		printf("Fork failed");
		return;
	}
	wait(&child_status); // parent reaps child
	if (WIFEXITED(child_status)) {
		if (WEXITSTATUS(child_status) == 0) {
			printf("Command exited successfully\n");
		}
		else {
			printf("Command exited with %d\n", WEXITSTATUS(child_status));
		}	
	}
}	
