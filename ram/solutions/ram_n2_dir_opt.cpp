#include <iostream>
#include <algorithm>
#include <functional>

const int MAX_N = 1e6;

int n, k;
long long r[MAX_N];

long long ans;

void input()
{
    std::cin >> n >> k;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> r[i];
    }
}

void output()
{
    std::cout << ans << "\n";
}

struct Line
{
    long long slope;
    long long bias;
    int startX;
};

int crossX(const Line& l1, const Line& l2)
{
    long long dSlope = l2.slope - l1.slope;
    long long dBias = l1.bias - l2.bias;
    if (dSlope == 0) return l1.bias <= l2.bias ? 0 : n;
    return std::min(std::max((dBias + dSlope - 1) / dSlope, 0ll), (long long) n);
}

struct CHT
{
    Line hull[MAX_N];
    int size;
    int start;

    void reset()
    {
        size = 0;
        start = 0;
    }

    void addLine(long long slope, long long bias)
    {
        Line line = {slope, bias};
        while (size > 0 && crossX(line, hull[size - 1]) <= hull[size - 1].startX)
        {
            --size;
        }

        if (size > 0) line.startX = crossX(line, hull[size - 1]);
        else line.startX = 0;

        hull[size++] = line;
    }

    long long query(int x)
    {
        while (start < size - 1 && hull[start + 1].startX <= x)
        {
            ++start;
        }

        return hull[start].slope * x + hull[start].bias;
    }
};

CHT cht;
long long dp[2][MAX_N];

void solve()
{
    std::sort(r, r + n, std::greater<long long>());

    bool curr = false;

    for (int i = 0; i < n; ++i)
    {
        dp[curr][i] = r[0] * (i + 1);
    }

    for (int t = 2; t <= k; ++t)
    {
        cht.reset();
        curr = !curr;

        cht.addLine(r[0], r[0]);
        dp[curr][0] = r[0];

        for (int i = 1; i < n; ++i)
        {
            cht.addLine(r[i], dp[!curr][i - 1] - r[i] * (i - 1));
            dp[curr][i] = cht.query(i);
        }
    }

    ans = dp[curr][n - 1];
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
