#ifndef ERROR_MSG_H
#define ERROR_MSG_H

#include <stdio.h>
#include <stdlib.h>

class Error
{
public:
    static void FileErr(const char msg[]);
    static void SyntaxErr(const char msg[]);
    static void RuntimeErr(const char msg[]);
};

void Error::FileErr(const char msg[])
{
    printf(msg);
    printf("\n");

    exit(1);
}

void Error::SyntaxErr(const char msg[])
{
    printf("Syntax error:\n");
    printf("[%s]", msg);
    printf("\n");

    exit(1);
}

void Error::RuntimeErr(const char msg[])
{
    printf("Runtime error:\n");
    printf("[%s]", msg);
    printf("\n");
    exit(1);
}

#endif