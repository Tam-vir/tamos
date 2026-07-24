#include "ctype.h"

int kisdigit(char c)
{
    return (c >= '0' &&
            c <= '9');
}

int kisalpha(char c)
{
    return ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z'));
}

int kisalnum(char c)
{
    return (kisdigit(c) ||
            kisalpha(c));
}

int kislower(char c)
{
    return (c >= 'a' &&
            c <= 'z');
}

int kisupper(char c)
{
    return (c >= 'A' &&
            c <= 'Z');
}

char ktolower(char c)
{
    if (kisupper(c))
    {
        return c + 32;
    }

    return c;
}

char ktoupper(char c)
{
    if (kislower(c))
    {
        return c - 32;
    }

    return c;
}