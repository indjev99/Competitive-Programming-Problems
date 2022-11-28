#include <iostream>
#include <iomanip>

const bool LEFT = false;
const bool RIGHT = true;

const int MAX_N = 1e6;

double prob;
int n, l, r;
double ans;

double cost[MAX_N + 1]; // left right

double costNext[2][MAX_N]; // max side, other
double failCostPart[2][MAX_N]; // max side, other

void dpInner(bool side, double prob)
{
    for (int i = 0; i < n; ++i)
    {
        if (i == 0) failCostPart[side][i] = 0;
        else failCostPart[side][i] = prob * costNext[side][i - 1] + prob * failCostPart[side][i - 1];

        double failCost = 1 / (1 - prob) + failCostPart[side][i];
        costNext[side][i] = 1 / prob + (1 / prob - 1) * failCost;
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
        cost[curr] = 1 + cost[curr + 1] + prob * costNext[LEFT][curr] + (1 - prob) * costNext[RIGHT][curr];
    }

    ans = cost[l];

    for (int curr = r; curr < l; ++curr)
    {
        ans += costNext[LEFT][curr];
    }
}

int main()
{
    std::cin >> prob >> n >> l >> r;
    solve();
    std::cout << std::setprecision(15) << ans << std::endl;

    return 0;
}
