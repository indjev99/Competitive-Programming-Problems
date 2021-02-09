#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;

struct chunk
{
    int l, r;
    int val;
};

int n, m;

vector<chunk> chunks;
vector< pair<int, int> > events;

int main()
{
    int i, j;

    scanf("%d %d", &n, &m);

    for (i=1;i<=n;i++)
    {
        int a, b;

        scanf("%d %d", &a, &b);

        b -= m - 1;

        if (a <= b)
        {
            events.push_back(make_pair(a, 1));
            events.push_back(make_pair(b, -1));
        }
    }

    sort(events.begin(), events.end());

    chunk cc;
    cc.l = 1;
    cc.val = 0;
    for (i=0;i<events.size();i++)
    {
        if (events[i].first == cc.l)
        {
            cc.val += events[i].second;
        }
        else
        {
            cc.r = events[i].first - 1;
            chunks.push_back(cc);

            cc.l = events[i].first;
            cc.val += events[i].second;
        }
    }

    cc.r = 100000001;
    chunks.push_back(cc);

    int lagMax = 0;
    int lagPtr = 0;
    int ans = 0;

    for (i=0;i<chunks.size();i++)
    {
        while(lagPtr <= i && chunks[i].r - chunks[lagPtr].l >= m)
        {
            lagMax = max(lagMax, chunks[lagPtr].val);
            lagPtr++;
        }

        ans = max(ans, chunks[i].val + lagMax);
    }

    printf("%d\n", ans);

    return 0;
}
