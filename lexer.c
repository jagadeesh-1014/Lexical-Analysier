
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include <stdlib.h>



#if 1

/* Global file pointer */
static FILE *sourceFile;

/* Keyword table */
static const char* keywords[MAX_KEYWORDS] =
{
    "int", "float", "return", "if", "else", "while", "for", "do",
    "break", "continue", "char", "double", "void", "switch",
    "case", "default", "const", "static", "sizeof", "struct"
};

/* Initialization */
void initializeLexer(const char* filename)
{
    char *extn = strrchr(filename, '.');

    if (extn == NULL || strcmp(extn, ".txt") != 0)
    {
        printf("Error: Invalid file type\n");
        exit(1);
    }

    sourceFile = fopen(filename, "r");
    if (!sourceFile)
    {
        printf("Error: Cannot open file\n");
        exit(1);
    }

    printf("INFO: Lexer initialized successfully\n");
}

/* Keyword check */
int isKeyword(const char* str)
{
    for (int i = 0; i < MAX_KEYWORDS; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

/* Operator check (single-char) */
int isOperator(const char* str)
{
    const char* ops = "+-*/%=<>!";
    return (str[1] == '\0' && strchr(ops, str[0]));
}

/* Special character check */
int isSpecialCharacter(char ch)
{
    const char* sp = ",;{}()[]";
    return strchr(sp, ch) != NULL;
}

/* Integer constant check */
int isConstant(const char* str)
{
    if (*str == '\0') return 0;

    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

/* Identifier check */
int isIdentifier(const char* str)
{
    if (!isalpha(str[0]) && str[0] != '_')
        return 0;

    for (int i = 1; str[i]; i++)
    {
        if (!isalnum(str[i]) && str[i] != '_')
            return 0;
    }

    return !isKeyword(str);
}

/* Categorize token */
void categorizeToken(Token* token)
{
    if (isKeyword(token->lexeme))
        token->type = KEYWORD;
    else if (isConstant(token->lexeme))
        token->type = CONSTANT;
    else if (isIdentifier(token->lexeme))
        token->type = IDENTIFIER;
    else if (isOperator(token->lexeme))
        token->type = OPERATOR;
    else if (strlen(token->lexeme) == 1 &&
             isSpecialCharacter(token->lexeme[0]))
        token->type = SPECIAL_CHARACTER;
    else
        token->type = UNKNOWN;
}

/* Core lexer function */
Token getNextToken()
{
    Token token;
    int ch;
    int i = 0;

    token.lexeme[0] = '\0';
    token.type = UNKNOWN;

    /* Skip whitespace */
    while ((ch = fgetc(sourceFile)) != EOF && isspace(ch));

    if (ch == EOF)
    {
        strcpy(token.lexeme, "EOF");
        token.type = EOF_TOKEN;
        return token;
    }

    /* Identifier / Keyword */
    if (isalpha(ch) || ch == '_')
    {
        while (isalnum(ch) || ch == '_')
        {
            token.lexeme[i++] = ch;
            ch = fgetc(sourceFile);
        }
        token.lexeme[i] = '\0';
        ungetc(ch, sourceFile);
        categorizeToken(&token);
        return token;
    }

    /* Constant */
    if (isdigit(ch))
    {
        while (isdigit(ch))
        {
            token.lexeme[i++] = ch;
            ch = fgetc(sourceFile);
        }
        token.lexeme[i] = '\0';
        ungetc(ch, sourceFile);
        token.type = CONSTANT;
        return token;
    }

    /* Operator */
    if (strchr("+-*/%=<>!", ch))
    {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = OPERATOR;
        return token;
    }

    /* Special Character */
    if (isSpecialCharacter(ch))
    {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = SPECIAL_CHARACTER;
        return token;
    }

    /* Unknown */
    token.lexeme[0] = ch;
    token.lexeme[1] = '\0';
    token.type = UNKNOWN;
    return token;
}


#endif
