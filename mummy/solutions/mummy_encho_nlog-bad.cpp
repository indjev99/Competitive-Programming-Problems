#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <numeric>
#include <string.h>
#include <random>
using namespace std;

int numMatches(const vector<int>& p);

const int MAXN = 512;
bool banned[MAXN][MAXN];
int options[MAXN];
int lockedIn[MAXN];
int lockedIns = 0;

void shift(vector<int>& p)
{
    int fst = p[0];
    for (int i = 0; i < int(p.size()) - 1; i++)
    {
        p[i] = p[i + 1];
    }
    p[ p.size() - 1 ] = fst;
}

void process(vector<int> p, int ans)
{
    if (ans == 0)
    {
        for (int i = 0; i < p.size(); i++)
        {
            if (!banned[ p[i] ][i])
            {
                banned[ p[i] ][i] = true;
                options[ p[i] ]--;
                assert(options[ p[i] ] >= 1);

                if (options[ p[i] ] == 1)
                {
                    for (int j = 0; j < p.size(); j++)
                    {
                        if (!banned[ p[i] ][j])
                        {
                            lockedIn[ p[i] ] = j;
                            lockedIns++;
                        }
                    }
                }
            }
        }
    }
}

mt19937 mt(1337);

vector<int> findPerm(int n)
{
    memset(banned, false, sizeof(banned));
    for (int i = 0; i < n; i++)
    {
        options[i] = n;
    }
    lockedIns = 0;

    int iterations = 0;
    while(lockedIns < n)
    {
        iterations++;
        assert(iterations <= 100);

        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), mt);

        for (int i = 0; i < n; i++)
        {
            process(p, numMatches(p));
            shift(p);

            if (lockedIns == n)
                break;
        }
    }

    vector<int> answer(n);

    for (int i = 0; i < n; i++)
    {
        answer[ lockedIn[i] ] = i;
    }

    return answer;
}
