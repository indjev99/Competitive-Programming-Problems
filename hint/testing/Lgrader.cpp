#include "hint.h"

#include <iostream>
#include <vector>

static int n, m, k;
static std::vector<int> a, b, sol, maybeSol;
static std::vector<bool> hint;

int main()
{
    std::cin >> n >> m;

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

    sol.resize(k);

    for (int& num : sol)
    {
        std::cin >> num;
    }

    hint = genHint(a, b, sol);
    maybeSol = solve(a, b, hint);

    std::cout << hint.size() << std::endl;
    std::cout << maybeSol.size() << std::endl;

    for (int i = 0; i < (int) maybeSol.size(); ++i)
    {
        if (i > 0) std::cout << " ";
        std::cout << maybeSol[i];
    }
    std::cout << std::endl;

    return 0;
}
