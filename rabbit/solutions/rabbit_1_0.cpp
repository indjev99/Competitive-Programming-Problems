#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
using namespace std;

int n, S, L;

int pull(int x, int sh)
{
    if (x < sh)
        return x + 1;
    else if (x > sh)
        return x - 1;
    else
        return x;
}

int push(int x, int sh)
{
    if (x < sh)
        return max(1, x - 1);
    else if (x > sh)
        return min(n, x + 1);
    else
        return x;
}

bool exists(pair<int, int> rng)
{
    return rng.first <= rng.second;
}

pair<int, int> lureRange(pair<int, int> rng, int shot)
{
    if (!exists(rng))
        return rng;
    if (rng.first == rng.second && shot == rng.first)
        return make_pair(0, -1);
    else
        return make_pair(pull(rng.first, shot), pull(rng.second, shot));
}

pair<int, int> scareRange(pair<int, int> rng, int shot)
{
    if (!exists(rng))
        return rng;
    if (rng.first == rng.second && (shot == rng.first || shot == rng.second))
        return make_pair(0, -1);
    if (shot > rng.first && shot < rng.second)
        throw "Don't shoot in ranges";

    if (shot == rng.first)
        return make_pair(push(rng.first+1, shot), push(rng.second, shot));
    else if (shot == rng.second)
        return make_pair(push(rng.first, shot), push(rng.second - 1, shot));
    else
        return make_pair(push(rng.first, shot), push(rng.second, shot));
}

vector<int> solve()
{
    int i;
    vector<int> ans;

    if (L >= S)
    {
        pair<int, int> active = make_pair(1, n);
        while(exists(active))
        {
            for (i=1;i<=S;i++)
            {
                if (!exists(active))
                    break;

                ans.push_back(active.first);
                active = scareRange(active, ans.back());
            }

            int choice = (active.first + active.second) / 2;
            for (i=1;i<=L;i++)
            {
                if (!exists(active))
                    break;

                ans.push_back(choice);
                active = lureRange(active, ans.back());
            }
        }
    }
    else
    {
        int magicSpot = round((double)n * (double)(S - L) / (double)(3 * S)) + 1;

        pair<int, int> rightRange = make_pair(magicSpot, n);
        pair<int, int> leftRange = make_pair(1, magicSpot - 1);

        int state = 0;
        while(exists(rightRange))
        {
            if (state < S)
            {
                ans.push_back(rightRange.first);
                rightRange = scareRange(rightRange, ans.back());
                leftRange = scareRange(leftRange, ans.back());
            }
            else
            {
                ans.push_back(n);
                rightRange = lureRange(rightRange, ans.back());
                leftRange = lureRange(leftRange, ans.back());
            }

            state = (state + 1) % (S + L);
        }

        while(exists(leftRange))
        {
            if (state < S)
            {
                ans.push_back(leftRange.second);
                leftRange = scareRange(leftRange, ans.back());
            }
            else
            {
                ans.push_back(leftRange.first);
                leftRange = lureRange(leftRange, ans.back());
            }

            state = (state + 1) % (S + L);
        }
    }

    return ans;
}

int main()
{
    int i, j;

    scanf("%d %d %d", &n, &S, &L);

    if (S != 1 || L != 0)
        return 0;

    vector<int> ans = solve();

    printf("%d\n", ans.size());

    for (i=0;i<ans.size();i++)
    {
        if (i != 0)
            printf(" ");

        printf("%d", ans[i]);
    }
    printf("\n");

    return 0;
}
