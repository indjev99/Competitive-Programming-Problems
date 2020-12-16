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

llong G[MAX_N];
llong F[MAX_N];

void DnC(int lF, int rF, int lOpt, int rOpt)
{
    if (lF > rF || lOpt > rOpt)
        return;

    int ind = (lF + rF) / 2;
    int i;
    int optInd = -1;

    for (i=lOpt;i<=rOpt;i++)
    {
        if (i >= ind)
            break;

        llong curCost = G[i] + cost(i + 1, ind);
        if (curCost < F[ind])
        {
            F[ind] = curCost;
            optInd = i;
        }
    }

    if (optInd == -1)
    {
        DnC(ind + 1, rF, lOpt, rOpt);
        return;
    }

    DnC(lF, ind - 1, lOpt, optInd);
    DnC(ind + 1, rF, optInd, rOpt);
}

int main()
{
    int i, j;

    scanf("%d %d", &n, &k);

    S[0] = 0;
    for (i=1;i<=n;i++)
    {
        scanf("%lld", &a[i]);
        S[i] = S[i - 1] + a[i];
    }

    G[0] = INF;
    F[0] = INF;
    for (i=1;i<=n;i++)
    {
        G[i] = cost(1, i);
        F[i] = INF;
    }

    for (i=2;i<=k;i++)
    {
        DnC(1, n, 1, n);

        for (j=1;j<=n;j++)
        {
            G[j] = F[j];
            F[j] = INF;
        }
    }

    printf("%lld\n", G[n] - sum(1, n));

    return 0;
}
