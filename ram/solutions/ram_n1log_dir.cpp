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
    int segs;
    int startX;
};

int crossX(const Line& l1, const Line& l2)
{
    long long dSlope = l2.slope - l1.slope;
    long long dBias = l1.bias - l2.bias;
    if (dSlope == 0)
    {
        if (l1.bias == l2.bias) return l1.segs <= l2.segs ? 0 : n;
        return l1.bias < l2.bias ? 0 : n;
    }
    int cross = std::min(std::max((dBias + dSlope - 1 + (l2.segs < l1.segs)) / dSlope, 0ll), (long long) n);
    return cross;
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

    void addLine(long long slope, long long bias, int segs)
    {
        Line line = {slope, bias, segs};
        while (size > 0 && crossX(line, hull[size - 1]) <= hull[size - 1].startX)
        {
            --size;
        }

        if (size > 0) line.startX = crossX(line, hull[size - 1]);
        else line.startX = 0;

        hull[size++] = line;
    }

    std::pair<long long, int> query(int x)
    {
        while (start < size - 1 && hull[start + 1].startX <= x)
        {
            ++start;
        }

        return {hull[start].slope * x + hull[start].bias, hull[start].segs};
    }
};

CHT cht;
long long dp[MAX_N];
int dpSegs[MAX_N];

std::pair<long long, int> solveCost(long long cost)
{
    cht.reset();

    cht.addLine(r[0], cost + r[0], 1);
    dp[0] = cost + r[0];
    dpSegs[0] = 1;

    for (int i = 1; i < n; ++i)
    {
        cht.addLine(r[i], dp[i - 1] + cost - r[i] * (i - 1), dpSegs[i - 1] + 1);
        auto [val, segs] = cht.query(i);
        dp[i] = val;
        dpSegs[i] = segs;
    }

    return {dp[n - 1] - cost * dpSegs[n - 1], dpSegs[n - 1]};
}

void solve()
{
    std::sort(r, r + n, std::greater<long long>());

    int numDiff = 0;
    long long sum = 0;
    for (int i = 0; i < n; ++i)
    {
        sum += r[i];
        numDiff += i == 0 || r[i] != r[i - 1];
    }

    if (k > numDiff)
    {
        ans = sum;
        return;
    }

    long long left = 0;
    long long leftVal = sum;
    int leftSegs = numDiff;

    long long right = r[0] * n + 1;
    long long rightVal = r[0] * n;
    int rightSegs = 1;

    while (right - left > 1)
    {
        long long mid = (left + right) / 2;
        auto [val, segs] = solveCost(mid);

        if (segs <= k)
        {
            right = mid;
            rightVal = val;
            rightSegs = segs;
        }
        else
        {
            left = mid;
            leftVal = val;
            leftSegs = segs;
        }
    }

    if (leftSegs == rightSegs) ans = leftVal;
    else ans = leftVal + (rightVal - leftVal) / (rightSegs - leftSegs) * (k - leftSegs);
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
