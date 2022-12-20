#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute1(char* const args[])
{
    int i;
    printf("*** Command:");
    for (i = 0; args[i] != NULL; i++)
        printf(" %s", args[i]);
    printf(" (%d words)\n", i);

    if (args[0] == NULL) {
        printf("*** Empty command\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        printf("*** Command not found: %s\n", args[0]);
        exit(1);
    } else if (pid > 0) {
        int status = 0;
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) == 0)
            printf("*** Child exited sucessfully\n");
        else
            printf("*** Child exited with %d\n", WEXITSTATUS(status));
    } else {
        printf("*** Fork failed\n");
    }
}

void execute2(char* const args1[], char* const args2[])
{
    int i;
    printf("*** First command:");
    for (i = 0; args1[i] != NULL; i++)
        printf(" %s", args1[i]);
    printf(" (%d words)\n", i);

    printf("*** Second command:");
    for (i = 0; args2[i] != NULL; i++)
        printf(" %s", args2[i]);
    printf(" (%d words)\n", i);
    
    int pipefd[2];
    pid_t child_pid1;
    pid_t child_pid2;
  	
  	if (pipe(pipefd) == -1) {
  		printf("*** Pipe failed\n");
  		exit(1);
  	}
  	
  	// Create first child process
  	if ((child_pid1 = fork()) < 0) {
  		printf("*** Fork to child 1 failed\n");
  		exit(1);
  	}
  	
  	
  	if (child_pid1 == 0) {
  		// We know we are in child 1
  		// We now need to move write end to stdout of file descriptor arr
  		if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
  			printf("*** Duplicating write end to stdout of Child 1 fd failed\n");
  			exit(1);
  		}
  		
  		// Close necessary file descriptors
  		if (close(pipefd[0]) == -1) {
  			printf("*** Closing read fd for child 1 failed\n");
  			exit(1);
  		}
  		if (close(pipefd[1]) == -1) {
  			printf("*** Closing write fd for child1 failed\n");
  			exit(1);
  		}
  		
  		// execute first command -- output provided as input to
  		// second command
  		if (execvp(args1[0], args1) < 0) {
  			printf("Command not found\n");
  			exit(1);
  		}
  	}
  	
  	// Parent process waits for child process, forks to a second child
  	// when first child completes
  	if (child_pid1 > 0) {
  		int child1_status;
  		waitpid(child_pid1, &child1_status, 0);
  		if (WEXITSTATUS(child1_status) == 0)
            printf("*** Child 1 exited sucessfully\n");
        else
            printf("*** Child 1 exited with %d\n", WEXITSTATUS(child1_status));
  		if ((child_pid2 = fork()) == -1) {
  			printf("*** Fork to child 2 failed\n");
  			exit(1);
  		}
  		
  	}
  	
  	
  	if (child_pid2 == 0) {
  		// If we are here, we know we are in the child
  		// We now need to move read end to STD IN
  		if (dup2(pipefd[0], STDIN_FILENO) == -1) {
  			printf("*** Duplicating read fd to stdin for child 2 failed\n");
  			exit(1);
  		}
  		
  		// Close necessary file descriptors
  		if (close(pipefd[0]) == -1) {
  			printf("*** Closing read fd for child 2 failed\n");
  			exit(1);
  		}
  		
  		if (close(pipefd[1]) == -1) {
  			printf("*** Closing write fd for child 2 failed\n");
  			exit(1);
  		}
  		
  		// execute command
  		if (execvp(args2[0], args2) < 0) {
  			printf("Command not found\n");
  			exit(1);
  		}
  	}
  	else {
  		// Parent waits on child
  		int child2_status;
  		
  		// Closes necessary shell file descriptors first
  		if (close(pipefd[0]) == -1) {
  			printf("*** Closing shell read fd failed\n");
  			exit(1);
  		}
  		
  		if (close(pipefd[1]) == -1) {
  			printf("*** Closing shell write fd failed\n");
  			exit(1);
  		}
  		waitpid(child_pid2, &child2_status, 0);
  		if (WEXITSTATUS(child2_status) == 0)
            printf("*** Child 2 exited sucessfully\n");
        else
            printf("*** Child 2 exited with %d\n", WEXITSTATUS(child2_status));
  	}	
}
