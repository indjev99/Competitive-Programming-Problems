#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>
#include <random>
#include <cassert>

std::mt19937 generator;

long long randInt(long long lb, long long ub)
{
    assert(ub > lb);
    std::uniform_int_distribution<long long> distribution(lb, ub - 1);
    return distribution(generator);
}

long long randInt(long long ub)
{
    return randInt(0, ub);
}

double randReal(double lb = 0, double ub = 1)
{
    std::uniform_real_distribution<double> distribution(lb, ub);
    return distribution(generator);
}

int n, m;
double p;
double penFair, penUnfair;
int k;

struct Die
{
    int idx;
    std::vector<int> counts;
    bool fair;
    double score;
};

std::vector<Die> dice;

void input()
{
    std::cin >> n >> m >> p >> penFair >> penUnfair >> k;

    dice.resize(k);

    for (int i = 0; i < k; ++i)
    {
        dice[i].idx = i;
        dice[i].counts.resize(n);

        for (int j = 0; j < n; ++j)
        {
            std::cin >> dice[i].counts[j];
        }
    }
}

void output()
{
    std::sort(dice.begin(), dice.end(), [](auto& left, auto& right){ return left.idx < right.idx; });
    for (int i = 0; i < k; ++i)
    {
        std::cout << dice[i].fair << "\n";
    }
}

std::vector<int> countCounts;

void computePVals()
{
    const int TRIALS = 2e5;

    countCounts.resize(m + 1, 0);

    for (int i = 0; i < TRIALS; ++i)
    {
        std::vector<int> counts(n, 0);
        for (int j = 0; j < m; ++j)
        {
            ++counts[randInt(n)];
        }
        for (int j = 0; j < n; ++j)
        {
            countCounts[counts[j]] += 1;
        }
    }
}

void giveScore(Die& die)
{
    die.score = 1;
    for (int i = 0; i < n; ++i)
    {
        die.score *= countCounts[die.counts[i]];
    }
}

void solve()
{
    computePVals();

    for (int i = 0; i < k; ++i)
    {
        giveScore(dice[i]);
    }

    std::sort(dice.begin(), dice.end(), [](auto& left, auto& right){ return left.score > right.score; });

    double fairWeight = p * penFair;
    double unfairWeight = (1 - p) * penUnfair;
    double fariRatio = fairWeight / (fairWeight + unfairWeight);

    for (int i = 0; i < k; ++i)
    {
        dice[i].fair = i < std::round(k * fariRatio);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    generator.seed(0);

    input();
    solve();
    output();

    return 0;
}
