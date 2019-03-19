#include "transmission.h"
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <algorithm>
#include <vector>
#include <random>
using namespace std;
typedef long long llong;

static mt19937 mt;
static const int BASE = 10;
static int CHECK = 5;
static const int G = 10000 / BASE;

static double fact[1011];

static void doMaths()
{
    int i;

    fact[0] = 0LL;

    for (i=1;i<=1000;i++)
    {
        fact[i] = fact[i-1] + log(i);
    }
}

static double dropChance(int k,int E)
{
    if (BASE+CHECK == k)
        return 0.0;

    double ans = (fact[E] - fact[k] - fact[E-k]) + k * (-log(G)) + log(G-1) * (E-k) - log(G) * (E-k);

    return ans;
}

static vector<bool> sanitize(vector<bool> data,int n)
{
    int mod = 0;

    while(data.size() < n)
    {
        mod++;
        data.push_back(true);
    }

    while(data.size() > n)
    {
        mod++;
        data.pop_back();
    }

    if (mod != 0)
    {
        fprintf(stderr,"Sanitization modified %d bits\n",mod);
    }

    return data;
}

static const int MAX_CHECK = 7;
static int translate[BASE+MAX_CHECK+1][(1<<(BASE+MAX_CHECK))];
static int total = 0;
static int suffix[(1<<BASE)];

static double F[15011][1001];
static int choice[15011][1001];

static double solve(int ind, int curg, int d, const vector<bool>& message)
{
    if (ind == -1)
        return 0.0;
    if (curg <= 0)
        return log(0.0);
    if (curg * (BASE+CHECK) < ind+1)
        return log(0.0);

    if (choice[ind][curg] != -1)
        return F[ind][curg];

    int i;
    int val = 0;

    double ch = solve(ind, curg-1, d, message) + dropChance(CHECK+BASE, d);

    choice[ind][curg] = ind+1;
    F[ind][curg] = ch;

    for (i=ind;i>=ind-BASE-CHECK+1 && i>=0;i--)
    {
        val *= 2;
        if (message[i])
            val++;

        double chance;
        if (ind - i + 1 >= BASE && translate[ind - i + 1][val] == 0 && suffix[0] != 0)
            chance = log(0.0);
        else
            chance = solve(i-1, curg-1, d, message) + dropChance(CHECK+BASE-(ind-i+1), d);

        if (choice[ind][curg] == -1 || F[ind][curg] < chance)
        {
            choice[ind][curg] = i;
            F[ind][curg] = chance;
        }
    }

    return F[ind][curg];
}

static int getBit(int mask,int ind)
{
    if ( (mask & (1<<ind)) > 0 )
        return 1;
    else
        return 0;
}

static void claim(int orig,int cnt,int seq)
{
    translate[cnt][seq] = orig;
}

static void breakDown(int mask, int builtCnt, int corrupted, int ind)
{
    if ( (BASE+CHECK-ind) + builtCnt < BASE )
        return;

    if (ind == BASE+CHECK)
    {
        claim(mask, builtCnt, corrupted);
        return;
    }

    breakDown(mask, builtCnt, corrupted, ind+1);

    breakDown(mask, builtCnt+1, corrupted | ((1<<builtCnt) * getBit(mask, ind)), ind+1);
}

static void preprocess()
{
    memset(translate,0,sizeof(translate));

    doMaths();

    int i;

    for (i=0;i<(1<<BASE);i++)
    {
        suffix[i] = mt() % (1<<CHECK);

        if (i == 0 && suffix[i] == 0)
        {
            i--;
            continue;
        }

        if (i == (1<<BASE)-1 && suffix[i] == (1<<CHECK)-1)
        {
            i--;
            continue;
        }
    }

    for (i=0;i<(1<<BASE);i++)
    {
        breakDown(i | ((1<<BASE) * suffix[i]), 0, 0, 0);
    }
}

static vector< pair<int,int> > segments;
vector<bool> receive(const vector<bool>& message, int n, int d)
{
    mt19937 z(13137);

    mt = z;

    if (d == 1)
        CHECK = 1;
    else if (d < 300)
        CHECK = 1;
    else
        CHECK = 5;

    preprocess();

    int i,j;
    vector<bool> ans;

    memset(choice,-1,sizeof(choice));

    double chance = solve((int)message.size() - 1, n / BASE, d, message);

    int cur = (int)message.size() - 1;
    int curg = n / BASE;

    segments.clear();
    while(cur != -1)
    {
        segments.push_back(make_pair(choice[cur][curg], cur));

        cur = choice[cur][curg]-1;
        curg--;
    }
    reverse(segments.begin(),segments.end());

    for (i=0;i<segments.size();i++)
    {
        int L = segments[i].first;
        int R = segments[i].second;
        int val = 0;
        int len = R - L + 1;

        for (j=R;j>=L;j--)
        {
            val *= 2LL;
            if (message[j])
                val++;
        }

        int decoded;
        if (len >= BASE)
            decoded = translate[len][val];
        else
            decoded = val;

        for (j=0;j<BASE;j++)
        {
            ans.push_back( (decoded & (1<<j)) > 0 );
        }
    }

    return sanitize(ans,n);
}
