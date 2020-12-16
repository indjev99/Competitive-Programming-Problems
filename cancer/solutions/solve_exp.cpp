#include <iostream>
#include <stdio.h>
using namespace std;
typedef long long llong;

const llong INF = 100000000000000000L;
const int MAX_N = 1011;
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

llong ans = INF;
void batrak(int ind, int segs, llong curCost)
{
    if (segs == k && ind > n)
    {
        ans = min(ans, curCost);
        return;
    }
    else if (segs == k || ind > n)
        return;

    for (int i = ind; i <= n; i++)
    {
        batrak(i + 1, segs + 1, curCost + cost(ind, i));
    }
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

    batrak(1, 0, 0);

    printf("%lld\n", ans - sum(1, n));

    return 0;
}
/**
7 3
6 3 8 6 1 5 4
**/
