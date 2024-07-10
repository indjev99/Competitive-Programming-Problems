#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>
#include <random>
#include <cassert>
#include <unordered_map>

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
    double likelihood;
    bool fair;
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

long long getOrdCode(const std::vector<int>& countsSorted)
{
    long long code = 0;
    for (int count : countsSorted)
    {
        code = code * (m + 1) + count;
    }
    return code;
}

struct OrdCodeData
{
    int fairCountsCount = 0;
    int unfairCountsCount = 0;
};

std::unordered_map<long long, OrdCodeData> ordCodeToData;

void sampleFair()
{
    std::vector<int> counts(n, 0);
    for (int i = 0; i < m; ++i)
    {
        ++counts[randInt(n)];
    }

    std::sort(counts.begin(), counts.end());
    ++ordCodeToData[getOrdCode(counts)].fairCountsCount;
}

void sampleUnfair()
{
    std::vector<double> cumuls(n);
    double prev = 0;
    for (int i = 0; i < n; ++i)
    {
        cumuls[i] = prev + randReal();
        prev = cumuls[i];
    }
    for (int i = 0; i < n; ++i)
    {
        cumuls[i] /= prev;
    }

    std::vector<int> counts(n, 0);
    for (int i = 0; i < m; ++i)
    {
        bool found = false;
        double rollVar = randReal();
        for (int j = 0; j < n; ++j)
        {
            if (rollVar < cumuls[j])
            {
                ++counts[j];
                found = true;
                break;
            }
        }

        if (!found) --i;
    }

    std::sort(counts.begin(), counts.end());
    ++ordCodeToData[getOrdCode(counts)].unfairCountsCount;
}

void computeProbs()
{
    const int TRIALS = 2e5;

    for (int i = 0; i < TRIALS; ++i)
    {
        sampleFair();
        sampleUnfair();
    }
}

double avgLikelihood = 0;

void computeLikelihood(Die& die)
{
    std::sort(die.counts.begin(), die.counts.end());
    const auto& data = ordCodeToData[getOrdCode(die.counts)];

    if (data.fairCountsCount == 0)
    {
        die.likelihood = 0;
        return;
    }

    die.likelihood = static_cast<double>(data.fairCountsCount) / (data.fairCountsCount + data.unfairCountsCount);
}

void decideFair(Die& die)
{
    double prob = die.likelihood * p / avgLikelihood;

    die.fair = prob * penFair > (1 - prob) * penUnfair;
}

void solve()
{
    computeProbs();

    for (int i = 0; i < k; ++i)
    {
        computeLikelihood(dice[i]);
        avgLikelihood += dice[i].likelihood / k;
    }

    for (int i = 0; i < k; ++i)
    {
        decideFair(dice[i]);
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
