#include <iostream>
#include <stdio.h>
using namespace std;
typedef long long llong;

const llong INF = 100000000000000000L;
const int MAX_N = 100011;
llong a[MAX_N];
llong S[MAX_N];
int n, k;

inline llong sum(int L, int R)
{
    return S[R] - S[L - 1];
}

inline llong cost(int L, int R)
{
    return sum(L, R) * (llong)(R - L + 1);
}

llong doubleSplit(int L, int R)
{
    int l = L, r = R - 2, mid, best = R - 1;
    llong bestCost = cost(L, best) + cost(best + 1, R);

    while(l <= r)
    {
        int fthird = l + (r - l) / 3;
        int sthird = l + 2 * (r - l) / 3;

        llong fcost = cost(L, fthird) + cost(fthird + 1, R);
        llong scost = cost(L, sthird) + cost(sthird + 1, R);

        bestCost = min(bestCost, fcost);
        bestCost = min(bestCost, scost);

        if (fcost >= scost)
            l = fthird + 1;
        else
            r = sthird - 1;
    }

    return bestCost;
}

int main()
{
    int i, j, in;

    scanf("%d %d", &n, &k);

    S[0] = 0;
    for (i=1;i<=n;i++)
    {
        scanf("%lld", &a[i]);
        S[i] = S[i - 1] + a[i];
    }

    llong ans = cost(1, 1) + cost(2, 2) + cost(3, n);
    for (i=1;i<=n-2;i++)
    {
        llong win = cost(1, i) + doubleSplit(i + 1, n);

        ans = min(ans, win);
    }

    printf("%lld\n", ans - sum(1, n));

    return 0;
}
/**
8 3
7 3 8 1 7 4 2 6
- 104
**/
