#include "hint.h"

#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <vector>

#define HINT 0
#define SOLVE 1

static int mode;
static int n, m, k;
static std::vector<int> a, b, sol, maybeSol;
static std::vector<bool> hint;

int main()
{
    signal(SIGPIPE, SIG_IGN);

    std::cin >> mode >> n >> m;

    a.resize(n);
    b.resize(m);

    for (int& num : a)
    {
        std::cin >> num;
    }

    for (int& num : b)
    {
        std::cin >> num;
    }

    std::cin >> k;

    if (mode == HINT)
    {
        sol.resize(k);

        for (int& num : sol)
        {
            std::cin >> num;
        }

        hint = genHint(a, b, sol);

        std::cout << hint.size() << "\n";
        
        for (int i = 0; i < (int) hint.size(); ++i)
        {
            if (i > 0) std::cout << " ";
            std::cout << hint[i];
        }
        std::cout << std::endl;
    }
    else if (mode == SOLVE)
    {
        hint.resize(k);

        for (int i = 0; i < k; ++i)
        {
            int bit;
            std::cin >> bit;
            hint[i] = bit;
        }

        maybeSol = solve(a, b, hint);

        std::cout << maybeSol.size() << "\n";

        for (int i = 0; i < (int) maybeSol.size(); ++i)
        {
            if (i > 0) std::cout << " ";
            std::cout << maybeSol[i];
        }
        std::cout << std::endl;
    }

    return 0;
}
