#include "triangle.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <random>
#include <chrono>
#include <numeric>

static const int MAXN = 1e3 + 10;
static int cntQueries;
static int perm[MAXN];
static bool isNumTaken[MAXN];
static int N;

static void error(std::string msg)
{
    std::cout << msg << '\n';
    exit(0);
}

bool query(int a, int b, int c)
{
    cntQueries++;
    if (!(0 <= a && a < N) || !(0 <= b && b < N) || !(0 <= c && c < N))
    {
        std::cout << "Invalid query: " << a << ' ' << b << ' ' << c << '\n';
        exit(0);
    }

    return (perm[a] + perm[b] > perm[c] && perm[a] + perm[c] > perm[b] && perm[b] + perm[c] > perm[a]);
}

int main()
{
    int T, S, R;
    std::cin >> T >> N >> R;
    if (R == 1)
    {
        std::cin >> S;
    }

    std::mt19937 rng(S);
    double average = 0.0;
    for (int t = 1 ; t <= T ; ++t)
    {
        if (R == 2)
        {
            for (int i = 0 ; i < N ; ++i)
            {
                std::cin >> perm[i];
            }

            std::fill(isNumTaken, isNumTaken + N + 1, false);
            for (int i = 0 ; i < N; ++i)
            {
                if (perm[i] < 1 || perm[i] > N || isNumTaken[perm[i]])
                {
                    error("Invalid input.");
                }

                isNumTaken[perm[i]] = true;
            }
        } else
        {
            std::iota(perm, perm + N, 1);
            std::shuffle(perm, perm + N, rng);
        }

        cntQueries = 0;
        std::vector <int> ans = solve(N);
        average += cntQueries;

        for (int i = 0 ; i < N ; ++i)
        {
            if (ans[i] != perm[i])
            {
                error("Wrong answer.");
            }
        }
    }

    average /= T;
    std::cout << std::fixed << std::setprecision(2) << "OK! " << average << '\n';
    return 0;
}
