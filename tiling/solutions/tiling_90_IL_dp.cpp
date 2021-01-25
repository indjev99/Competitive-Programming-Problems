#include <iostream>

typedef unsigned int uint;

const uint MOD = 1e9 + 7;

long long n;
bool Is, Ls;

uint ans;

void input()
{
    int t;
    std::cin >> n >> t;
    Is = t & 1;
    Ls = t & 2;
}

void output()
{
    std::cout << ans << std::endl;
}

struct State
{
    uint base;
    uint ridge;
};

void solve()
{
    State prev, curr, next;
    prev = {0, 0};
    curr = {1, 0};

    while (n--)
    {
        next = {0, 0};

        if (Is) 
        {
            next.base += curr.base + prev.base;
            next.ridge += curr.ridge;
        }

        if (Ls)
        {
            next.base += prev.ridge;
            next.ridge += 2 * curr.base;
        }

        next.base %= MOD;
        next.ridge %= MOD;

        prev = curr;
        curr = next;
    }

    ans = curr.base;
}

int main()
{
    input();
    solve();
    output();

    return 0;
}