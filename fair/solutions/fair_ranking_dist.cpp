#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

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

void giveScore(Die& die)
{
    die.score = 0;
    for (int i = 0; i < n; ++i)
    {
        die.score += std::abs(die.counts[i] - static_cast<double>(m) / n);
    }
}

void solve()
{
    for (int i = 0; i < k; ++i)
    {
        giveScore(dice[i]);
    }

    std::sort(dice.begin(), dice.end(), [](auto& left, auto& right){ return left.score < right.score; });

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

    input();
    solve();
    output();

    return 0;
}
