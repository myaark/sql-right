#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tokenizer/tokenizer.h"

static void execute(const char *input)
{
    size_t token_count = 0;
    Token *tokens = tokenize(input, &token_count);

    if (!tokens)
    {
        fprintf(stderr, "Error: Tokenization failed\n");
        return;
    }

    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; i++)
    {
        printf("Type: %d, Value: '%s', Length: %zu, Line: %zu, Column: %zu\n",
               tokens[i].type,
               tokens[i].value,
               tokens[i].length,
               tokens[i].line,
               tokens[i].column);
    }

    free_tokens(tokens, token_count);
}
static void run_meta_command(const char *input)
{
    if (strcmp(input, ".exit") == 0)
    {
        printf("Exiting...\n");
        exit(EXIT_SUCCESS);
    }
    else if (strcmp(input, ".help") == 0)
    {
        printf("Available commands:\n.exit - Exit the program\n.help - Show this help message\n");
    }
    else
    {
        printf("Unrecognized command '%s'. Type '.help' for available commands.\n", input);
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    char *stmt = NULL;
    size_t stmt_length = 0;

    for (;;)
    {
        char *line = readline(stmt ? "  ...> " : "sqlright>: ");

        if (!line)
        {
            putchar('\n');
            break;
        }

        size_t n = strlen(line);
        while (n > 0 && isspace((unsigned char)line[n - 1]))
        {
            line[--n] = '\0';
        }

        // blank line
        if (n == 0)
        {
            free(line);
            continue;
        }

        // meta-commands
        if (!stmt && line[0] == '.')
        {
            add_history(line);
            run_meta_command(line);
            free(line);
            continue;
        }

        char *new_input = realloc(stmt, stmt_length + n + 2);
        if (!new_input)
        {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free(line);
            return EXIT_FAILURE;
        }
        stmt = new_input;
        if (stmt_length > 0)
        {
            stmt[stmt_length++] = ' ';
        } // without this SELCT * FROM users; would be SELECT *FROM users;

        memcpy(stmt + stmt_length, line, n);
        stmt_length += n;
        stmt[stmt_length] = '\0';
        free(line);

        if (stmt_length > 0 && stmt[stmt_length - 1] == ';')
        {
            add_history(stmt);
            execute(stmt);
            free(stmt);
            stmt = NULL;
            stmt_length = 0;
        }
    }
    free(stmt);
    return 0;
}
