#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_BUFFER 1024

int main(void)
{
    char input[MAX_BUFFER];

    while (true)
    {
        char *line = readline("sqlright>: ");

        if (line == NULL) {
            printf("\nExiting...\n");
            break;
        }

        if (strlen(line) >= MAX_BUFFER) {
            fprintf(stderr, "Input too long; maximum is %d characters.\n",
                    MAX_BUFFER - 1);
            free(line);
            continue;
        }

        if (*line != '\0') {
            add_history(line);
        }

        strcpy(input, line);
        free(line);

        if (strcmp(input, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        printf("You entered: %s\n", input);
    }

    return 0;
}