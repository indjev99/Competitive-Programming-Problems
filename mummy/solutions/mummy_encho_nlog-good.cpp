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
bool isLocked[MAXN];
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

vector<pair<vector<int>, int>> unsolved;
vector<vector<int>> solved;
void preprocess(const vector<int>& p, int ans)
{
    int n = p.size();

    int explained = 0;
    for (int i = 0; i < n; i++)
    {
        if (isLocked[ p[i] ] && lockedIn[ p[i] ] == i)
        {
            explained++;
        }
    }

    //printf("%d / %d explained\n", explained, ans);
    assert(explained <= ans);

    if (ans != explained)
        unsolved.push_back({p, ans - explained});
    else
        solved.push_back(p);
}

void refreshInfo(int val, int pos)
{
    //printf("Refreshing because %d is at position %d\n", val, pos);

    int i = 0;
    while(i < unsolved.size())
    {
        vector<int>& p = unsolved[i].first;

        if (p[pos] == val)
            unsolved[i].second--;

        if (unsolved[i].second == 0)
        {
            solved.push_back(p);
            swap(unsolved[i], unsolved.back());
            unsolved.pop_back();
        }
        else
            i++;
    }
}

void process(const vector<int>& p)
{
    /*printf("Solved: ");
    for (auto x : p)
    {
        printf("%d ", x);
    }
    printf("\n");*/

    for (int i = 0; i < p.size(); i++)
    {
        if (!banned[ p[i] ][i] && !isLocked[ p[i] ])
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
                        isLocked[ p[i] ] = true;

                        refreshInfo(p[i], j);
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
    memset(isLocked, false, sizeof(isLocked));
    for (int i = 0; i < n; i++)
    {
        options[i] = n;
    }
    lockedIns = 0;
    unsolved.clear();

    int iterations = 0;
    while(lockedIns < n)
    {
        iterations++;
        assert(iterations <= 100);

        vector<int> p(n);
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), mt);

        //printf("\nMade permutation\n");
        for (int i = 0; i < n; i++)
        {
            /*for (auto x : p)
            {
                printf("%d ", x);
            }
            printf("\n");*/

            preprocess(p, numMatches(p));

            while(!solved.empty())
            {
                auto P = solved.back();
                solved.pop_back();
                process(P);
            }

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

/**
vector<int> HIDDEN;
int score = 0;
int numMatches(const vector<int>& p)
{
    score++;

    int ans = 0;
    for (int i = 0; i < p.size(); i++)
    {
        if (p[i] == HIDDEN[i])
            ans++;
    }
    return ans;
}

int main()
{
    auto SEEDS = {3452, 4890, 2324, 8201, 7449, 6832, 1900, 9001, 4210, 5533, 1091, 9431, 5021, 1252, 8927, 7421, 9629, 7249, 5135, 2100, 1742, 1066, 6739, 2964, 1042};
    const int n = 300;
    static std::mt19937 rng;


    for (auto seed : SEEDS)
    {
        //printf("Running test\n");

        HIDDEN.resize(n);
        iota(HIDDEN.begin(), HIDDEN.end(), 0);

        rng.seed(seed);
        std::shuffle(HIDDEN.begin(), HIDDEN.end(), rng);

        score = 0;
        findPerm(n);
        printf("Score: %d\n", score);
    }
}
**/
