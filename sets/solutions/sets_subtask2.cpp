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

void solve()
{
    ans = INF;
    for (int l = 0; l < n; ++l)
    {
        for (int r = l + 1; r <= n; ++r)
        {
            // S_X = X[l, r)
            // S_Y = Y[0, l) \cup Y[r, n)
            if (l == 0 && r == n) continue;
            int minX = INF, maxX = -INF;
            int minY = INF, maxY = -INF;
            for (int i = 0; i < n; ++i)
            {
                if (l <= i && i < r) updateMinMax(minX, maxX, x[i]);
                else updateMinMax(minY, maxY, y[i]);
            }
            int currAns = maxX - minX + maxY - minY;
            ans = std::min(ans, currAns);
        }
    }
}

int main()
{
    std::ios::sync_with_stdio(false);

    input();
    solve();
    output();

    return 0;
}
