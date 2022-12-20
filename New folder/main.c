
#define _GNU_SOURCE
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int MAX_ARGC = 100;

int main(int argc, char* argv[])
{
    char* line = NULL;
    size_t len;

    printf("*** Welcome to the CS3281 Shell\n");
    for (;;) {
        printf("CS3281$ ");
        if (getline(&line, &len, stdin) <= 0) {
            printf("\n");
            break;
        }

        // special exit command
        if (strcmp(line, "exit\n") == 0)
            break;

        char* shell_argv[MAX_ARGC + 1];
        int shell_argc;
        char* pos = line;
        for (shell_argc = 0; shell_argc <= MAX_ARGC; shell_argc++) {
            // skip the spaces
            while (*pos == ' ' || *pos == '\t')
                pos++;

            // if finished scanning
            if (*pos == '\n' || *pos == '\0')
                break;

            // store the address of the argument
            shell_argv[shell_argc] = pos;

            // skip the characters of the argument
            while (*pos != ' ' && *pos != '\t' && *pos != '\n' && *pos != '\0')
                pos++;

            // write zero at end to zero terminate
            if (*pos != '\0')
                *(pos++) = '\0';
        }
        shell_argv[shell_argc] = NULL;

        execute(shell_argv);
    }
    printf("*** Closing the CS3281 Shell\n");

    free(line);
    return 0;
}
