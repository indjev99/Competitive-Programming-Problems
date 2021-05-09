#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <math.h>
#include <random>

using namespace std;

mt19937 generator;

int randNum(int lb, int ub)
{
    uniform_int_distribution<int> distribution(lb, ub);
    return distribution(generator);
}

bool oneIn(int chances)
{
    uniform_int_distribution<int> distribution(0, chances - 1);
    return distribution(generator) == 0;
}

const double M_POW = 1.5;

int n, m, q;

struct EQ
{
    int from, to;

    EQ() = default;
    EQ(int from, int to): from(from), to(to) {}
};

bool operator==(const EQ& left, const EQ& right)
{
    return (left.from == right.from && left.to == right.to) || (left.from == right.to && left.to == right.from);
}

struct EQ_Hash
{
    std::size_t operator()(const EQ& curr) const
    {
        std::size_t h1 = std::hash<int>{}(curr.from);
        std::size_t h2 = std::hash<int>{}(curr.to);
        return ((h1 + h2) << 12) ^ (h1 * h2);
    }
};

std::vector<EQ> missing;
std::vector<EQ> queries;

std::vector<int> nums;

void shuffleEQs(std::vector<EQ>& eqs)
{
    for (EQ& curr : eqs)
    {
        curr.from = nums[curr.from];
        curr.to = nums[curr.to];
        if (oneIn(2)) std::swap(curr.from, curr.to);
    }

    std::shuffle(eqs.begin(), eqs.end(), generator);
}

void shuffleEverything()
{
    nums.resize(n);
    std::iota(nums.begin(), nums.end(), 0);

    std::shuffle(nums.begin(), nums.end(), generator);

    shuffleEQs(missing);
    shuffleEQs(queries);
}

#define RANDOM 0
#define TREE 1
#define PATH 2
#define MAX_INTERESTING 3
#define NO_OVERLAP 4

int type;
bool odd;
bool manyIntr;
bool disconnect;

std::unordered_set<EQ, EQ_Hash> missingSet;

const int MAX_N = 1e5;

int numMiss[MAX_N];

bool addMissing(int from, int to)
{
    if (from == to || missingSet.find({from, to}) != missingSet.end()) return false;
    missingSet.insert({from, to});
    missing.push_back({from, to});
    ++numMiss[from];
    ++numMiss[to];
    return true;
}

int par[MAX_N];

void genTest(ostream& outfile)
{
    missing.clear();
    queries.clear();

    missingSet.clear();
    std::fill(numMiss, numMiss + n, 0);

    if (odd) --n;

    m = pow(n, M_POW);

    int numIntr = m / n;

    if (manyIntr) --numIntr;
    else numIntr /= 2;

    if (type == RANDOM)
    {
        while (missing.size() < m)
        {
            int from, to;
            do
            {
                from = randNum(0, n - 1);
                to = randNum(0, n - 1);
            }
            while (!addMissing(from, to));
        }
    }
    else if (type == TREE || type == PATH)
    {
        std::vector<int> entryPoints;
        if (type == PATH)
        {
            for (int i = 1; i < numIntr; ++i)
            {
                par[i] = i - 1;
            }
            int mid = randNum(1, numIntr - 1);
            par[0] = randNum(numIntr, n - 1);
            par[mid] = randNum(numIntr, n - 1);
            entryPoints = {par[0], par[mid]};
        }
        else if (type == TREE)
        {
            for (int i = 0; i < numIntr; ++i)
            {
                par[i] = randNum(-1, i - 1);
                if (par[i] == -1)
                {
                    par[i] = randNum(numIntr, n - 1);
                    if (entryPoints.size() == 2) par[i] = entryPoints.back();
                    if (entryPoints.size() < 3) entryPoints.push_back(par[i]);
                }
            }
            if (type == TREE && disconnect)
            {
                for (int i = 0; i < numIntr; ++i)
                {
                    if (i != 3 && par[i] < i)
                    {
                        par[i] = -1;
                        break;
                    }
                }
            }
        }
        EQ extraEdge;
        do
        {
            extraEdge.from = randNum(numIntr / 4, numIntr - 1);
            extraEdge.to = randNum(numIntr / 4, numIntr - 1);
        }
        while (extraEdge.from == extraEdge.to || par[extraEdge.to] == extraEdge.from || par[extraEdge.from] == extraEdge.to);
        if (type != TREE || disconnect) extraEdge = {-1, -1};
        for (int i = 0; i < numIntr; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (i <= j && j < numIntr) continue;
                if (par[i] != j && !(extraEdge == EQ(i, j)))
                {
                    addMissing(i, j);
                }
            }
        }
        if (entryPoints.size() >= 2) addMissing(entryPoints[0], entryPoints[1]);
        while (missing.size() < m)
        {
            int from, to;
            do
            {
                from = randNum(numIntr, n - 1);
                to = randNum(numIntr, n - 1);
            }
            while (!addMissing(from, to));
        }
    }
    else if (type == MAX_INTERESTING)
    {
        int tresh = (n + 1) / 2;
        for (int i = 0; i < n && missing.size() < m; ++i)
        {
            for (int j = i + 1; j < n && numMiss[i] < tresh && missing.size() < m; ++j)
            {
                if (numMiss[j] >= tresh) continue;
                addMissing(i, j);
            }
        }
    }
    else if (type == NO_OVERLAP)
    {
        for (int i = 0; i < n; ++i)
        {
            if (i % 2 == 0) addMissing(1, i);
            else addMissing(0, i);
        }
        while (missing.size() < m)
        {
            int from, to;
            do
            {
                from = randNum(2, n - 1);
                to = randNum(2, n - 1);
            }
            while (!addMissing(from, to));
        }
    }

    queries = missing;

    if (queries.size() > q - 6)
    {
        std::shuffle(queries.begin() + 1, queries.end(), generator);
        queries.resize(q - 6);
    }

    queries.push_back({0, 0});
    queries.push_back({n - 1, n - 1});
    if ((type == TREE || type == PATH) && numIntr > 3) queries.push_back({3, par[3]});

    while (queries.size() < q)
    {
        int from = randNum(0, n - 1);
        int to = randNum(0, n - 1);
        queries.push_back({from, to});
    }

    shuffleEverything();

    assert(missing.size() == m);
    assert(queries.size() == q);

    outfile << n << " " << m << "\n";
    for (const EQ& curr : missing)
    {
        outfile << curr.from << " " << curr.to << "\n";
    }
    outfile << q << "\n";
    for (const EQ& curr : queries)
    {
        outfile << curr.from << " " << curr.to << "\n";
    }

    if (odd) ++n;
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        n = atoi(argv[1]);
        q = atoi(argv[2]);
        generator.seed(atoi(argv[3]));
        genTest(cout);
        return 0;
    }

    generator.seed(time(nullptr));

    int num;
    while (true)
    {
        cout << "Test num? ";
        cin >> num;

        if (num <= 0) break;

        cout << "n q? ";
        cin >> n >> q;

        for (int i = 0; i < 10; ++i)
        {
            if (i == 0)
            {
                type = RANDOM;
                odd = false;
            }
            else if (i == 1)
            {
                type = TREE;
                odd = false;
                manyIntr = false;
                disconnect = true;
            }
            else if (i == 2)
            {
                type = NO_OVERLAP;
                odd = true;
            }
            else if (i == 3)
            {
                type = PATH;
                odd = false;
                manyIntr = true;
            }
            else if (i == 4)
            {
                type = TREE;
                odd = true;
                manyIntr = false;
                disconnect = false;
            }
            else if (i == 5)
            {
                type = MAX_INTERESTING;
                odd = false;
            }
            else if (i == 6)
            {
                type = NO_OVERLAP;
                odd = false;
            }
            else if (i == 7)
            {
                type = TREE;
                odd = false;
                manyIntr = true;
                disconnect = true;
            }
            else if (i == 8)
            {
                type = PATH;
                odd = true;
                manyIntr = false;
            }
            else if (i == 9)
            {
                type = TREE;
                odd = true;
                manyIntr = true;
                disconnect = false;
            }

            string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');
            ofstream outfile("missing." + nums + ".in");
            genTest(outfile);

            ++num;
        }
    }
    return 0;
}
