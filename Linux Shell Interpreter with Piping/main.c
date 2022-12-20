#define _GNU_SOURCE
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int MAX_WORDS = 100;

void make_words(char* line, char** words)
{
    int count;
    char* pos = line;
    for (count = 0; count < MAX_WORDS; count++) {
        // skip the spaces
        while (*pos == ' ' || *pos == '\t')
            pos++;

        // if finished scanning
        if (*pos == '\n' || *pos == '\0')
            break;

        // store the address of the argument
        words[count] = pos;

        // skip the characters of the argument
        while (*pos != ' ' && *pos != '\t' && *pos != '\n' && *pos != '\0')
            pos++;

        // write zero at end to zero terminate
        if (*pos != '\0')
            *(pos++) = '\0';
    }
    words[count] = NULL;
}

int main(int argc, char* argv[])
{
    char* line = NULL;
    size_t len;

    printf("*** Welcome to the CS3281 Shell\n");
    for (;;) {
        printf("*** $ ");
        if (getline(&line, &len, stdin) <= 0) {
            printf("\n");
            break;
        }

        // spacial exit command
        if (strcmp(line, "exit\n") == 0)
            break;

        char* line1 = strtok(line, "|");
        char* argv1[MAX_WORDS];
        make_words(line1, argv1);

        char* line2 = strtok(NULL, "|");
        if (line2 == NULL) {
            execute1(argv1);
        } else {
            char* argv2[MAX_WORDS];
            make_words(line2, argv2);
            execute2(argv1, argv2);
        }
    }
    printf("*** Closing the CS3281 Shell\n");

    free(line);
    return 0;
}
