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

// suffY[i] = Y[i, n)
int suffMinY[MAX_N + 1], suffMaxY[MAX_N + 1];

void solve()
{
    suffMinY[n] = INF;
    suffMaxY[n] = -INF;
    for (int i = n - 1; i >= 0; --i)
    {
        suffMinY[i] = std::min(suffMinY[i + 1], y[i]);
        suffMaxY[i] = std::max(suffMaxY[i + 1], y[i]);
    }

    ans = INF;
    int prefMinY = INF, prefMaxY = -INF;
    for (int l = 0; l < n; ++l)
    {
        // prefY = Y[0, l)
        if (l > 0) updateMinMax(prefMinY, prefMaxY, y[l - 1]);
        int minX = INF, maxX = -INF;
        for (int r = l + 1; r <= n; ++r)
        {
            // S_X = X[l, r)
            // S_Y = prefY \cup suffY[r]
            if (l == 0 && r == n) continue;
            updateMinMax(minX, maxX, x[r - 1]);
            int minY = std::min(prefMinY, suffMinY[r]);
            int maxY = std::max(prefMaxY, suffMaxY[r]);
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
