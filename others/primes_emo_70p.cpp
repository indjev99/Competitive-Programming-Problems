#include "primes.h"

#include <numeric>

std::vector<int> solve(int n)
{
    int one = -1;
    std::vector<int> primes;

    std::vector<int> active(n);
    std::iota(active.begin(), active.end(), 0);

    while (!active.empty())
    {
        int x = active.front();

        std::vector<int> divs;
        std::vector<int> newActive;

        for (int y : active)
        {
            if (x == y || query(y, x))
            {
                divs.push_back(y);
            }
            else
            {
                newActive.push_back(y);
            }
        }

        active.swap(newActive);

        bool newOne = false;
        std::vector<int> newPrimes;

        for (int i = 0; i < (int) divs.size(); i++)
        {
            int dDivs = 0;
            if (one != -1 && !newOne) dDivs++;

            for (int a : divs)
            {
                if (a == divs[i] || divs[i] == x || a == one || (a != x && query(a, divs[i])))
                {
                    dDivs++;
                }
            }

            if (dDivs == 1)
            {
                one = divs[i];
                newOne = true;
            }

            if (dDivs == 2)
            {
                primes.push_back(divs[i]);
                newPrimes.push_back(divs[i]);
            }
        }

        if (newOne)
        {
            std::vector<int> newActive;
            for (int y : active)
            {
                if (y != one)
                {
                    newActive.push_back(y);
                }
            }
            active.swap(newActive);
        }

        for (int p : newPrimes)
        {
            std::vector<int> newActive;
            for (int y : active)
            {
                if (p != y && !query(p, y))
                {
                    newActive.push_back(y);
                }
            }
            active.swap(newActive);
        }
    }

    return primes;
}
