#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <string.h>
#include <vector>
using namespace std;
typedef long long llong;

const int MAXN = 4000111;
int n;
char str[MAXN + 1];

int S[MAXN + 1];
int prevBigger[MAXN + 1];
int nextBigger[MAXN + 1];
bool archived[MAXN + 1];

vector<int> groups[MAXN + 1];

int gIdCtr = 0;

int theArchive[MAXN + 1];

vector<int> unarchivedGroups;

void archive(int gid)
{
    //printf("Archiving %d\n", gid);
    assert(!archived[gid]);

    assert(unarchivedGroups.back() == gid);
    unarchivedGroups.pop_back();

    int val = 0;
    for (int i = int(groups[gid].size()) - 1; i >= 0; i--)
    {
        theArchive[val] += groups[gid][i];
        val++;
    }
    archived[gid] = true;
}

void unarchive(int gid)
{
    //printf("Unarchiving %d\n", gid);

    assert(archived[gid]);
    unarchivedGroups.push_back(gid);

    int val = 0;
    for (int i = int(groups[gid].size()) - 1; i >= 0; i--)
    {
        theArchive[val] -= groups[gid][i];
        val++;
    }
    archived[gid] = false;
}

int mergeGroups(int id1, int id2)
{
    int s1 = groups[id1].size();
    int s2 = groups[id2].size();

    if (s1 < s2)
    {
        swap(s1, s2);
        swap(id1, id2);
    }

    for (int i = 0; i < s2; i++)
    {
        groups[id1][s1 - i - 1] += groups[id2][s2 - i - 1];
    }

    if (unarchivedGroups.back() == id2)
    {
        unarchivedGroups.pop_back();
    }
    else
    {
        assert(unarchivedGroups[ unarchivedGroups.size() - 2 ] == id2);
        swap(unarchivedGroups[ unarchivedGroups.size() - 2 ], unarchivedGroups[ unarchivedGroups.size() - 1 ]);
        unarchivedGroups.pop_back();
    }

    return id1;
}

inline int countInGroup(int gid, int num)
{
    if (num < 0 || num >= groups[gid].size())
        return 0;
    else
        return groups[gid][ groups[gid].size() - num - 1 ];
}

inline int countArchive(int num)
{
    if (num < 0 || num > n)
        return 0;
    return theArchive[num];
}

int fetchGlobal(int num)
{
    int ans = countArchive(num);
    for (auto gid : unarchivedGroups)
    {
        ans += countInGroup(gid, num);
    }
    return ans;
}

int myGroup[MAXN + 1];

llong solve()
{
    llong ans = 0;

    gIdCtr = 1;
    groups[1].push_back(1);
    unarchivedGroups.push_back(1);
    archive(1);
    myGroup[n] = 1;

    int curQuery = S[n - 1];
    int curAns = 1;
    ans = 1;
    for (int i = n - 1; i >= 1; i--)
    {
        //printf("\nSolving %d\n", i);

        if (S[i] == S[i + 1] - 1) /// New group case
        {
            gIdCtr++;
            groups[gIdCtr].push_back(1);
            unarchivedGroups.push_back(gIdCtr);
            myGroup[i] = gIdCtr;
            curAns++;

            //printf("New group %d causes answer %d\n", gIdCtr, curAns);
        }
        else
        {
            assert(S[i] == S[i + 1] + 1);
            myGroup[i] = myGroup[i + 1];
            int gid = myGroup[i];

            if (archived[gid])
                unarchive(gid);

            //printf("Removing %d because group %d is shifting, but adding 1\n", countInGroup(gid, curQuery), gid);
            curAns -= countInGroup(gid, curQuery);
            curAns++;
            //printf("The current answer is now %d\n", curAns);

            groups[gid].push_back(1);

            //printf("Next bigger is %d\n", nextBigger[i]);
            if (nextBigger[i] != -1) /// Merge
            {
                int otherGid = myGroup[ nextBigger[i] ];

                if (archived[otherGid])
                    unarchive(otherGid);

                myGroup[i] = mergeGroups(gid, otherGid);
                //printf("Merged %d and %d into %d\n", gid, otherGid, myGroup[i]);
                gid = myGroup[i];
            }
        }

        int gid = myGroup[i];
        if (!archived[gid] && i - prevBigger[i] > groups[gid].size())
            archive(gid);

        /*printf("All values are:\n");

        for (int i = 0; i <= n; i++)
        {
            printf("cnt_%d = %d\n", i, fetchGlobal(i));
        }

        for (int i = 0; i < n; i++)
        {
            printf("Archive_%d = %d\n", i, theArchive[i]);
        }*/

        int newQuery = S[i - 1];
        if (newQuery > curQuery)
            curAns += fetchGlobal(newQuery);
        else
            curAns -= fetchGlobal(curQuery);

        //printf("Query changes %d->%d\n", curQuery, newQuery);
        //printf("Adjusted answer = %d\n", curAns);

        curQuery = newQuery;
        //printf("+%d from %d\n", curAns, i);
        ans += curAns;
    }

    return ans;
}

llong solveCase()
{
    for (int i = 0; i <= n; i++)
    {
        archived[i] = false;
        groups[i].clear();
    }

    S[0] = 0;
    for (int i = 1; i <= n; i++)
    {
        S[i] = S[i - 1];
        if (str[i] == '(')
            S[i]++;
        else
            S[i]--;

        assert(S[i] >= 0);
    }
    assert(S[n] == 0);

    vector< pair<int, int> > stk;
    for (int i = n; i >= 1; i--)
    {
        while(!stk.empty() && stk.back().first <= S[i])
        {

            prevBigger[ stk.back().second ] = i;
            stk.pop_back();
        }

        stk.push_back({S[i], i});
    }
    while(!stk.empty())
    {
        prevBigger[ stk.back().second ] = -1;
        stk.pop_back();
    }

    for (int i = 1; i <= n; i++)
    {
        while (!stk.empty() && stk.back().first <= S[i])
        {
            nextBigger[ stk.back().second ] = i;
            stk.pop_back();
        }
        stk.push_back({S[i], i});
    }
    while(!stk.empty())
    {
        nextBigger[ stk.back().second ] = -1;
        stk.pop_back();
    }

    /*for (int i = 1; i <= n; i++)
    {
        printf("%d ", S[i]);
    }
    printf("\n");
    for (int i = 1; i <= n; i++)
    {
        printf("%d | Prev: %d | Next: %d\n", i, prevBigger[i], nextBigger[i]);
    }*/

    return solve();
}

int main()
{
    //freopen("brackets.01.in", "r", stdin);

    scanf("%d", &n);
    scanf("%s", str + 1);
    assert(n == strlen(str + 1));

    printf("%lld\n", solveCase());
}
