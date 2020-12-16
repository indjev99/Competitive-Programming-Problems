#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <math.h>
using namespace std;
typedef long long llong;
typedef long double ldouble;

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

ldouble F[MAX_N];
int G[MAX_N];

int deq[MAX_N];
int frnt, bck;

bool eq(ldouble a, ldouble b)
{
    return fabs(a - b) < 1e-10;
}

bool currentlyBetter(int a, int b, int k)
{
    ldouble c1 = F[a] + (ldouble)cost(a+1, k);
    ldouble c2 = F[b] + (ldouble)cost(b+1, k);

    return (!eq(c1, c2) && c1 < c2) || (eq(c1, c2) && G[a] <= G[b]);
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

pair<ldouble, int> solve(ldouble segCost)
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
        F[i] = F[chosen] + (ldouble)cost(chosen + 1, i) + segCost;
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

    ldouble l = 0.0, r = (ldouble)cost(1, n) + 1.0;
    ldouble mid, best;
    pair<ldouble, int> bestResponse;

    while(r - l > 1e-3)
    {
        mid = (l + r) / 2.0;

        pair<ldouble, int> resp = solve(mid);
        if (resp.second >= k)
        {
            bestResponse = resp;
            best = mid;
            l = mid;
        }
        else
        {
            r = mid;
        }
    }

    llong ans = round( bestResponse.first - k * (ldouble)best );

    printf("%lld\n", ans - sum(1, n));

    return 0;
}
