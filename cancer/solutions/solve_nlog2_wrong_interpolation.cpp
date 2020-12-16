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

int deq[MAX_N];
int frnt, bck;

bool currentlyBetter(int a, int b, int k)
{
    llong c1 = F[a] + cost(a+1, k);
    llong c2 = F[b] + cost(b+1, k);

    return c1 < c2 || (c1 == c2 && G[a] <= G[b]);
}

int betterMoment(int a, int b)
{
    int l = b + 1, r = n, best = n + 1, mid;

    while(l <= r)
    {
        mid = (l + r) / 2;

        if (currentlyBetter(b, a, mid))
        {
            best = mid;
            r = mid - 1;
        }
        else
            l = mid + 1;
    }

    return best;
}

pair<llong, int> solve(llong segCost)
{
    int i, j;

    frnt = 1, bck = 0;

    F[0] = 0;
    G[0] = 0;
    frnt = 1;
    bck = 1;
    deq[1] = 0;

    for (i=1;i<=n;i++)
    {
        while(frnt < bck && currentlyBetter(deq[frnt+1], deq[frnt], i))
        {
            frnt++;
        }

        int chosen = deq[frnt];
        F[i] = F[chosen] + cost(chosen + 1, i) + segCost;
        G[i] = G[chosen] + 1;

        while(frnt < bck && betterMoment(deq[bck], i) <= betterMoment(deq[bck-1], deq[bck]))
        {
            bck--;
        }

        bck++;
        deq[bck] = i;
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
    llong mid, best = -1;
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

    assert(best != -1);

    llong ans;
    if (bestResponse.second == k)
        ans = bestResponse.first - best * (llong)bestResponse.second;
    else
    {
        pair<llong, int> prev = solve(best + 1);
        llong realBotVal = bestResponse.first - best * (llong)bestResponse.second;
        llong realTopVal = prev.first - (best + 1) * (llong)prev.second;

        ans = (realBotVal + realTopVal) / 2;
    }

    printf("%lld\n", ans - sum(1, n));

    return 0;
}
