#include <iostream>
#include <iomanip>
#include <vector>

const double EPS = 1e-9;

const int NONE = 0;
const int LEFT = 1;
const int RIGHT = 2;

const int MAX_N = 1e2;

double prob;
int n, l, r;
double ans;

double cost[MAX_N + 1][MAX_N + 1][3]; // left correct, right correct, garbage state

void relax(int left, int right, int garbage)
{
    double& currCost = cost[left][right][garbage];

    if (left == n && right == n && garbage == NONE)
    {
        currCost = 0;
    }
    else if (garbage == LEFT)
    {
        currCost = 1 / prob;
        double remProb = 1;
        for (int i = 0; i < right; ++i)
        {
            currCost += prob * remProb * cost[left][right - i][NONE];
            remProb *= 1 - prob;
        }
        currCost += remProb * cost[left][0][NONE];
    }
    else if (garbage == RIGHT)
    {
        currCost = 1 / (1 - prob);
        double remProb = 1;
        for (int i = 0; i < left; ++i)
        {
            currCost += (1 - prob) * remProb * cost[left - i][right][NONE];
            remProb *= prob;
        }
        currCost += remProb * cost[0][right][NONE];
    }
    else if (left == right)
    {
        currCost = 1 + prob * cost[left + 1][right][NONE] + (1 - prob) * cost[left][right + 1][NONE];
    }
    else if (left < right)
    {
        currCost = 1 + prob * cost[left + 1][right][NONE] + (1 - prob) * cost[left][right][RIGHT];
    }
    else if (right < left)
    {
        currCost = 1 + prob * cost[left][right][LEFT] + (1 - prob) * cost[left][right + 1][NONE];
    }
}

struct State
{
    int left, right, garbage;
};

void solve()
{
    std::vector<State> states;

    for (int sum = 2 * n; sum >= 0; --sum)
    {
        for (int left = std::max(0, sum - n); left <= std::min(n, sum); ++left)
        {
            int right = sum - left;
            states.push_back({left, right, LEFT});
            states.push_back({left, right, RIGHT});
            states.push_back({left, right, NONE});
        }
    }

    for (State state : states)
    {
        cost[state.left][state.right][state.garbage] = 0;
    }

    int iters = 0;
    double& curr = cost[l][r][NONE];
    double prev = curr;

    while (true)
    {
        for (State state : states)
        {
            relax(state.left, state.right, state.garbage);
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
