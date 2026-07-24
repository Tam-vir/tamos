#include "math.h"

int kabs(
    int x)
{
    return x < 0 ? -x : x;
}

long klabs(
    long x)
{
    return x < 0 ? -x : x;
}

int kmin(
    int a,
    int b)
{
    return a < b ? a : b;
}

int kmax(
    int a,
    int b)
{
    return a > b ? a : b;
}

int kpow_int(
    int base,
    int exp)
{
    int result = 1;

    while (exp > 0)
    {
        result *= base;
        exp--;
    }

    return result;
}
