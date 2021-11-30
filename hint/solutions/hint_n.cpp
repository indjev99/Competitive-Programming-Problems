#include "hint.h"
#include <algorithm>

std::vector<bool> genHint(const std::vector<int>& a, const std::vector<int>& b, const std::vector<int>& sol)
{
    std::vector<bool> hint;

    int n = a.size();
    int k = sol.size();

    for (int i = 0, j = 0; i < n; ++i)
    {
        if (j < k && a[i] == sol[j])
        {
            hint.push_back(true);
            ++j;
        }
        else hint.push_back(false);
    }

    return hint;
}

std::vector<int> solve(const std::vector<int>& a, const std::vector<int>& b, const std::vector<bool>& hint)
{
    std::vector<int> sol;

    int n = a.size();

    for (int i = 0; i < n; ++i)
    {
        if (hint[i]) sol.push_back(a[i]);
    }

    return sol;
}
