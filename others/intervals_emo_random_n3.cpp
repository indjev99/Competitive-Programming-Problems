#include "intervals.h"

#include <algorithm>
#include <random>

std::mt19937 generator;

int randInt(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(generator);
}

const int MAX_N = 5000;

bool ints[MAX_N][MAX_N];

std::vector<int> intsOf[MAX_N];

std::vector<int> max_cardinality_intersection(int n)
{
    generator.seed(0);

    std::vector<int> nonSolo;

    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            ints[i][j] = ints[j][i] = length_intersection({i, j});
            if (ints[i][j])
            {
                intsOf[i].push_back(j);
                intsOf[j].push_back(i);
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        if (!intsOf[i].empty())
        {
            nonSolo.push_back(i);
        }
    }

    if (nonSolo.empty())
    {
        return {0};
    }

    std::vector<int> best;

    int numIters = 0;
    int maxIters = n * n * n ;

    while (numIters < maxIters)
    {
        std::vector<int> curr;

        int start = nonSolo[randInt(0, nonSolo.size())];

        curr = {start};

        for (int p = 0; p < (int) intsOf[start].size(); ++p)
        {
            std::swap(intsOf[start][p], intsOf[start][randInt(p, intsOf[start].size())]);
    
            int i = intsOf[start][p];

            bool rej = false;
            for (int j : curr)
            {
                ++numIters;
                if (!ints[j][i])
                {
                    rej = true;
                    break;
                }
            }

            if (rej) continue;

            curr.push_back(i);

            if (intsOf[i].size() < intsOf[start].size() - (p + 1))
            {
                start = i;
                p = -1;
            }
        }

        if (curr.size() > best.size())
        {
            best = curr;
        }
    }

    return best;
}
