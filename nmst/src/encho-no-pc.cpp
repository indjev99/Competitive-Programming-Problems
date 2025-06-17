#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <algorithm>
using namespace std;
typedef long long llong;

const int MAXM = 400111;
const int MAXN = 100111;
const llong MOD = 1000000007;

int n, m;
pair<int, pair<int, int>> edges[MAXM + 1];

int father[MAXN + 1];
int sz[MAXN + 1];
int byLen[20];

int Find(int ver)
{
	int n = 0;
	while (father[ver] != -1) {
		ver = father[ver];
		n++;
	}
	byLen[n]++;
	return ver;
}

int ways = 0;
int cnt;

inline pair<int, int> getGroups(int idx)
{
    return {Find(edges[idx].second.first), Find(edges[idx].second.second)};
}

vector<int> relevant;
void backtrack(int idx, int cntLeft)
{
    if ((int)relevant.size() - idx < cntLeft)
        return;

    if (cntLeft == 0)
    {
        ways++;
        return;
    }

    backtrack(idx + 1, cntLeft);

    auto [r1, r2] = getGroups(relevant[idx]);
    if (r1 != r2)
    {
        if (sz[r1] > sz[r2])
            swap(r1, r2);

        int rem = father[r1];
        int remSz = sz[r2];

        father[r1] = r2;
        sz[r2] += sz[r1];

        backtrack(idx + 1, cntLeft - 1);

        father[r1] = rem;
        sz[r2] = remSz;
    }
}

int main()
{
    memset(father, -1, sizeof(father));

    scanf("%d %d", &n, &m);

    for (int i = 1; i <= n; i++)
    {
        sz[i] = 1;
    }

    for (int i = 1; i <= m; i++)
    {
        scanf("%d %d %d", &edges[i].second.first, &edges[i].second.second, &edges[i].first);
    }

    sort(edges + 1, edges + 1 + m);

    int cur = 1;

    llong ans = 1;
    while(cur <= m)
    {
        int curR = cur;

        while(curR < m && edges[curR + 1].first == edges[curR].first)
            curR++;

        /// Solving cur - curR

        cnt = 0;
        relevant.clear();
        for (int i = cur; i <= curR; i++)
        {
            auto [r1, r2] = getGroups(i);
            if (r1 != r2)
            {
                relevant.push_back(i);
            }
        }

        vector<pair<int, int>> reverses;
        vector<pair<int, int>> reversesSz;
        for (int i = cur; i <= curR; i++)
        {
            auto [r1, r2] = getGroups(i);

            if (r1 != r2)
            {
                if (sz[r1] > sz[r2])
                    swap(r1, r2);

                cnt++;
                reverses.push_back({r1, father[r1]});
                reversesSz.push_back({r2, sz[r2]});
                father[r1] = r2;
                sz[r2] += sz[r1];
            }
        }
        for (int i = (int)reverses.size() - 1; i >= 0; i--)
        {
            father[ reverses[i].first ] = reverses[i].second;
            sz[ reversesSz[i].first ] = reversesSz[i].second;
        }

        ways = 0;
        backtrack(0, cnt);
        ans *= ways;
        ans %= MOD;

        for (int i = cur; i <= curR; i++)
        {
            auto [r1, r2] = getGroups(i);

            if (r1 != r2)
            {
                if (sz[r1] > sz[r2])
                    swap(r1, r2);

                father[r1] = r2;
                sz[r2] += sz[r1];
            }
        }

        cur = curR + 1;
    }

    printf("%lld\n", ans);

	for(size_t i = 0; i < 20; i++) std::cerr << ' ' << byLen[i];
	std::cerr << '\n';

    return 0;
}
