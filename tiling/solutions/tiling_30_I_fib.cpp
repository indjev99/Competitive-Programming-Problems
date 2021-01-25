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
    uint prev, curr, next;
    prev = 0;
    curr = 1;

    while (n--)
    {
        next = (curr + prev) % MOD;

        prev = curr;
        curr = next;
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