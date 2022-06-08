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

long long dp[MAX_N];
int dpSegs[MAX_N];

std::pair<long long, int> solveCost(long long cost)
{
    for (int i = 0; i < n; ++i)
    {
        dp[i] = cost + r[0] * (i + 1);
        dpSegs[i] = 1;

        for (int j = 0; j < i; ++j)
        {
            long long val = dp[j] + cost + r[j + 1] * (i - j);
            if (val < dp[i] || (val == dp[i] && dpSegs[j] + 1 < dpSegs[i]))
            {
                dp[i] = val;
                dpSegs[i] = dpSegs[j] + 1;
            }
        }
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
