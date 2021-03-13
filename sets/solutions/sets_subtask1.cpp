#include <iostream>

const int MAX_N = 2e6;
const int INF = 2e8;

int n;
int x[MAX_N], y[MAX_N];

int ans;

void input()
{
    std::cin >> n;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> x[i] >> y[i];
    }
}

void output()
{
    std::cout << ans << std::endl;
}

void updateMinMax(int& minX, int& maxX, int x)
{
    minX = std::min(minX, x);
    maxX = std::max(maxX, x);
}

void recSolve(int i, int minX, int maxX, int minY, int maxY)
{
    if (i == n)
    {
        int currAns = maxX - minX + maxY - minY;
        if (currAns >= 0) ans = std::min(ans, currAns);
        return;
    }

    recSolve(i + 1, std::min(minX, x[i]), std::max(maxX, x[i]), minY, maxY);
    recSolve(i + 1, minX, maxX, std::min(minY, y[i]), std::max(maxY, y[i]));
}

void solve()
{
    ans = INF;
    recSolve(0, INF, -INF, INF, -INF);
}

int main()
{
    std::ios::sync_with_stdio(false);

    input();
    solve();
    output();

    return 0;
}
