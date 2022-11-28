#include <iostream>
#include <iomanip>

const bool LEFT = false;
const bool RIGHT = true;

const int MAX_N = 2e3;

double prob;
int n, l, r;
double ans;

double cost[MAX_N + 1]; // left right

double costInner[2][MAX_N + 1][MAX_N + 1]; // max side, max, other
double failCostPart[2][MAX_N + 1][MAX_N]; // max side, max, other

void dpInner(bool side, double prob)
{
    for (int max = 0; max <= n; ++max)
    {
        failCostPart[side][max][0] = 0;
        for (int i = 1; i < max; ++i)
        {
            failCostPart[side][max][i] = prob * costInner[side][i][i - 1] + prob * failCostPart[side][max][i - 1];
        }

        costInner[side][max][max] = 0;
        for (int i = max - 1; i >= 0; --i)
        {
            double failCost = 1 / (1 - prob) + failCostPart[side][max][i];
            costInner[side][max][i] = costInner[side][max][i + 1] + 1 / prob + (1 / prob - 1) * failCost;
        }
    }
}

void solve()
{
    if (l < r)
    {
        std::swap(l, r);
        prob = 1 - prob;
    }

    dpInner(LEFT, 1 - prob);
    dpInner(RIGHT, prob);

    cost[n] = 0;

    for (int curr = n - 1; curr >= 0; --curr)
    {
        cost[curr] = 1 + cost[curr + 1] + prob * costInner[LEFT][curr + 1][curr] + (1 - prob) * costInner[RIGHT][curr + 1][curr];
    }

    ans = cost[l] + costInner[LEFT][l][r];
}

int main()
{
    std::cin >> prob >> n >> l >> r;
    solve();
    std::cout << std::setprecision(15) << ans << std::endl;

    return 0;
}
