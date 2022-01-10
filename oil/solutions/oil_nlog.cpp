#include <iostream>
#include <limits.h>
#include <queue>

const int MAX_N = 1e6;
const long long NEG_INF = LLONG_MIN;

int n;
long long storageCost;
long long prices[MAX_N];

long long ans;

void input()
{
    std::cin >> n >> storageCost;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> prices[i]; 
    }
}

void output()
{
    std::cout << ans << std::endl;
}

std::priority_queue<long long> q;

void solve()
{
    for (int i = 0; i < n; ++i)
    {
        prices[i] += (n - i) * storageCost;
    }

    for (int i = n - 1; i >= 0; --i)
    {
        if (!q.empty() && q.top() > prices[i])
        {
            ans += q.top() - prices[i];
            q.pop();
            q.push(prices[i]);
        }
        q.push(prices[i]);
    }
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