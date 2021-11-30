#include "hint.h"
#include <algorithm>
#include <map>
#include <math.h>

const int MAX_N = 2e5;
const int MAX_SQRT = 450;
int dp[MAX_SQRT + 1][MAX_N + 1];

struct Counts
{
    std::vector<int> counts;
    std::vector<int> reps;
    std::map<std::pair<int, int>, int> invReps;
};

Counts findCounts(const std::vector<int>& b)
{
    Counts data;

    int m = b.size();

    data.counts.resize(m, 0);
    data.reps.resize(m);

    for (int i = 0; i < m; ++i)
    {
        data.reps[i] = data.counts[b[i]]++;
        data.invReps[{b[i], data.reps[i]}] = i;
    }
    return data;
}

int findNumBits(int maxNum)
{
    int numBits = 1;
    while ((1 << numBits) < maxNum) ++numBits;
    return numBits;
}

void encode(int num, int numBits, std::vector<bool>& hint)
{
    for (int i = 0; i < numBits; ++i)
    {
        hint.push_back(num % 2);
        num /= 2;
    }
}

std::vector<bool> genHint(const std::vector<int>& a, const std::vector<int>& b, const std::vector<int>& sol)
{
    std::vector<bool> hint;

    int n = a.size();
    int k = sol.size();

    Counts data = findCounts(b);

    int i = 0, j = 0;
    std::vector<int> aPos, bPos;

    for (int num : sol)
    {
        while (a[i] != num) ++i;
        while (b[j] != num) ++j;

        aPos.push_back(i);
        bPos.push_back(j);

        ++i;
        ++j;
    }

    int t = 0;
    int root = sqrt(n);

    for (int bucket = 0; bucket < root; ++bucket)
    {
        int start = bucket * n / root;
        int end = (bucket + 1) * n / root;
        
        int numABits = findNumBits(end - start + 1);

        while (t < k && aPos[t] < start) ++t;
        if (t == k || aPos[t] >= end)
        {
            encode(end - start, numABits, hint);
            continue;
        }

        int numBBits = findNumBits(data.counts[a[aPos[t]]]);

        encode(aPos[t] - start, numABits, hint);
        encode(data.reps[bPos[t]], numBBits, hint);
    }

    return hint;
}

int decode(int numBits, const std::vector<bool>& hint, int start)
{
    int num = 0;
    for (int i = 0; i < numBits; ++i)
    {
        num += hint[start + i] * (1 << i);
    }
    return num;
}

void solveMini(int aStart, int aEnd, int bStart, int bEnd, const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& sol)
{
    int n = aEnd - aStart;
    int m = bEnd - bStart;

    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            if (a[aStart + i - 1] == b[bStart + j - 1]) dp[i][j] = 1 + dp[i - 1][j - 1];
            else dp[i][j] = std::max(dp[i][j - 1], dp[i - 1][j]);
        }
    }

    int i = n, j = m;

    while (i != 0 && j != 0)
    {
        if (a[aStart + i - 1] == b[bStart + j - 1])
        {
            sol.push_back(a[aStart + i - 1]);
            --i;
            --j;
        }
        else if (dp[i - 1][j] > dp[i][j - 1]) --i;
        else --j;
    }

    std::reverse(sol.end() - dp[n][m], sol.end());
}

std::vector<int> solve(const std::vector<int>& a, const std::vector<int>& b, const std::vector<bool>& hint)
{
    std::vector<int> sol;

    int n = a.size();
    int m = b.size();

    Counts data = findCounts(b);

    int root = sqrt(n);

    int numBucks = 0;
    std::vector<int> aPosBuck, bPosBuck;

    for (int bucket = 0, i = 0; bucket < root; ++bucket)
    {
        int start = bucket * n / root;
        int end = (bucket + 1) * n / root;

        int numABits = findNumBits(end - start + 1);
        int offset = decode(numABits, hint, i);
        i += numABits;

        if (offset == end - start) continue;

        aPosBuck.push_back(start + offset);

        int numBBits = findNumBits(data.counts[a[aPosBuck.back()]]);
        bPosBuck.push_back(data.invReps[{a[aPosBuck.back()], decode(numBBits, hint, i)}]);
        i += numBBits;

        ++numBucks;
    }

    int t = 0;

    for (int bucket = 0; bucket < root; ++bucket)
    {
        int end = (bucket + 1) * n / root;

        if (t == numBucks || aPosBuck[t] >= end) continue;
    
        int bStart = bPosBuck[t];
        int bEnd = t + 1 < numBucks ? bPosBuck[t + 1] : m;

        sol.push_back(a[aPosBuck[t]]);
        solveMini(aPosBuck[t] + 1, std::min(end, n), bStart + 1, bEnd, a, b, sol);

        ++t;
    }

    return sol;
}
