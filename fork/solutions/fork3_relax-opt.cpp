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

void relaxStates(const std::vector<State>& states, int goalLeft, int goalRight, int& iters)
{
    double& curr = cost[goalLeft][goalRight];
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
}

void solve()
{
    int iters = 0;

    for (int max = n; max >= 0; --max)
    {
        std::vector<State> statesLeft;
        std::vector<State> statesRight;

        for (int other = max - 1; other >= 0; --other)
        {
            statesLeft.push_back({max, other});
            statesRight.push_back({other, max});
        }

        relax(max, max);

        relaxStates(statesLeft, max, max - 1, iters);
        relaxStates(statesRight, max - 1, max, iters);
    }

    ans = cost[l][r];
}

int main()
{
    std::cin >> prob >> n >> l >> r;
    solve();
    std::cout << std::setprecision(15) << ans << std::endl;

    return 0;
}
