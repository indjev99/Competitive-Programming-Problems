#include <iostream>
#include <vector>
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

vector< pair<int, int> > segs;
llong eval()
{
    int i;
    llong ans = 0;

    for (i=0;i<segs.size();i++)
    {
        ans += cost(segs[i].first, segs[i].second);
    }

    return ans;
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

    int segSz = n / k;

    for (i=1;i<=k;i++)
    {
        segs.push_back(make_pair(i, i));
    }
    segs.back().second = n;

    bool improv = true;
    while(improv)
    {
        improv = false;

        for (i=1;i<segs.size();i++)
        {
            pair<int,int> L = segs[i-1];
            pair<int,int> R = segs[i];

            while(L.first < L.second)
            {
                if (cost(L.first, L.second) + cost(R.first, R.second) > cost(L.first, L.second-1) + cost(R.first-1, R.second))
                {
                    L.second--;
                    R.first--;
                    improv = true;
                }
                else
                    break;
            }

            while(R.first < R.second)
            {
                if (cost(L.first, L.second) + cost(R.first, R.second) > cost(L.first, L.second+1) + cost(R.first+1, R.second))
                {
                    L.second++;
                    R.first++;
                    improv = true;
                }
                else
                    break;
            }

            segs[i-1] = L;
            segs[i] = R;
        }
    }

    printf("%lld\n", eval() - sum(1, n));

    return 0;
}
