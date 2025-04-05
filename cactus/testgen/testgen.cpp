#include <iostream>
#include <fstream>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

std::mt19937 generator(0);

int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(generator);
}

double randReal(double lb, double ub)
{
    std::uniform_real_distribution<double> distribution(lb, ub);
    return distribution(generator);
}

#define PCM_RNG 0
#define PCM_STAR 1
#define PCM_PATH 2

#define PNM_RNG 0
#define PNM_APPLES 1
#define PNM_BEADS 2

struct TestSpec
{
    std::string name;

    int n;

    std::vector<std::pair<int, double>> cycleSizeProbs;
    double sharedParNodeProb;
    int parCycleMode;
    int parNodeMode;

    bool doAppleCycle = false;
    int initPathH = 0;
    bool addEars = false;
};

int selectCycleSize(const std::vector<std::pair<int, double>>& cycleSizeProbs)
{
    double roll = randReal(0, 1);
    double sum = 0;
    for (auto [size, prob] : cycleSizeProbs)
    {
        sum += prob;
        if (roll <= sum) return size;
    }

    std::cerr << "WARNING: Couldn't select cycle size: sum = " << sum << " roll = " << roll << std::endl;

    return cycleSizeProbs.back().first;
}

std::vector<std::pair<int, int>> genEdgesImpl(const TestSpec& testSpec)
{
    int n = testSpec.n;
    std::vector<std::pair<int, double>> cycleSizeProbs = testSpec.cycleSizeProbs;
    double sharedParNodeProb = testSpec.sharedParNodeProb;
    int parCycleMode = testSpec.parCycleMode;
    int parNodeMode = testSpec.parNodeMode;
    bool doAppleCycle = testSpec.doAppleCycle && parNodeMode == PNM_APPLES;
    int initPathH = testSpec.initPathH;
    bool addEars = testSpec.addEars;

    if (addEars) n -= 4;

    std::vector<std::pair<int, int>> edges;

    std::vector<std::vector<int>> cycles;
    std::vector<int> cyclePars;

    int currNode = 0;

    while (currNode < n)
    {
        int cycleSize = currNode < initPathH ? 1 : selectCycleSize(cycleSizeProbs);

        int parentCycle = -1;
        if (!cycles.empty())
        {
            if (currNode < initPathH) parentCycle = cycles.size() - 1;
            else if (parCycleMode == PCM_RNG) parentCycle = randNum(0, cycles.size());
            else if (parCycleMode == PCM_STAR) parentCycle = initPathH == 0 ? 0 : initPathH - 1;
            else if (parCycleMode == PCM_PATH) parentCycle = cycles.size() - 1;
            else
            {
                std::cerr << "ERROR: Invalid: parCycleMode = " << parCycleMode << std::endl;
                exit(0);
            }
        }

        int parentNode = -1;
        if (parentCycle != -1)
        {
            int parentNodeIdx = -1;
            if (parNodeMode == PNM_RNG) parentNodeIdx = randNum(0, cycles[parentCycle].size());
            else if (parNodeMode == PNM_APPLES) parentNodeIdx = 0;
            else if (parNodeMode == PNM_BEADS) parentNodeIdx = cycles[parentCycle].size() / 2;
            else
            {
                std::cerr << "ERROR: Invalid: parNodeMode = " << parNodeMode << std::endl;
                exit(0);
            }
            parentNode = cycles[parentCycle][parentNodeIdx];
        }

        bool shareParNode = false;
        if (parentCycle != -1 && (cycles[parentCycle].size() > 1 || parentCycle < initPathH) && cycleSize > 1)
        {
            shareParNode = randReal(0, 1) < sharedParNodeProb;
        }

        cycleSize = std::min(cycleSize, n - currNode + shareParNode);
        if (cycleSize == 2) cycleSize = 1;

        if (cycleSize == 1) shareParNode = false;

        if (!shareParNode) cyclePars.push_back(parentCycle);
        else cyclePars.push_back(cyclePars[parentCycle]);

        cycles.push_back({});
        for (int i = 0; i < cycleSize; i++)
        {
            int node = -1;
            if (i == 0 && shareParNode) node = parentNode;
            else node = currNode++;
            cycles.back().push_back(node);
        }

        if (parentNode != -1 && !shareParNode)
        {
            edges.push_back({parentNode, cycles.back().front()});
        }

        if (cycleSize > 1)
        {
            for (int i = 0; i < cycleSize; i++)
            {
                edges.push_back({cycles.back()[i], cycles.back()[(i + 1) % cycleSize]});
            }
        }
    }

    if (doAppleCycle && cycles.back().front() != cycles.front().front() && cycles[cyclePars.back()].front() != cycles.front().front())
    {
        edges.push_back({cycles.back().front(), cycles.front().front()});

        int curr = cycles.size() - 1;

        cycles.push_back({});
        while (curr != -1)
        {
            cycles.back().push_back(cycles[curr].front());
            curr = cyclePars[curr];
        }
    }

    if (addEars)
    {
        n += 4;

        int headC = 0;
        int tailC = initPathH == 0 ? cycles.size() - 1 : initPathH;
        if (tailC >= (int) cycles.size()) tailC = cycles.size() - 1;

        int headNode = cycles[headC].front();
        int tailNode = cycles[tailC][cycles[tailC].size() / 2];

        edges.push_back({headNode, n - 4});
        edges.push_back({headNode, n - 3});
        edges.push_back({tailNode, n - 2});
        edges.push_back({tailNode, n - 1});

        cycles.push_back({n - 4});
        cycles.push_back({n - 3});
        cycles.push_back({n - 2});
        cycles.push_back({n - 1});
    }

    std::vector<bool> inCycle(n, false);

    int numCycles = 0;
    for (int i = 0; i < (int) cycles.size(); i++)
    {
        if (cycles[i].size() == 1) continue;

        numCycles++;

        for (int u : cycles[i])
        {
            inCycle[u] = true;
        }
    }

    int numSingles = 0;
    for (int i = 0; i < n; i++)
    {
        if (!inCycle[i]) numSingles++;
    }

    std::cout << "Counts:" << " edges = " << edges.size() << " cycles = " << numCycles << " singles = " << numSingles << std::endl;

    return edges;
}

std::vector<std::pair<int, int>> genEdges(const TestSpec& testSpec)
{
    int n = testSpec.n;

    std::vector<std::pair<int, int>> edges = genEdgesImpl(testSpec);

    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), generator);

    for (int i = 0; i < (int) edges.size(); i++)
    {
        assert(edges[i].first >= 0);
        assert(edges[i].second >= 0);
        assert(edges[i].first < n);
        assert(edges[i].second < n);

        edges[i].first = perm[edges[i].first];
        edges[i].second = perm[edges[i].second];

        if (randNum(0, 2) == 1) std::swap(edges[i].first, edges[i].second);
    }

    std::shuffle(edges.begin(), edges.end(), generator);

    return edges;
}

void genTest(std::ofstream& outfile, const TestSpec& testSpec)
{
    int n = testSpec.n;

    std::vector<std::pair<int, int>> edges = genEdges(testSpec);

    outfile << n << " " << edges.size() << "\n";

    for (int i = 0; i < (int) edges.size(); i++)
    {
        outfile << edges[i].first << " " << edges[i].second << "\n";
    }
}

std::vector<std::pair<int, double>> makeUpTo(int k, bool dots = false)
{
    std::vector<std::pair<int, double>> sizeProbs;

    double dotProb = dots ? (k * 0.5) / (4 + k * 0.5) : 0;

    double nonDotProb = 1.0 - dotProb;

    double sum = 0;

    for (int i = 1; i <= k; i++)
    {
        if (i == 2) continue;

        double p = nonDotProb / (k - 1);
        if (i == 1 || i == 3) p += dotProb / 2;
    
        sizeProbs.push_back({i, p});

        sum += sizeProbs.back().second;
    }

    assert(std::abs(sum - 1) < 1e-6);

    return sizeProbs;
}

std::vector<std::pair<int, double>> makeOddsMeanedUpTo(double mean, int maxVal)
{
    std::vector<std::pair<int, double>> sizeProbs;

    int cntH = 0;
    double sumH = 0;

    for (int i = 5; i <= maxVal; i += 2)
    {
        sumH += i;
        cntH += 1;
    }

    double meanH = sumH / cntH;

    double p3 = meanH - mean / (meanH - 3);

    double sumP = p3;

    sizeProbs.push_back({3, p3});

    for (int i = 5; i <= maxVal; i += 2)
    {
        sizeProbs.push_back({i, (1 - p3) / cntH});
        sumP += sizeProbs.back().second;
    }

    assert(std::abs(sumP - 1) < 1e-6);

    return sizeProbs;
}

const int N = 16000;

std::vector<TestSpec> testSpecs;

void addStandardSet(const std::string& name, const std::vector<std::pair<int, double>>& shapes, bool simple = false, bool perfOnly = false)
{
    double totalP = 0;

    for (auto [sz, p] : shapes)
    {
        totalP += p;
    }

    if (abs(totalP - 1) > 1e-6)
    {
        std::cerr << "ERROR in:" << name << ": " << totalP << std::endl;
    }

    assert(abs(totalP - 1) <= 1e-6);

    std::vector<TestSpec> newSpecs = {
        {
            .name = name + ", star",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " edges, star",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix, star",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + ", iPath star",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.3 * N),
        },
        {
            .name = name + ", path rng",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = name + " edges, path rng",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = name +  " mix, path rng",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = name + " mix, path apples cyc",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
            .doAppleCycle = true
        },
        {
            .name = name + " mix 0.95, path apples cyc",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.95,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
            .doAppleCycle = true
        },
        {
            .name = name + " mix 0.25, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.25,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.7, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.7,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.75, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.75,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.75, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.75,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.8, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.8,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.85, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.85,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.9, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.9,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.92, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.92,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.935, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.935,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.95, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.95,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.965, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.965,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.975, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.975,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.98, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.98,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.985, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.985,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.9925, path apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.9925,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + ", rng",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = name + " edges, rng",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = name + " mix, rng",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = name + ", iPath rng",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.3 * N),
        },
        {
            .name = name + " edges, rng apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix, rng apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.75, rng apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.75,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.85, rng apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.85,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.9, rng apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.9,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.925, rng apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.925,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.95, rng apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.9,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = name + " mix 0.975, rng apples",
            .n = N,
            .cycleSizeProbs = shapes,
            .sharedParNodeProb = 0.975,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
        },
    };

    std::vector<TestSpec> simpleNewSpecs;
    for (TestSpec spec : newSpecs)
    {
        if (spec.sharedParNodeProb == 0 || spec.sharedParNodeProb == 1 || spec.sharedParNodeProb == 0.5 || spec.sharedParNodeProb == 0.95)
        {
            simpleNewSpecs.push_back(spec);
        }
    }

    if (simple) newSpecs = simpleNewSpecs;

    std::vector<TestSpec> perfOnlyNewSpecs;
    for (TestSpec spec : newSpecs)
    {
        if (spec.sharedParNodeProb > 0.5)
        {
            perfOnlyNewSpecs.push_back(spec);
        }
    }

    if (perfOnly) newSpecs = perfOnlyNewSpecs;

    std::cout << "Adding " << name << ": " << newSpecs.size() << std::endl;

    testSpecs.insert(testSpecs.end(), newSpecs.begin(), newSpecs.end());
}

std::vector<std::pair<int, double>> NODES = {{1, 1.0}};
std::vector<std::pair<int, double>> FULLCYCLE = {{N, 1}};

std::vector<std::pair<int, double>> TRIANGLES = {{3, 1.0}};
std::vector<std::pair<int, double>> SQUARES = {{4, 1.0}};
std::vector<std::pair<int, double>> PENTS = {{5, 1.0}};
std::vector<std::pair<int, double>> HEXES = {{6, 1.0}};
std::vector<std::pair<int, double>> SEPTS = {{7, 1.0}};
std::vector<std::pair<int, double>> OCTS = {{8, 1.0}};
std::vector<std::pair<int, double>> DECS = {{10, 1.0}};

std::vector<std::pair<int, double>> TRIS_FOURS = {{3, 0.50}, {4, 0.50}};
std::vector<std::pair<int, double>> TRIS_HEXES = {{3, 0.50}, {6, 0.50}};

std::vector<std::pair<int, double>> TRIS_UNDECS = {{3, 0.75}, {11, 0.25}};
std::vector<std::pair<int, double>> ODDS_MEAN_5 = makeOddsMeanedUpTo(5, 50);

std::vector<std::pair<int, double>> TRIS_TRIDECS = {{3, 0.55}, {13, 0.45}};
std::vector<std::pair<int, double>> MANY_SMALLISH = {{2, 0.10}, {4, 0.10}, {5, 0.10}, {6, 0.10}, {7, 0.20}, {8, 0.15}, {9, 0.04}, {10, 0.04}, {12, 0.04}, {13, 0.04}, {15, 0.03}, {17, 0.03}, {20, 0.03}};

std::vector<std::pair<int, double>> ALMOST_NODES = {{1, 0.95}, {3, 0.01}, {4, 0.01}, {5, 0.01}, {6, 0.005}, {7, 0.005}, {8, 0.005}, {9, 0.005}};
std::vector<std::pair<int, double>> ALMOST_TRIANGLES = {{1, 0.01}, {3, 0.95}, {4, 0.01}, {5, 0.01}, {6, 0.005}, {7, 0.005}, {8, 0.005}, {9, 0.005}};
std::vector<std::pair<int, double>> ALMOST_SQUARES = {{1, 0.01}, {3, 0.01}, {4, 0.95}, {5, 0.01}, {6, 0.005}, {7, 0.005}, {8, 0.005}, {9, 0.005}};
std::vector<std::pair<int, double>> ALMOST_PENTS = {{1, 0.01}, {3, 0.01}, {4, 0.95}, {5, 0.01}, {6, 0.005}, {7, 0.005}, {8, 0.005}, {9, 0.005}};
std::vector<std::pair<int, double>> ALMOST_SEPTS = {{1, 0.01}, {3, 0.01}, {4, 0.95}, {5, 0.01}, {6, 0.005}, {7, 0.005}, {8, 0.005}, {9, 0.005}};
std::vector<std::pair<int, double>> ALMOST_NODTRIS = {{1, 0.48}, {3, 0.48}, {4, 0.01}, {5, 0.005}, {6, 0.005}, {7, 0.005}, {8, 0.005}, {9, 0.005}, {10, 0.005}};

std::vector<std::pair<int, double>> SMALLS = {{1, 0.1}, {3, 0.25}, {4, 0.2}, {5, 0.15}, {6, 0.1}, {7, 0.05}, {8, 0.05}, {9, 0.05}, {10, 0.05}};
std::vector<std::pair<int, double>> MIDS = makeUpTo(sqrt(N));
std::vector<std::pair<int, double>> DOTS = makeUpTo(sqrt(N), true);
std::vector<std::pair<int, double>> BIGS = makeUpTo(N / 5);

std::vector<std::pair<int, double>> ANTI_ARITH = {
    { 3, 0.3820 },
    { 4, 0.2361 },
    { 5, 0.1459 },
    { 6, 0.0902 },
    { 7, 0.0557 },
    { 8, 0.0344 },
    { 9, 0.0213 },
    { 10, 0.0132 },
    { 11, 0.0081 },
    { 12, 0.0050 },
    { 13, 0.0031 },
    { 14, 0.0019 },
    { 15, 0.0012 },
    { 16, 0.0007 },
    { 17, 0.0005 },
    { 18, 0.0003 },
    { 19, 0.0002 },
    { 20, 0.0001 },
    { 21, 0.0001 },
};

std::vector<std::pair<int, double>> ANTI_ARITH_CEIL = {
    { 3, 0.5000 },
    { 4, 0.2900 },
    { 5, 0.1250 },
    { 6, 0.0313 },
    { 7, 0.0312 },
    { 8, 0.0079 },
    { 9, 0.0078 },
    { 10, 0.0039 },
    { 11, 0.0010 },
    { 12, 0.0010 },
    { 13, 0.0005 },
    { 14, 0.0002 },
    { 15, 0.0001 },
    { 16, 0.0001 },
};

std::vector<std::pair<int, double>> ANTI_HUFFMAN = {
    { 3, 0.3814 },
    { 4, 0.2369 },
    { 5, 0.1446 },
    { 6, 0.0924 },
    { 7, 0.0534 },
    { 8, 0.0379 },
    { 9, 0.0199 },
    { 10, 0.0135 },
    { 11, 0.0090 },
    { 12, 0.0037 },
    { 13, 0.0036 },
    { 14, 0.0022 },
    { 15, 0.0009 },
    { 16, 0.0005 },
    { 17, 0.0001 },
};

std::vector<std::pair<int, double>> ANTI_HUFFMAN_UND = {
    { 3, 0.3807 },
    { 4, 0.2385 },
    { 5, 0.1422 },
    { 6, 0.0963 },
    { 7, 0.0558 },
    { 8, 0.0296 },
    { 9, 0.0262 },
    { 10, 0.0141 },
    { 11, 0.0071 },
    { 12, 0.0036 },
    { 13, 0.0023 },
    { 14, 0.0014 },
    { 15, 0.0008 },
    { 16, 0.0006 },
    { 17, 0.0003 },
    { 18, 0.0003 },
    { 19, 0.0002 },    
};

std::vector<std::pair<int, double>> ANTI_HUFFMAN_UNU = {
    { 3, 0.3421 },
    { 4, 0.1577 },
    { 5, 0.1577 },
    { 6, 0.0830 },
    { 7, 0.0748 },
    { 8, 0.0424 },
    { 9, 0.0413 },
    { 10, 0.0240 },
    { 11, 0.0178 },
    { 12, 0.0175 },
    { 13, 0.0105 },
    { 14, 0.0091 },
    { 15, 0.0083 },
    { 16, 0.0051 },
    { 17, 0.0028 },
    { 18, 0.0020 },
    { 19, 0.0013 },
    { 20, 0.0009 },
    { 21, 0.0006 },
    { 22, 0.0004 },
    { 23, 0.0003 },
    { 24, 0.0002 },
    { 25, 0.0002 },
};

void init()
{
    std::vector<TestSpec> weirdSpecs = {
        {
            .name = "nodes, star",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "nodes, path",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "nodes, path ears",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_RNG,
            .addEars = true,
        },
        {
            .name = "nodes, iPath star",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.3 * N),
        },
        {
            .name = "nodes, rng",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "nodes, rng",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "nodes, iPath 0.1 rng",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.1 * N),
        },
        {
            .name = "nodes, iPath rng",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.3 * N),
        },
        {
            .name = "nodes, iPath 0.6 rng",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.6 * N),
        },
        {
            .name = "nodes, iPath 0.9 rng",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.9 * N),
        },
        {
            .name = "nodes, iPath 0.98 rng",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.98 * N),
        },
        {
            .name = "nodes, iPath 0.99 rng",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.99 * N),
        },
        {
            .name = "nodes, iPath 0.995 rng",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.995 * N),
        },
        {
            .name = "nodes, iPath 0.98 rng cycle",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
            .doAppleCycle = true,
            .initPathH = (int) (0.98 * N),
        },
        {
            .name = "nodes, iPath 0.99 rng cycle",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
            .doAppleCycle = true,
            .initPathH = (int) (0.99 * N),
        },
        {
            .name = "nodes, iPath 0.995 rng cycle",
            .n = N,
            .cycleSizeProbs = NODES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
            .doAppleCycle = true,
            .initPathH = (int) (0.995 * N),
        },

        {
            .name = "almost_nodes mix, star",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "almost_nodes mix, path",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "almost_nodes mix, path ears",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_RNG,
            .addEars = true,
        },
        {
            .name = "almost_nodes mix, iPath star",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.3 * N),
        },
        {
            .name = "almost_nodes mix, rng",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "almost_nodes mix, iPath rng",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.3 * N),
        },
        {
            .name = "almost_nodes mix, iPath 0.9 rng",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.9 * N),
        },
        {
            .name = "almost_nodes mix, iPath 0.98 rng",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.98 * N),
        },
        {
            .name = "almost_nodes mix, iPath 0.99 rng",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.99 * N),
        },
        {
            .name = "almost_nodes mix, iPath 0.995 rng",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.995 * N),
        },
        {
            .name = "almost_nodes mix, iPath 0.99 rng cycle",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
            .doAppleCycle = true,
            .initPathH = (int) (0.98 * N),
        },
        {
            .name = "almost_nodes mix, iPath 0.99 rng cycle",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
            .doAppleCycle = true,
            .initPathH = (int) (0.99 * N),
        },
        {
            .name = "almost_nodes mix, iPath 0.995 rng cycle",
            .n = N,
            .cycleSizeProbs = ALMOST_NODES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
            .doAppleCycle = true,
            .initPathH = (int) (0.995 * N),
        },

        {
            .name = "fullcycle",
            .n = N,
            .cycleSizeProbs = FULLCYCLE,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "fullcycle, ears",
            .n = N,
            .cycleSizeProbs = FULLCYCLE,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .addEars = true,
        },
        {
            .name = "fullcycle, iPath",
            .n = N,
            .cycleSizeProbs = FULLCYCLE,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.3 * N),
        },
        {
            .name = "fullcycle, iPath 0.5",
            .n = N,
            .cycleSizeProbs = FULLCYCLE,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.5 * N),
        },
        {
            .name = "fullcycle, iPath 0.15 path",
            .n = N,
            .cycleSizeProbs = FULLCYCLE,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.15 * N),
        },
        {
            .name = "fullcycle edges, iPath 0.65",
            .n = N,
            .cycleSizeProbs = FULLCYCLE,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.65 * N),
        },
        {
            .name = "fullcycle edges, iPath 0.4 ears",
            .n = N,
            .cycleSizeProbs = FULLCYCLE,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.4 * N),
            .addEars = true,
        },

        {
            .name = "triangles, rng beads",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_BEADS,
        },
        {
            .name = "triangles edges, rng beads",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_BEADS,
        },
        {
            .name = "triangles mix, rng beads",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_BEADS,
        },
        {
            .name = "triangles mix 0.2, rng",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.2,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "triangles mix 0.35, rng",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.35,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "triangles mix 0.65, rng",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.65,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "triangles mix 0.8, rng",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.8,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
        },
        {
            .name = "triangles mix, path beads ears",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.5,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_BEADS,
            .addEars = true
        },
        {
            .name = "triangles mix 0.65, path apples",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.65,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = "triangles mix 0.675, path apples",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.675,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = "triangles mix 0.6875, path apples",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.6875,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = "triangles mix 0.725, path apples",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.75,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = "triangles mix 0.75, path apples",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.75,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = "triangles, iPath 0.26 star",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.26 * N),
        },
        {
            .name = "triangles, iPath 0.26 star",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.26 * N),
        },
        {
            .name = "triangles, iPath 0.26 star",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.26 * N),
        },
        {
            .name = "triangles, iPath 0.26 star",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.26 * N),
        },
        {
            .name = "triangles, iPath 0.26 star",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.26 * N),
        },
        {
            .name = "triangles, iPath 0.26 star",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.26 * N),
        },
        {
            .name = "triangles, iPath 0.26 star",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.26 * N),
        },
        {
            .name = "triangles, iPath 0.26 star",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.26 * N),
        },
        {
            .name = "triangles, iPath 0.26 star",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.26 * N),
        },
        {
            .name = "triangles, iPath 0.26 star",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_STAR,
            .parNodeMode = PNM_APPLES,
            .initPathH = (int) (0.26 * N),
        },
        {
            .name = "triangles mix 0.75, path apples ears",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.75,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
            .addEars = true
        },
        {
            .name = "triangles mix 0.93, path apples ears",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.93,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
            .addEars = true
        },
        {
            .name = "triangles mix 0.97, path apples ears",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0.97,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_APPLES,
            .addEars = true
        },
        {
            .name = "triangles, iPath 0.9 rng",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.9 * N)
        },
        {
            .name = "triangles, iPath 0.98 rng",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.98 * N)
        },
        {
            .name = "triangles, iPath 0.99 rng",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.99 * N)
        },
        {
            .name = "triangles, iPath 0.995 rng",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.995 * N)
        },
        {
            .name = "triangles edges, iPath 0.99 rng",
            .n = N,
            .cycleSizeProbs = TRIANGLES,
            .sharedParNodeProb = 0,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.99 * N)
        },

        {
            .name = "squares, iPath 0.99 rng",
            .n = N,
            .cycleSizeProbs = SQUARES,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.99 * N)
        },
        {
            .name = "pents, iPath 0.99 rng",
            .n = N,
            .cycleSizeProbs = PENTS,
            .sharedParNodeProb = 1,
            .parCycleMode = PCM_RNG,
            .parNodeMode = PNM_RNG,
            .initPathH = (int) (0.99 * N)
        },

        {
            .name = "octs mix 0.94, path apples",
            .n = N,
            .cycleSizeProbs = OCTS,
            .sharedParNodeProb = 0.94,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
        {
            .name = "octs mix 0.96, path apples",
            .n = N,
            .cycleSizeProbs = OCTS,
            .sharedParNodeProb = 0.96,
            .parCycleMode = PCM_PATH,
            .parNodeMode = PNM_APPLES,
        },
    };

    std::cout << "Adding weird: " << weirdSpecs.size() << std::endl;

    testSpecs.insert(testSpecs.end(), weirdSpecs.begin(), weirdSpecs.end());

    addStandardSet("almost_nodtris", ALMOST_NODTRIS, true);
    addStandardSet("almost_triangles", ALMOST_TRIANGLES, true);
    addStandardSet("almost_squares", ALMOST_SQUARES, true);
    addStandardSet("almost_pents", ALMOST_PENTS, true);
    addStandardSet("almost_septs", ALMOST_SEPTS, true);

    addStandardSet("smalls", SMALLS, true);
    addStandardSet("mids", MIDS, true);
    addStandardSet("dots", DOTS, true);
    addStandardSet("bigs", BIGS, true);

    addStandardSet("triangles", TRIANGLES);
    addStandardSet("squares", SQUARES);
    addStandardSet("pents", PENTS);
    addStandardSet("hexes", HEXES, false, true);
    addStandardSet("septs", SEPTS, false, true);
    addStandardSet("octs", OCTS, false, true);
    addStandardSet("decs", DECS, false, true);

    addStandardSet("tris_fours", TRIS_FOURS, false, true);

    addStandardSet("tris_hexes", TRIS_HEXES, false, true);

    addStandardSet("tris_undecs", TRIS_UNDECS, false, true);
    addStandardSet("odds_mean_5", ODDS_MEAN_5, false, true);

    addStandardSet("tris_tridecs", TRIS_TRIDECS, false, true);
    addStandardSet("many_smallish", MANY_SMALLISH, false, true);

    addStandardSet("anti_arith", ANTI_ARITH, false, true);
    addStandardSet("anti_arith_ceil", ANTI_ARITH_CEIL, false, true);
    addStandardSet("anti_huffman", ANTI_HUFFMAN);
    addStandardSet("anti_huffman_und", ANTI_HUFFMAN_UND, false, true);
    addStandardSet("anti_huffman_unu", ANTI_HUFFMAN_UNU, false, true);

    std::cout << "Total subtests: " << testSpecs.size() << std::endl;
}

const int T = 5;

int main()
{
    std::ios::sync_with_stdio(false);

    init();

    std::vector<int> perm(testSpecs.size());
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), generator);

    assert(testSpecs.size() % T == 0);

    std::vector<std::vector<int>> batches(testSpecs.size() / T);
    for (int i = 0; i < (int) testSpecs.size(); i++)
    {
        batches[i / T].push_back(perm[i]);
    }

    int num = 1;

    for (const std::vector<int>& batch : batches)
    {
        std::cout << std::endl;
        std::cout << std::endl;

        std::string nums = "";
        nums += (num / 100 + '0');
        nums += (num / 10 % 10 + '0');
        nums += (num % 10 + '0');

        std::ofstream dummySol("cactus." + nums + ".sol");
        std::ofstream outfile("cactus." + nums + ".in");

        std::cout << "Test " << num << ":" << std::endl;

        outfile << T << std::endl;

        for (int i : batch)
        {
            const TestSpec& ts = testSpecs[i];

            std::cout << std::endl;
            std::cout << ts.name << std::endl;

            genTest(outfile, ts);    
        }

        num++;
    }

    return 0;
}
