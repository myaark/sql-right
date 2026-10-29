#pragma once

#include <stddef.h>

typedef enum
{
    TOKEN_TYPE_UNKNOWN,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_INTEGER_LITERAL,
    TOKEN_TYPE_STRING_LITERAL,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_SEMICOLON,
    TOKEN_TYPE_PUNCTUATION
} TokenType;

typedef struct
{
    TokenType type;
    char *value;
    size_t length;
    size_t line;
    size_t column;
} Token;

Token *tokenize(const char *input, size_t *token_count);
void free_tokens(Token *tokens, size_t token_count);
