#include <iostream>
#include <stdio.h>
#include <assert.h>
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

llong F[MAX_N];
int G[MAX_N];

pair<llong, int> solve(llong segCost)
{
    int i, j;

    F[0] = 0;
    G[0] = 0;
    for (i=1;i<=n;i++)
    {
        F[i] = cost(1, i) + segCost;
        G[i] = 1;
        for (j=0;j<i;j++)
        {
            llong newCost = F[j] + cost(j+1, i) + segCost;

            if (newCost < F[i] || (newCost == F[i] && G[j] + 1 < G[i]))
            {
                F[i] = newCost;
                G[i] = G[j] + 1;
            }
        }
    }

    return make_pair(F[n], G[n]);
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

    llong l = 0, r = cost(1, n) + 1;
    llong mid, best;
    pair<llong, int> bestResponse;

    while(l <= r)
    {
        mid = (l + r) / 2;

        pair<llong, int> resp = solve(mid);
        if (resp.second >= k)
        {
            bestResponse = resp;
            best = mid;
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }

    llong ans;
    if (bestResponse.second == k)
        ans = bestResponse.first - best * (llong)bestResponse.second;
    else
    {
        pair<llong, int> prev = solve(best + 1);
        llong realBotVal = bestResponse.first - best * (llong)bestResponse.second;
        llong realTopVal = prev.first - (best + 1) * (llong)prev.second;

        assert( (realTopVal - realBotVal) % (bestResponse.second - prev.second) == 0 );
        ans = realBotVal + ( (realTopVal - realBotVal) / (bestResponse.second - prev.second) ) * (bestResponse.second - k);
    }

    printf("%lld\n", ans - sum(1, n));

    return 0;
}
