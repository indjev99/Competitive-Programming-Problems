#include <iostream>
#include <stdio.h>
using namespace std;
typedef long long llong;

const llong INF = 100000000000000000L;
const int MAX_N = 5011;
llong a[MAX_N];
int n, k;

inline llong sum(int L, int R)
{
    llong sm = 0;

    for (int i = L; i <= R; i++)
    {
        sm += a[i];
    }

    return sm;
}

inline llong cost(int L, int R)
{
    return sum(L, R) * (llong)(R - L + 1);
}

llong F[MAX_N][MAX_N];

int main()
{
    int i, j, in;

    scanf("%d %d", &n, &k);

    for (i=1;i<=n;i++)
    {
        scanf("%lld", &a[i]);
    }

    F[1][0] = INF;
    for (i=1;i<=n;i++)
    {
        F[1][i] = cost(1, i);
    }

    for (i=2;i<=k;i++)
    {
        for (j=1;j<=n;j++)
        {
            F[i][j] = F[i-1][j-1] + cost(j, j);

            for (in=0;in<j;in++)
            {
                F[i][j] = min(F[i][j], F[i-1][in] + cost(in+1, j));
            }
        }
    }

    printf("%lld\n", F[k][n] - sum(1, n));

    return 0;
}