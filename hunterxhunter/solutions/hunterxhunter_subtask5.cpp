#include <iostream>
#include <algorithm>

const int MAX_N = 10000;
const int MAX_REWARD = 20000;

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

int buffer[5][2 * MAX_N + 1];

void solve_path(int startP)
{
    int prevreward = -1;
    int currP = startP;
    int* dpPrev2;
    int* dpPrev = &buffer[0][n];
    int* dpCurr;
    int buffCnt = 1;

    std::fill(dpPrev - n, dpPrev + n + 1, NEG_INF);
    dpPrev[0] = 0;

    do
    {
        dpCurr = &buffer[buffCnt][n];
        buffCnt = (buffCnt + 1) % 3;

        people[currP].vis = true;
        int currReward = people[currP].reward;

        for (int balance = -n; balance <= n; ++balance)
        {
            int currBest = NEG_INF;
            if (balance - 1 >= -n) currBest = std::max(currBest, dpPrev[balance - 1]);
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

    ans = std::max(ans, *std::max_element(dpPrev, dpPrev + n + 1));
}

void solve()
{
    solve_path(0);
    solve_path(people[0].target);
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