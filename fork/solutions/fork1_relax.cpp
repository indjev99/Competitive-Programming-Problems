#include <iostream>
#include <iomanip>
#include <vector>

const double INF = 1e9;
const double EPS = 1e-9;

const bool LEFT = false;
const bool RIGHT = true;

const int MAX_N = 1e2;
const int MAX_GARBAGE = 10;

double prob;
int n, l, r;
double ans;

double cost[MAX_N + 1][MAX_N + 1][2][MAX_GARBAGE + 1]; // left correct, right correct, garbage side, amount of garbage

double& getCost(int left, int right, int leftG, int rightG)
{
    if (leftG < 0)
    {
        left += leftG;
        leftG = 0;
    }

    left = std::max(left, 0);

    if (rightG < 0)
    {
        right += rightG;
        rightG = 0;
    }

    right = std::max(right, 0);

    if (leftG > MAX_GARBAGE || rightG > MAX_GARBAGE)
    {
        static double inf;
        inf = INF;
        return inf;
    }

    if (leftG > 0) return cost[left][right][LEFT][leftG];
    else return cost[left][right][RIGHT][rightG];
}

void relax(int left, int right, int leftG, int rightG)
{
    double bestCost = INF;
    double currCost;

    if (left == n && right == n && leftG == 0 && rightG == 0)
    {
        currCost = 0;
        bestCost = std::min(bestCost, currCost);
    }

    if (leftG > 0 || rightG > 0)
    {
        currCost = 1 + prob * getCost(left, right, leftG - 1, rightG) + (1 - prob) * getCost(left, right, leftG, rightG - 1);
        bestCost = std::min(bestCost, currCost);
    }

    bool both = left == right && left < n && leftG == 0 && rightG == 0;

    if (both)
    {
        currCost = 1 + prob * getCost(left + 1, right, leftG, rightG) + (1 - prob) * getCost(left, right + 1, leftG, rightG);
        bestCost = std::min(bestCost, currCost);
    }

    if (leftG == 0 && left < n && !both)
    {
        currCost = 1 + prob * getCost(left + 1, right, leftG, rightG) + (1 - prob) * getCost(left, right, leftG, rightG + 1);
        bestCost = std::min(bestCost, currCost);
    }

    if (rightG == 0 && right < n && !both)
    {
        currCost = 1 + prob * getCost(left, right, leftG + 1, rightG) + (1 - prob) * getCost(left, right + 1, leftG, rightG);
        bestCost = std::min(bestCost, currCost);
    }

    getCost(left, right, leftG, rightG) = bestCost;
}

struct State
{
    int left, right, leftG, rightG;
};

void solve()
{
    std::vector<State> states;

    for (int sum = 0; sum <= 2 * n; ++sum)
    {
        for (int left = std::max(0, sum - n); left <= std::min(n, sum); ++left)
        {
            int right = sum - left;
            for (int garbage = 0; garbage <= MAX_GARBAGE; ++garbage)
            {
                states.push_back({left, right, garbage, 0});
                if (garbage == 0) continue;
                states.push_back({left, right, 0, garbage});
            }
        }
    }

    for (State state : states)
    {
        getCost(state.left, state.right, state.leftG, state.rightG) = 0;
    }

    int iters = 0;
    double& curr = getCost(l, r, 0, 0);
    double prev = curr;

    while (true)
    {
        for (State state : states)
        {
            relax(state.left, state.right, state.leftG, state.rightG);
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