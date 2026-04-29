#include "triangle.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <random>
#include <chrono>

static const int T = 1e3;
static const int MAXN = 1e3 + 10;
static int perm[MAXN];
static long long seed = 4571891552734; //std::chrono::steady_clock::now().time_since_epoch().count()
static std::mt19937 rng(seed);
static int n;
static int cnt;

bool query(int a, int b, int c)
{
    cnt++;
    if (!(0 <= a && a < n) || !(0 <= b && b < n) || !(0 <= c && c < n))
    {
        std::cout << "WA!" << '\n';
        exit(0);
    }
    
    return (perm[a] + perm[b] > perm[c] && perm[a] + perm[c] > perm[b] && perm[b] + perm[c] > perm[a]);
}

int main()
{
    long long sum = 0;
    std::cin >> n;
    
    for (int t = 1 ; t <= T ; ++t)
    {
        std::iota(perm, perm + n, 1);
        std::shuffle(perm, perm + n, rng);
        
        cnt = 0;
        std::vector <int> ans = solve(n);
        if (ans.size() != n)
        {
            std::cout << "WA!\n";
            exit(0);
        }
        
        for (int i = 0 ; i < n ; ++i)
        {
            if (perm[i] != ans[i])
            {
                std::cout << "WA!" << '\n';
                exit(0);
            }
        }
        
        sum += cnt;
    }

    std::cout << "OK! " << std::fixed << std::setprecision(5) << 4571891552734 << ' ' << ((double)sum / T) << '\n';
    return 0;
}