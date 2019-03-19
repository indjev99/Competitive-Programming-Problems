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

static int suffix[(1<<BASE)];

static int getBit(int mask,int ind)
{
    if ( (mask & (1<<ind)) > 0 )
        return 1;
    else
        return 0;
}

static const int MAX_CHECK = 7;

static int translate[BASE+MAX_CHECK+1][(1<<(BASE+MAX_CHECK))];
static int total = 0;

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

vector<bool> transmit(const vector<bool>& data, int d)
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

    int cur = 0;
    int i;
    vector<bool> ans;

    while(cur < data.size())
    {
        int val = 0;
        for (i=cur;i<=cur+BASE-1;i++)
        {
            if (i < data.size())
            {
                if (data[i])
                {
                    val |= (1<<(i-cur));
                }
            }
        }

        val = val | ((1<<BASE) * suffix[val]);

        for (i=0;i<BASE+CHECK;i++)
        {
            ans.push_back( (val & (1<<i)) > 0 );
        }

        cur += BASE;
    }

    return ans;
}
