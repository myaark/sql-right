
#include "tokenizer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* Consume one character and update its source position. */
static void advance_position(
    const char *input,
    size_t *index,
    size_t *line,
    size_t *column)
{
    if (input[*index] == '\n')
    {
        (*line)++;
        *column = 1;
    }
    else
    {
        (*column)++;
    }

    (*index)++;
}

/* Copy a substring into a null-terminated string. */
static char *copy_string(const char *input, size_t length)
{
    char *copy = malloc(length + 1);

    if (!copy)
        return NULL;

    memcpy(copy, input, length);
    copy[length] = '\0';

    return copy;
}

/* Identify a small set of SQL keywords, case-insensitively. */
static int is_keyword(const char *value)
{
    static const char *keywords[] = {
        "SELECT", "FROM", "WHERE",
        "INSERT", "INTO", "VALUES",
        "UPDATE", "SET", "DELETE",
        "CREATE", "TABLE", "DROP",
        "AND", "OR", "NOT", "NULL",
        "IS", "IN", "AS", "JOIN",
        "ON", "ORDER", "BY", "GROUP",
        "HAVING", "LIMIT", "DISTINCT",
        "PRIMARY", "KEY", "INTEGER",
        "TEXT", "AND"};

    size_t count = sizeof(keywords) / sizeof(keywords[0]);

    for (size_t k = 0; k < count; k++)
    {
        if (strlen(value) != strlen(keywords[k]))
            continue;

        size_t j = 0;

        while (value[j] != '\0' &&
               toupper((unsigned char)value[j]) ==
                   keywords[k][j])
        {
            j++;
        }

        if (value[j] == '\0')
            return 1;
    }

    return 0;
}

/* Append a token to the dynamically growing array. */
static int append_token(
    Token **tokens,
    size_t *count,
    size_t *capacity,
    TokenType type,
    const char *input,
    size_t start,
    size_t length,
    size_t line,
    size_t column)
{
    if (*count == *capacity)
    {
        size_t new_capacity = (*capacity == 0) ? 16 : *capacity * 2;

        if (new_capacity < *capacity ||
            new_capacity > (size_t)-1 / sizeof(Token))
            return 0;

        Token *grown = realloc(
            *tokens, new_capacity * sizeof(Token));

        if (!grown)
            return 0;

        *tokens = grown;
        *capacity = new_capacity;
    }

    char *value = copy_string(input + start, length);

    if (!value)
        return 0;

    (*tokens)[*count] = (Token){
        .type = type,
        .value = value,
        .length = length,
        .line = line,
        .column = column};

    (*count)++;
    return 1;
}

void free_tokens(Token *tokens, size_t token_count)
{
    for (size_t i = 0; i < token_count; i++)
        free(tokens[i].value);

    free(tokens);
}

Token *tokenize(const char *input, size_t *token_count)
{
    if (!token_count)
        return NULL;

    *token_count = 0;

    if (!input)
        return NULL;

    Token *tokens = NULL;
    size_t count = 0;
    size_t capacity = 0;

    size_t i = 0;
    size_t line = 1;
    size_t column = 1;

    while (input[i] != '\0')
    {
        unsigned char c = (unsigned char)input[i];

        /* Skip whitespace. */
        if (isspace(c))
        {
            advance_position(input, &i, &line, &column);
            continue;
        }

        /* Skip single-line SQL comments. */
        if (input[i] == '-' && input[i + 1] == '-')
        {
            while (input[i] != '\0' && input[i] != '\n')
                advance_position(input, &i, &line, &column);

            continue;
        }

        /* Skip block comments. */
        if (input[i] == '/' && input[i + 1] == '*')
        {
            advance_position(input, &i, &line, &column);
            advance_position(input, &i, &line, &column);

            while (input[i] != '\0' &&
                   !(input[i] == '*' && input[i + 1] == '/'))
            {
                advance_position(input, &i, &line, &column);
            }

            if (input[i] == '\0')
                goto error;

            advance_position(input, &i, &line, &column);
            advance_position(input, &i, &line, &column);
            continue;
        }

        size_t start = i;
        size_t start_line = line;
        size_t start_column = column;
        TokenType type;
        size_t token_length;

        /* Identifiers and keywords. */
        if (isalpha(c) || c == '_')
        {
            while (isalnum((unsigned char)input[i]) ||
                   input[i] == '_')
            {
                advance_position(input, &i, &line, &column);
            }

            token_length = i - start;
            char *word = copy_string(input + start, token_length);

            if (!word)
                goto error;

            type = is_keyword(word)
                       ? TOKEN_TYPE_KEYWORD
                       : TOKEN_TYPE_IDENTIFIER;

            free(word);
        }
        /* Integer literals. */
        else if (isdigit(c))
        {
            while (isdigit((unsigned char)input[i]))
                advance_position(input, &i, &line, &column);

            type = TOKEN_TYPE_INTEGER_LITERAL;
        }
        /* Single-quoted SQL strings, including escaped quotes. */
        else if (input[i] == '\'')
        {
            advance_position(input, &i, &line, &column);

            int closed = 0;

            while (input[i] != '\0')
            {
                if (input[i] == '\'')
                {
                    advance_position(input, &i, &line, &column);

                    if (input[i] == '\'')
                    {
                        /* SQL represents an embedded quote as ''. */
                        advance_position(input, &i, &line, &column);
                    }
                    else
                    {
                        closed = 1;
                        break;
                    }
                }
                else
                {
                    advance_position(input, &i, &line, &column);
                }
            }

            if (!closed)
                goto error;

            type = TOKEN_TYPE_STRING_LITERAL;
        }
        /* Operators, including two-character operators. */
        else if (strchr("=<>!+-*/%", input[i]))
        {
            char first = input[i];

            advance_position(input, &i, &line, &column);

            if ((first == '<' &&
                 (input[i] == '=' || input[i] == '>')) ||
                (first == '>' && input[i] == '=') ||
                (first == '!' && input[i] == '=') ||
                (first == '=' && input[i] == '=') ||
                (first == '|' && input[i] == '|'))
            {
                advance_position(input, &i, &line, &column);
            }

            type = TOKEN_TYPE_OPERATOR;
        }
        /* Semicolons and punctuation. */
        else if (input[i] == ';')
        {
            advance_position(input, &i, &line, &column);
            type = TOKEN_TYPE_SEMICOLON;
        }
        else if (strchr("(),.", input[i]))
        {
            advance_position(input, &i, &line, &column);
            type = TOKEN_TYPE_PUNCTUATION;
        }
        else
        {
            /* Unknown input: don't silently accept it. */
            goto error;
        }

        token_length = i - start;

        if (!append_token(
                &tokens, &count, &capacity,
                type, input, start, token_length,
                start_line, start_column))
        {
            goto error;
        }
    }

    *token_count = count;
    return tokens;

error:
    free_tokens(tokens, count);
    *token_count = 0;
    return NULL;
}
