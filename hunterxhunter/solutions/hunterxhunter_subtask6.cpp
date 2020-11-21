#include <iostream>
#include <algorithm>

const int MAX_N = 700;
const int MAX_REWARD = 20000;
const int MAX_K = MAX_N / 2;

const int NEG_INF = -(MAX_N * MAX_REWARD + 1);

struct Person
{
    int target;
    int reward;
    bool vis;
};

int n, k;
Person people[MAX_N];
int ans;

void input()
{
    std::cin >> n >> k;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> people[i].target >> people[i].reward;
        people[i].vis = false;
    }
}

void output()
{
    std::cout << ans << std::endl;
}

int buffer[5][(1 + MAX_K) * MAX_N + 1];

void solve_path(int startP, int* init, int* best, bool overwrite)
{
    int prevreward = -1;
    int currP = startP;
    int* dpPrev2;
    int* dpPrev = init;
    int* dpCurr;
    int buffCnt = 0;

    do
    {
        dpCurr = &buffer[buffCnt][n * k];
        buffCnt = (buffCnt + 1) % 3;

        people[currP].vis = true;
        int currReward = people[currP].reward;

        for (int balance = -(n * k); balance <= n; ++balance)
        {
            int currBest = NEG_INF;
            if (balance - 1 >= -(n * k)) currBest = std::max(currBest, dpPrev[balance - 1]);
            if (balance + k <= n) currBest = std::max(currBest, dpPrev[balance + k] + currReward);
            if (prevreward >= 0) currBest = std::max(currBest, dpPrev2[balance] + prevreward);
            dpCurr[balance] = currBest;
        }

        currP = people[currP].target;
        prevreward = currReward;
        
        dpPrev2 = dpPrev;
        dpPrev = dpCurr;
    } 
    while (currP != startP);

    for (int balance = -(n * k); balance <= n; ++balance)
    {
        if (overwrite) best[balance] = dpPrev[balance];
        else best[balance] = std::max(best[balance], dpPrev[balance]);
    }
}

void solve_cycle(int startP, int* init, int* best)
{
    solve_path(startP, init, best, true);
    solve_path(people[startP].target, init, best, false);
}

void solve()
{
    int* init = &buffer[3][n * k];
    int* best = &buffer[4][n * k];

    std::fill(init - n * k, init + n + 1, NEG_INF);
    init[0] = 0;

    for (int i = 0; i < n; ++i)
    {
        if (!people[i].vis)
        {
            solve_cycle(i, init, best);
            std::swap(init, best);
        }
    }

    ans = *std::max_element(init, init + n + 1);
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