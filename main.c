#include <stdio.h>
#include "lexer.h"

/* Helper function to convert enum to string */
const char* tokenTypeToString(TokenType type)
{
    switch (type)
    {
        case KEYWORD:           return "KEYWORD";
        case IDENTIFIER:        return "IDENTIFIER";
        case CONSTANT:          return "CONSTANT";
        case OPERATOR:          return "OPERATOR";
        case SPECIAL_CHARACTER: return "SPECIAL_CHARACTER";
        case EOF_TOKEN:         return "EOF";
        default:                return "UNKNOWN";
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <source_file.txt>\n", argv[0]);
        return 1;
    }

    initializeLexer(argv[1]);

    printf("----------------------------------------");
    printf("\n| %-15s | %-20s|\n", "TOKEN", "TYPE");
    printf("----------------------------------------\n");

    Token token;
    while (1)
    {
        token = getNextToken();

        if (token.type == EOF_TOKEN)
            break;

        printf("| %-15s | %-20s|\n",
               token.lexeme,
               tokenTypeToString(token.type));
               //printf("-----------------------------------\n");
    }
     printf("----------------------------------------\n");
    return 0;
}
