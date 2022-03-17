#include "gcd.h"

ull play(ull MAX_X)
{
    for (ull rem = 0; rem < MAX_X; ++rem)
    {
        ull res = query(MAX_X - rem, MAX_X);
        if (res == MAX_X) return rem;
    }
    return 0;
}
