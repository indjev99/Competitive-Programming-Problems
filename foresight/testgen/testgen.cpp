#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <random>
#include <string>

const int MAX_N = 1e6;

std::mt19937 generator;
int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(generator);
}
bool oneIn(int chances)
{
    std::uniform_int_distribution<int> distribution(0, chances - 1);
    return distribution(generator) == 0;
}

#define RANDOM 0
#define CYCLE 1
#define CROSS_CYCLE 2
#define SPARSE_CROSS_CYCLE 3
#define SQRT_CYCLES 4
#define SPARSE_CROSS_SQRT_CYCLES 5
#define COMP_TREE 6
#define COMP_TREE_REV 7
#define COMP_CHAIN_ISH 8
#define TREE 9
#define TREE_REV 10
#define BAL_TREE 11
#define CYCLE_PATH 12
#define PATH_CYCLE 13

int n, m, s, t;
int type;

std::vector<std::pair<int, int>> edges;
std::vector<int> perm;

void genTest(std::ofstream& outfile)
{
    edges.clear();
    perm.clear();

    --n;
    m -= 2;

    if (type == RANDOM)
    {
        for (int i = 0; i < m; ++i)
        {
            edges.emplace_back(randNum(0, n), randNum(0, n));
        }
        s = randNum(0, n);
        t = randNum(0, n);
    }
    else if (type == CYCLE)
    {
        for (int i = 0; i < n; ++i)
        {
            edges.emplace_back(i, (i + 1) % n);
        }
        s = randNum(0, n);
        t = randNum(0, n);
    }
    else if (type == CROSS_CYCLE)
    {
        t = 0;
        for (int i = 0; i < n; ++i)
        {
            edges.emplace_back(i, (i + 1) % n);
        }
        for (int i = 1; i <= n / 2; ++i)
        {
            edges.emplace_back(i, (n - i) % n);
        }
        s = randNum(0, n);
    }
    else if (type == SPARSE_CROSS_CYCLE)
    {
        t = 0;
        for (int i = 0; i < n; ++i)
        {
            edges.emplace_back(i, (i + 1) % n);
        }
        for (int i = 1; i <= n / 2; ++i)
        {
            if (oneIn(10))
            {
                edges.emplace_back(i, (n - i) % n);
            }
        }
        s = randNum(0, n);
    }
    else if (type == SQRT_CYCLES)
    {
        t = 0;
        s = n - 1;
        int numCycles = sqrt(n);
        for (int i = 0; i < numCycles; ++i)
        {
            int start = n * i / numCycles;
            int stop = n * (i + 1) / numCycles;
            int nextStop = n * (i + 2) / numCycles;
            int k = stop - start;
            for (int i = 0; i < k; ++i)
            {
                edges.emplace_back(start + i, start + (i + 1) % k);
            }
            if (nextStop <= n)
            {
                edges.emplace_back(randNum(start, stop), randNum(stop, nextStop));
                edges.emplace_back(randNum(start, stop), randNum(stop, nextStop));  
            }
        }
    }
    else if (type == SPARSE_CROSS_SQRT_CYCLES)
    {
        t = 0;
        s = n - 1;
        int numCycles = sqrt(n);
        for (int i = 0; i < numCycles; ++i)
        {
            int start = n * i / numCycles;
            int stop = n * (i + 1) / numCycles;
            int nextStop = n * (i + 2) / numCycles;
            int k = stop - start;
            for (int i = 0; i < k; ++i)
            {
                edges.emplace_back(start + i, start + (i + 1) % k);
            }
            if (oneIn(2))
            {
                edges.emplace_back(randNum(start, stop), randNum(start, stop));
            }
            if (nextStop <= n)
            {
                edges.emplace_back(randNum(start, stop), randNum(stop, nextStop));
                edges.emplace_back(randNum(start, stop), randNum(stop, nextStop));  
            }
        }
    }
    else if (type == COMP_TREE || type == COMP_TREE_REV)
    {
        if (type == COMP_TREE)
        {
            t = 0;
            s = n - 1;
        }
        else
        {
            s = n - 1;
            t = 0;
        }

        int numComps = (n - 1) / 100;
        std::vector<int> dists(numComps, -1);
        for (int i = 0; i < numComps; ++i)
        {
            int start = (n - 1) * i / numComps;
            int stop = (n - 1) * (i + 1) / numComps;

            int h = (stop - start) * 10 - 2;
            for (int i = 0; i < h; ++i)
            {
                edges.emplace_back(randNum(start, stop), randNum(start, stop));
            }

            if (i > 0)
            {
                int parComp = randNum(0, i);

                dists[i] = 1 + dists[parComp];

                int parStart = (n - 1) * parComp / numComps;
                int parStop = (n - 1) * (parComp + 1) / numComps;
                edges.emplace_back(randNum(start, stop), randNum(parStart, parStop));
                edges.emplace_back(randNum(start, stop), randNum(parStart, parStop));
            }
            else dists[i] = 0;
        }

        int tDist = 0;
        for (int i = 0; i < numComps; ++i)
        {
            tDist = std::max(tDist, dists[i]);
        }
    
        int tDistCnt = 0;
        for (int i = 0; i < numComps && (int) edges.size() < m; ++i)
        {
            if (dists[i] == tDist)
            {
                ++tDistCnt;
    
                int start = (n - 1) * i / numComps;
                int stop = (n - 1) * (i + 1) / numComps;
                edges.emplace_back(randNum(start, stop), n - 1);
            }
        }

        if (tDistCnt == 1)
        {
            edges.emplace_back(n - 2, n - 1);
        }
    }
    else if (type == COMP_CHAIN_ISH)
    {
        t = 0;
        s = n - 1;

        int numComps = n / 100;
        for (int i = 0; i < numComps; ++i)
        {
            int start = n * i / numComps;
            int stop = n * (i + 1) / numComps;

            int h = (stop - start) * 10 - 6;
            for (int i = 0; i < h; ++i)
            {
                edges.emplace_back(randNum(start, stop), randNum(start, stop));
            }

            if (i > 0)
            {
                int parComp = randNum(std::max(0, i - 3), i);
                int parStart = n * parComp / numComps;
                int parStop = n * (parComp + 1) / numComps;
                edges.emplace_back(randNum(start, stop), randNum(parStart, parStop));
                edges.emplace_back(randNum(start, stop), randNum(parStart, parStop));
            }
            if (i > 0)
            {
                int parComp = randNum(std::max(0, i - 3), i);
                int parStart = n * parComp / numComps;
                int parStop = n * (parComp + 1) / numComps;
                edges.emplace_back(randNum(start, stop), randNum(parStart, parStop));
                edges.emplace_back(randNum(start, stop), randNum(parStart, parStop));
            }
            if (i > 0)
            {
                int parComp = randNum(std::max(0, i - 3), i);
                int parStart = n * parComp / numComps;
                int parStop = n * (parComp + 1) / numComps;
                edges.emplace_back(randNum(start, stop), randNum(parStart, parStop));
                edges.emplace_back(randNum(start, stop), randNum(parStart, parStop));
            }
        }
    }
    else if (type == TREE || type == TREE_REV)
    {
        if (type == TREE)
        {
            t = 0;
            s = n - 1;
        }
        else
        {
            s = n - 1;
            t = 0;
        }

        std::vector<int> dists(n, -1);
        std::vector<std::vector<int>> layers;
        for (int i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                int par = randNum(0, i);
                dists[i] = 1 + dists[par];
                edges.emplace_back(i, par);
            }
            else dists[i] = 0;

            while ((int) layers.size() <= dists[i]) layers.emplace_back();
            layers[dists[i]].push_back(i);
        }

        while ((int) edges.size() < m)
        {
            int k = randNum(0, layers.size());
            int i = randNum(0, layers[k].size());
            int j = randNum(0, layers[k].size());
            edges.emplace_back(layers[k][i], layers[k][j]);
        }
    }
    else if (type == BAL_TREE)
    {
        t = 0;
        s = n - 1;

        std::vector<int> dists(n, -1);
        std::vector<std::vector<int>> layers;
        for (int i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                int par = (i - 1) / 2;
                dists[i] = 1 + dists[par];
                edges.emplace_back(i, par);
            }
            else dists[i] = 0;

            while ((int) layers.size() <= dists[i]) layers.emplace_back();
            layers[dists[i]].push_back(i);
        }

        while ((int) edges.size() < m)
        {
            int k = layers.size() - 1;
            int i = randNum(0, layers[k].size());
            int j = randNum(0, layers[k].size());
            edges.emplace_back(layers[k][i], layers[k][j]);
        }
    }
    else if (type == CYCLE_PATH)
    {
        t = 0;
        int cycLen = n * 2 / 5;
        s = cycLen / 2;
        for (int i = 0; i < cycLen; ++i)
        {
            edges.emplace_back(i, (i + 1) % cycLen);
        }
        edges.emplace_back(s, cycLen);
        edges.emplace_back(s, cycLen);
        for (int i = cycLen; i < n; ++i)
        {
            edges.emplace_back(i, (i + 1) % n);
            edges.emplace_back(i, (i + 1) % n);
        }
    }
    else if (type == PATH_CYCLE)
    {
        t = 0;
        int cycLen = n * 2 / 5 + 4;
        s = cycLen / 2;
        for (int i = 0; i < cycLen; ++i)
        {
            edges.emplace_back(i, (i + 1) % cycLen);
        }
        edges.emplace_back(s, cycLen);
        edges.emplace_back(s, cycLen);
        for (int i = cycLen; i < n; ++i)
        {
            edges.emplace_back(i, (i + 1) % n);
            edges.emplace_back(i, (i + 1) % n);
        }
    }

    edges.emplace_back(t, n);
    edges.emplace_back(t, n);
    t = n++;
    m += 2;

    perm.resize(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), generator);

    s = perm[s];
    t = perm[t];

    std::shuffle(edges.begin(), edges.end(), generator);
    for (auto& [a, b] : edges)
    {
        a = perm[a];
        b = perm[b];
        if (oneIn(2)) std::swap(a, b);
    }

    assert(0 <= s && s < n);
    assert(0 <= t && t < n);
    assert((int) edges.size() <= m);
    outfile << n << " " << edges.size() << " " << s << " " << t << "\n";
    for (auto& [a, b] : edges)
    {
        assert(0 <= a && a < n);
        assert(0 <= b && b < n);
        outfile << a << " " << b << "\n";
    }
}

struct Test
{
    int type;
    int mult;
};

std::vector<Test> easy = {
    {BAL_TREE, 10},
    {CYCLE_PATH, 10},
    {PATH_CYCLE, 10},
    {RANDOM, 2},
    {RANDOM, 10},
    {SQRT_CYCLES, 10}
};

std::vector<Test> mid = {
    {BAL_TREE, 10},
    {CYCLE_PATH, 10},
    {PATH_CYCLE, 10},
    {RANDOM, 2},
    {RANDOM, 10},
    {SQRT_CYCLES, 10},
    {COMP_CHAIN_ISH, 10},
    {TREE, 2},
    {TREE_REV, 10}
};

std::vector<Test> hard = {
    {BAL_TREE, 10},
    {CYCLE_PATH, 10},
    {PATH_CYCLE, 10},
    {RANDOM, 1},
    {RANDOM, 2},
    {RANDOM, 10},
    {CYCLE, 10},
    {SPARSE_CROSS_CYCLE, 10},
    {SQRT_CYCLES, 10},
    {SPARSE_CROSS_SQRT_CYCLES, 10},
    {COMP_TREE, 10},
    {COMP_TREE_REV, 10},
    {COMP_CHAIN_ISH, 10},
    {TREE, 2},
    {TREE, 10},
    {TREE_REV, 2},
    {TREE_REV, 10}
};

int main()
{
    int num, meta;
    while (true)
    {
        num = 0;

        std::cout << "Test num? ";
        std::cin >> num;

        if (num <= 0 || num > 99) break;

        std::cout << "MAX_N Meta? ";
        std::cin >> n >> meta;

        std::vector<Test> *tests;
        if (meta == 0) tests = &easy;
        if (meta == 1) tests = &mid;
        if (meta == 2) tests = &hard;

        for (Test test : *tests)
        {
            type = test.type;
            m = test.mult * n;

            std::string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');
    
            std::string name = "foresight." + nums + ".in";
            std::cerr << name << " " << type << " " << m << std::endl;
    
            std::ofstream outfile(name);
            genTest(outfile);
            outfile.close();

            ++num;
        }
    }
    return 0;
}
