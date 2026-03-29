#include <iostream>
#include <set>
#include <fstream>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>
#include <optional>
#include <filesystem>

std::mt19937 generator(0);

int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub);
    return distribution(generator);
}

double randReal(double lb, double ub)
{
    std::uniform_real_distribution<double> distribution(lb, ub);
    return distribution(generator);
}

struct TestSpec
{
    std::string msg = "";
    std::optional<std::pair<std::string, std::string>> sample_path;

    int n;
    int m;
    long long maxW;

    bool noNeg = false;
    bool noPos = false;
    int cycleSign = 0;
    bool allMax = false;
    bool allMin = false;
    bool halfHalf = false;
    bool breakCycle = false;
    int trashNodes = 0;
    bool smallPosCycles = false;
};

struct Edge
{
    int from;
    int to;
    long long w;
};

void genTest(std::ofstream& outfile, const TestSpec& testSpec)
{
    int trueN = testSpec.n;
    int trashNodes = testSpec.trashNodes;
    int n = trueN - trashNodes;
    int m = testSpec.m;

    long long minW = -testSpec.maxW;
    long long maxW = testSpec.maxW;
    if (testSpec.noNeg) minW = 0;
    if (testSpec.noPos) maxW = 0;

    int cycleSign = testSpec.cycleSign;
    bool breakCycle = testSpec.breakCycle;

    long long sum = 0;
    std::vector<long long> steps(n);
    for (int i = 0; i < n; i++)
    {
        if (testSpec.allMax) steps[i] = maxW;
        else if (testSpec.allMin) steps[i] = minW;
        else if (testSpec.halfHalf) steps[i] = i < n / 2 ? minW : maxW;
        else steps[i] = randNum(minW, maxW);
        sum += steps[i];
    }

    if (cycleSign <= 0)
    {
        while (sum > 0)
        {
            int i = randNum(0, n - 1);
            if (steps[i] == minW) continue;

            long long old = steps[i];
            long long neww = randNum(std::max(minW, steps[i] - sum), steps[i] - 1);
            sum += neww - old;
            steps[i] = neww;
        }
    }

    if (cycleSign >= 0)
    {
        while (sum < 0)
        {
            int i = randNum(0, n - 1);
            if (steps[i] == maxW) continue;

            long long old = steps[i];
            long long neww = randNum(steps[i] + 1, std::min(maxW, steps[i] - sum));
            sum += neww - old;
            steps[i] = neww;
        }
    }

    if (cycleSign < 0 && sum == 0)
    {
        while (sum == 0)
        {
            int i = randNum(0, n - 1);
            if (steps[i] == minW) continue;

            steps[i]--;
            sum--;
        }
    }

    if (cycleSign > 0 && sum == 0)
    {
        while (sum == 0)
        {
            int i = randNum(0, n - 1);
            if (steps[i] == maxW) continue;

            steps[i]++;
            sum++;
        }
    }

    std::vector<long long> prefs(2 * n + 1);
    prefs[0] = 0;
    for (int i = 0; i < 2 * n; i++)
    {
        prefs[i + 1] = prefs[i] + steps[i % n];
    }

    std::vector<Edge> edges;
    std::set<std::pair<int, int>> used;

    const auto insert_edge = [&] (const Edge edge)
    {
        if (used.find(std::make_pair(edge.from, edge.to)) == used.end())
        {
            used.insert(std::make_pair(edge.from, edge.to));
            edges.push_back(edge);
            return true;
        }
        return false;
    };

    for (int i = 0; i < n; i++)
    {
        if (breakCycle && i == n - 1) continue;
        assert(insert_edge({i, (i + 1) % n, steps[i]}));
    }

    if (testSpec.smallPosCycles)
    {
        auto leftTrash = trashNodes;
        auto next = n;
        while (leftTrash > 0)
        {
            const auto newSize = leftTrash < 10 ? leftTrash : randNum(1, leftTrash);
            const auto newEdges = newSize + 1;

            if ((int) edges.size() + newEdges > m)
            {
                break;
            }

            auto left = randNum(0, n - 1);
            auto right = (left + randNum(1, n - 1)) % n;
            if (left > right) { std::swap(left, right); }
            const auto cMinW = prefs[right] - prefs[left];
            const auto perEdge = (cMinW + newEdges - 1) / newEdges;

            if (perEdge < minW || perEdge > maxW)
            {
                continue;
            }

            leftTrash -= newSize;

            for (int i = 0; i < newSize - 1; i ++)
            {
                insert_edge({next + i, next + i + 1, perEdge});
            }
            insert_edge({left, next, perEdge});
            insert_edge({next + newSize - 1, right, perEdge});
            next += newSize;
        }
    }
    else
    {
        for (int i = 0; i < trashNodes && (int) edges.size() < m; i++)
        {
            int num = n + i;
            int prev = randNum(0, num - 1);

            if (randNum(0, 1)) std::swap(num, prev);

            insert_edge({prev, num, randNum(minW, maxW)});
        }
    }

    for (int steps = 3 * (m - (int) edges.size()); steps >= 0 && (int) edges.size() < m; steps --)
    {
        int i = randNum(0, n - 1);
        int j = i + randNum(1, n - 1);

        if (breakCycle)
        {
            if (j % n < i) continue;
            insert_edge({i, j % n, randNum(minW, maxW)});
        }
        else
        {
            long long cMinW = prefs[j] - prefs[i];
            if (cMinW > maxW) continue;
            insert_edge({i, j % n, randNum(std::max(cMinW, minW), maxW)});
        }
    }

    std::vector<int> nums(trueN);
    std::iota(nums.begin(), nums.end(), 0);

    if (m != n) std::shuffle(nums.begin(), nums.end(), generator);

    for (auto& [f, t, w] : edges)
    {
        assert(f >= 0 && f < trueN && t >= 0 && t < trueN);
        assert(w >= minW && w <= maxW);
        f = nums[f];
        t = nums[t];
        assert(f >= 0 && f < trueN && t >= 0 && t < trueN);
    }

    if (m != n) std::shuffle(edges.begin(), edges.end(), generator);

    std::cerr << "Generated " << trueN << " " << edges.size() << std::endl;
    outfile << trueN << " " << edges.size() << "\n";
    for (auto [f, t, w] : edges)
    {
        outfile << f << " " << t << " " << w << "\n";
    }
}

const int N = 5000;
const int SMALL_N = 500;
const int MAX_W = 1e9;

std::vector<TestSpec> testSpecs = {
    {
        .msg = "subtask 0",
        .sample_path = std::make_pair("samples/sample.1.in", "samples/sample.1.sol")
    },
    {
        .sample_path = std::make_pair("samples/sample.2.in", "samples/sample.2.sol")
    },
    {
        .msg = "subtask 1",
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noPos = true,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = N / 2,
    },

    {
        .msg = "subtask 2",
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .noNeg = true,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = N / 2,
    },

    {
        .msg = "subtask 3",
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = SMALL_N / 2,
    },


    {
        .msg = "subtask 4",
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },

    {
        .msg = "subtask 5",
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },

    {
        .msg = "subtask 6",
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = SMALL_N / 2,
    },
    {
        .n = SMALL_N,
        .m = 2 * SMALL_N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = SMALL_N / 2,
    },

    {
        .msg = "subtask 7",
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .breakCycle = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = 0,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMin = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .allMax = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 1,
        .halfHalf = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMin = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .allMax = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = -1,
        .halfHalf = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMin = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .allMax = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = N / 2,
    },
    {
        .n = N,
        .m = 2 * N,
        .maxW = MAX_W,
        .cycleSign = 0,
        .halfHalf = true,
        .trashNodes = N / 2,
        .smallPosCycles = true,
    },
};

int main()
{
    std::ios::sync_with_stdio(false);

    int num = 1;

    for (const TestSpec& ts : testSpecs)
    {
        std::string nums = "";
        nums += (num / 100 % 10 + '0');
        nums += (num / 10 % 10 + '0');
        nums += (num % 10 + '0');

        const auto outpath = "tests/trip." + nums + ".in";
        const auto solpath = "tests/trip." + nums + ".sol";

        if (ts.msg != "")
        {
            std::cerr << std::endl;
            std::cerr << ts.msg << std::endl;
        }
        std::cerr << "Test " << num << ": " << outpath << std::endl;

        if (ts.sample_path)
        {
            std::filesystem::copy_file(ts.sample_path.value().first, outpath, std::filesystem::copy_options::overwrite_existing);
            std::filesystem::copy_file(ts.sample_path.value().second, solpath, std::filesystem::copy_options::overwrite_existing);
        }
        else
        {
            std::ofstream solfile(solpath);

            if (ts.breakCycle || ts.cycleSign > 0)
            {
                solfile << 0 << "\n";
            }
            else
            {
                solfile << 1 << "\n";
            }

            std::ofstream outfile(outpath);

            genTest(outfile, ts);
        }

        num++;
    }

    return 0;
}
