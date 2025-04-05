#include <iostream>
#include <vector>
#include <stdio.h>
#include <assert.h>
using namespace std;
typedef long long llong;

int n, m;
vector<int> groups[100111];
const int K = 800;
vector<int> bigPrec[100111];
int F[100111];
int Key = 1;

int main()
{
    scanf("%d %d", &n, &m);

    for (int i = 1; i <= m; i++)
    {
        int k;
        scanf("%d", &k);

        for (int j = 1; j <= k; j++)
        {
            int a;
            scanf("%d", &a);
            groups[a].push_back(i);
        }
    }

    for (int i = 1; i <= n; i++)
    {
        if (groups[i].size() > K)
        {
            Key++;
            for (auto x : groups[i])
            {
                F[x] = Key;
                //F[x] = true;
            }

            bigPrec[i].resize(n, 0);
            for (int j = 1; j <= n; j++)
            {
                if (!bigPrec[j].empty())
                {
                    bigPrec[i][j] = bigPrec[j][i];
                    continue;
                }

                for (auto x : groups[j])
                {
                    if (F[x] == Key)
                        bigPrec[i][j] += x;
                }
            }
            Key++;

            /*for (auto x : groups[i])
            {
                F[x] = false;
            }*/
        }
    }

    int q;

    scanf("%d", &q);

    for (int i = 1; i <= q; i++)
    {
        int a, b;

        scanf("%d %d", &a, &b);

        if (!bigPrec[a].empty())
            printf("%d\n", bigPrec[a][b]);
        else if (!bigPrec[b].empty())
            printf("%d\n", bigPrec[b][a]);
        else
        {
            if (groups[a].size() > groups[b].size())
                swap(a, b);

            Key++;
            for (auto x : groups[a])
            {
                F[x] = Key;
                //F[x] = true;
            }

            int ans = 0;
            for (auto x : groups[b])
            {
                if (F[x] == Key)
                    ans += x;
            }

            /*for (auto x : groups[a])
            {
                F[x] = false;
            }*/
            Key++;

            printf("%d\n", ans);
        }
    }

    return 0;
}
