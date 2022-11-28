#include <iostream>
#include <iomanip>

double prob;
int n, l, r;
double ans;

double costNext(int curr, double prob)
{
    return 2 / prob + 2 * (1 - prob) / prob * curr;
}

void solve()
{
    if (l < r)
    {
        std::swap(l, r);
        prob = 1 - prob;
    }

    ans = 0;

    for (int curr = l; curr < n; ++curr)
    {
        ans += 1 + prob * costNext(curr, 1 - prob) + (1 - prob) * costNext(curr, prob);
    }

    for (int curr = r; curr < l; ++curr)
    {
        ans += costNext(curr, l > r ? 1 - prob : prob);
    }
}

int main()
{
    std::cin >> prob >> n >> l >> r;
    solve();
    std::cout << std::setprecision(15) << ans << std::endl;

    return 0;
}
