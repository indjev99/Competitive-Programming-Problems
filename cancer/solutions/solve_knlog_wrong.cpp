#include <iostream>
#include <stdio.h>
using namespace std;
typedef long long llong;

const llong INF = 100000000000000000L;
const int MAX_N = 5011;
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

llong F[MAX_N][MAX_N];

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

            int l = 0, r = j, mid, best;

            while(l <= r)
            {
                int ft = l + (r - l) / 3;
                int st = l + 2 * (r - l) / 3;

                llong cft = F[i-1][ft] + cost(ft + 1, j);
                llong cst = F[i-1][st] + cost(st + 1, j);

                F[i][j] = min(F[i][j], cft);
                F[i][j] = min(F[i][j], cst);

                if (cft >= cst)
                    l = ft + 1;
                else
                    r = st - 1;
            }
        }
    }

    printf("%lld\n", F[k][n] - sum(1, n));

    return 0;
}