#include <iostream>
#include <iomanip>

double prob;
int n, l, r;
double ans;

void solve()
{
    if (l < r)
    {
        std::swap(l, r);
        prob = 1 - prob;
    }

    ans = (n - l) * (1 + 2 * prob / (1 - prob) + 2 * (1 - prob) / prob);
    ans += (long long) (n - l) * (n + l - 1) * (prob * prob / (1 - prob) + (1 - prob) * (1 - prob) / prob);
    ans += (l - r) * 2 / (1 - prob);
    ans += (long long) (l - r) * (l + r - 1) * prob / (1 - prob);
}

int main()
{
    std::cin >> prob >> n >> l >> r;
    solve();
    std::cout << std::setprecision(15) << ans << std::endl;

    return 0;
}
