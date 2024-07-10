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

struct UnfairDie
{
    std::vector<std::vector<double>> probPows;
};

std::vector<UnfairDie> unfairDice;

void genOneUnfairDie()
{
    std::vector<double> probs(n);

    double total = 0;
    for (int i = 0; i < n; ++i)
    {
        probs[i] = randReal();
        total += probs[i];
    }

    for (int i = 0; i < n; ++i)
    {
        probs[i] /= total;
    }

    UnfairDie unfairDie;
    unfairDie.probPows.resize(n);

    for (int i = 0; i < n; ++i)
    {
        unfairDie.probPows[i].resize(m + 1);
        unfairDie.probPows[i][0] = 1;
        for (int j = 1; j <= m; ++j)
        {
            unfairDie.probPows[i][j] = probs[i] * unfairDie.probPows[i][j - 1];
        }
    }

    unfairDice.push_back(unfairDie);
}

void genUnfairDice()
{
    const int NUM_UNFAIR = 2e3;

    for (int i = 0; i < NUM_UNFAIR; ++i)
    {
        genOneUnfairDie();
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

std::unordered_map<long long, bool> ordCodeIsFair;

void decideFair(Die& die)
{
    std::sort(die.counts.begin(), die.counts.end());
    long long ordCode = getOrdCode(die.counts);

    if (ordCodeIsFair.count(ordCode))
    {
        die.fair = ordCodeIsFair.at(ordCode);
        return;
    }

    double relProbRollCondFair = powl(1.0 / n, m);

    double relProbRollCondUnair = 0;

    for (const UnfairDie& unfairDie : unfairDice)
    {
        double currRelProb = 1.0;
        for (int i = 0; i < n; ++i)
        {
            currRelProb *= unfairDie.probPows[i][die.counts[i]];
        }

        relProbRollCondUnair += currRelProb / unfairDice.size();
    }

    double relProbRollAndFair = relProbRollCondFair * p;
    double relProbRollAndUnfair = relProbRollCondUnair * (1 - p);

    double relProbRoll = relProbRollAndFair + relProbRollAndUnfair;

    double probFairCondRoll = relProbRollAndFair / relProbRoll;
    double probUnfairCondRoll = 1 - probFairCondRoll;

    die.fair = probFairCondRoll * penFair > probUnfairCondRoll * penUnfair;

    ordCodeIsFair[ordCode] = die.fair;
}

void solve()
{
    genUnfairDice();

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
