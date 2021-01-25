#include <iostream>

typedef unsigned int uint;

const uint MOD = 1e9 + 7;

long long n;

uint ans;

void input()
{
    int t;
    std::cin >> n >> t;
}

void output()
{
    std::cout << ans << std::endl;
}

void solve()
{
    if (n % 3 != 0) return;
    n /= 3;

    uint curr;
    curr = 1;

    while (n--)
    {
        curr = 2 * curr % MOD;
    }

    ans = curr;
}

int main()
{
    input();
    solve();
    output();

    return 0;
}