#include <iostream>
#include <iomanip>
#include <vector>

const double EPS = 1e-9;

const int MAX_N = 1e2;

double prob;
int n, l, r;
double ans;

double cost[MAX_N + 1][MAX_N + 1]; // left correct, right correct

void relax(int left, int right)
{
    double& currCost = cost[left][right];

    if (left == n && right == n)
    {
        currCost = 0;
    }
    else if (left == right)
    {
        currCost = 1 + prob * cost[left + 1][right] + (1 - prob) * cost[left][right + 1];
    }
    else if (left < right)
    {
        currCost = 2 + prob * cost[left + 1][right];
        if (left >= 1)
        {
            double remProb = (1 - prob) * prob;
            for (int i = 1; i < left; ++i)
            {
                currCost += (1 - prob) * remProb * cost[left - i][right];
                remProb *= prob;
            }
            currCost += remProb * cost[0][right];
            currCost /= 1 - (1 - prob) * (1 - prob);
        }
        else
        {
            currCost /= 1 - (1 - prob);
        }
    }
    else if (right < left)
    {
        currCost = 2 + (1 - prob) * cost[left][right + 1];
        if (right >= 1)
        {
            double remProb = prob * (1 - prob);
            for (int i = 1; i < right; ++i)
            {
                currCost += prob * remProb * cost[left][right - i];
                remProb *= 1 - prob;
            }
            currCost += remProb * cost[left][0];
            currCost /= 1 - prob * prob;
        }
        else
        {
            currCost /= 1 - prob;
        }
    }
}

struct State
{
    int left, right;
};

void solve()
{
    std::vector<State> states;

    for (int sum = 2 * n; sum >= 0; --sum)
    {
        for (int left = std::max(0, sum - n); left <= std::min(n, sum); ++left)
        {
            int right = sum - left;
            states.push_back({left, right});
        }
    }

    for (State state : states)
    {
        cost[state.left][state.right] = 0;
    }

    int iters = 0;
    double& curr = cost[l][r];
    double prev = curr;

    while (true)
    {
        for (State state : states)
        {
            relax(state.left, state.right);
        }

        ++iters;
        if (std::abs(curr - prev) < EPS) break;
        prev = curr;
    }

    ans = curr;
}

int main()
{
    std::cin >> prob >> n >> l >> r;
    solve();
    std::cout << std::setprecision(15) << ans << std::endl;

    return 0;
}
