#include "shoes.h"

#include <numeric>
#include <iterator>
#include <algorithm>
#include <math.h>

const int MAX_N = 5e3;

int findMatch(const std::vector<int>& unmatched, int x)
{
    int left = 0;
    int right = unmatched.size();

    while (right - left > 1)
    {
        int mid = (left + right) / 2;

        std::vector<int> temp;
        std::copy(unmatched.begin() + mid, unmatched.begin() + right, std::back_inserter(temp));
        temp.push_back(x);

        if (pairInSet(temp)) left = mid;
        else right = mid;
    }

    return left;
}

std::vector<std::pair<int, int>> guessPairs(int n)
{
    std::vector<std::pair<int, int>> pairs;

    std::vector<int> left(n);
    std::vector<int> right(n);

    std::iota(left.begin(), left.end(), 1);
    std::iota(right.begin(), right.end(), n + 1);

    for (int i = 0; i < n; ++i)
    {
        int match = findMatch(left, right[i]);

        pairs.push_back({left[match], right[i]});
        std::swap(left[match], left.back());
        left.pop_back();
    }

    return pairs;
}
