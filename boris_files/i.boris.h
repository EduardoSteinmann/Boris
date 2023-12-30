#ifndef IBORIS_H
#define IBORIS_H
#include <stdio.h>
#include <stdbool.h>
typedef int NUMBER;
typedef enum Test Test;
typedef struct StrLit StrLit;
typedef struct Foo Foo;
typedef union Baz Baz;
typedef struct K K;
enum Test
{
    ONE,
    ZERO,
    CONFUSING,
};
struct StrLit
{
    const char* str_lit;
};
struct Foo
{
    const char* bar;
    Foo* next;
    union Baz
    {
        int f;
        struct K
        {
            int d;
        };
    };
};
void    print(const char* s)     ;
void    hello_world()     ;
extern int ml ;
#endif
