// prime_algorithms.c : two distinct algorithms to verify prime numbers
// by nicholas raffone and juan piñeros

#include "prime_algorithms.h"

int prime_algo1(int n)
{
    int i;
    if (n == 1)
        return (NO);
    for (i = 2; i < n; i++)
        if (n % i == 0)
            return (NO);
    return (YES);
}

int prime_algo2(int n)
{
    int i = 0, limitup = 0;
    limitup = (int)(sqrt((float)n));
    if (n == 1)
        return (NO);
    for (i = 2; i <= limitup; i++)
        if (n % i == 0)
            return (NO);
    return (YES);
}