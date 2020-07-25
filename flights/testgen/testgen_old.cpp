#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

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
#define SOLO_ZERO 1
#define ALL_ZERO 2
#define ALL_ZERO_ONE 3
#define ALL_ZERO_SOLO_ONE 4
#define CLUSTER 5

#define RANDOM_FLIGHTS 0
#define RANDOM_INC 1
#define PATH 2
#define TWO_PATHS 3
#define ALL_FROM_ZERO 4
#define ALL_TO_ZERO 5
#define ALL_TO_FROM_ZERO 6
#define ZERO_TO_ONE 7
#define ZERO_FROM_ONE 8
#define ZERO_TO_FROM_ONE 9
#define ZERO_TO_FROM_ONE_RANDOM 10
#define FROM_ZERO_TO_ONE 11
#define RANDOM_CLUSTER 12
#define PATH_CLUSTER 13

int n, m, t;
int gType, fType;

struct Edge
{
    int from, to;
};

bool operator==(const Edge& a, const Edge& b)
{
    return a.from == b.from && a.to == b.to;
}

struct EdgeHash
{
    std::size_t operator()(const Edge& edge) const
    {
        std::size_t h1 = std::hash<int>()(edge.from);
        std::size_t h2 = std::hash<int>()(edge.to);
        return h1 ^ h2;
    }
};

std::vector<Edge> edges;
std::vector<Edge> flights;
std::vector<int> edgesFrom[MAX_N];
std::unordered_set<Edge, EdgeHash> used;
std::vector<int> renumber;

bool addEdge(int from, int to)
{
    if (from == to) return false;
    if (from < 0 || from >= n || to < 0 || to >= n || from == to)
    {
        std::cerr << "Warning: out of bounds edge attempt: " << from << " " << to << " / " << n << std::endl;
        return false;
    }
    from = renumber[from];
    to = renumber[to];
    if (from > to) std::swap(from, to);
    Edge edge = {from, to};
    if (used.find(edge) == used.end())
    {
        used.insert(edge);
        edges.push_back(edge);
        edgesFrom[from].push_back(to);
        edgesFrom[to].push_back(from);
        return true;
    }
    return false;
}

void addFlight(int from, int to, bool discard=true)
{
    if (discard && from == to) return;
    if (from < 0 || from >= n || to < 0 || to >= n || from == to)
    {
        std::cerr << "ERROR: Out of bounds flight: " << from << " " << to << " / " << n << std::endl;
        return;
    }
    from = renumber[from];
    to = renumber[to];
    flights.push_back({from, to});
}

void genGraph()
{
    if (gType == RANDOM)
    {
        while (edges.size() < m)
        {
            addEdge(randNum(0, n), randNum(0, n));
        }
    }
    else if (gType == SOLO_ZERO)
    {
        while (edges.size() < m)
        {
            addEdge(randNum(1, n), randNum(1, n));
        }
    }
    else if (gType == ALL_ZERO)
    {
        for (int i = 1; i < n; ++i)
        {
            if (!oneIn(20)) addEdge(0, i);
        }
        while (edges.size() < m)
        {
            addEdge(randNum(1, n), randNum(1, n));
        }
    }
    else if (gType == ALL_ZERO_ONE)
    {
        for (int i = 2; i < n; ++i)
        {
            if (!oneIn(20)) addEdge(0, i);
            if (!oneIn(20)) addEdge(1, i);
        }
        while (edges.size() < m)
        {
            addEdge(randNum(2, n), randNum(2, n));
        }
    }
    else if (gType == ALL_ZERO_SOLO_ONE)
    {
        for (int i = 2; i < n; ++i)
        {
            if (!oneIn(20)) addEdge(0, i);
        }
        while (edges.size() < m)
        {
            addEdge(randNum(2, n), randNum(2, n));
        }
    }
    else if (gType == CLUSTER)
    {
        int cs = std::min(n - 1, (int) sqrt(m));
        for (int i = 0; i < cs; ++i)
        {
            for (int j = i + 1; j < cs; ++j)
            {
                if (!oneIn(5)) addEdge(i, j);
            }
            if (oneIn(2)) addEdge(i, cs);
        }
        while (edges.size() < m)
        {
            addEdge(randNum(0, n), randNum(0, n));
        }
    }
    std::shuffle(edges.begin(), edges.end(), generator);
}

void genFlights()
{
    if (fType == RANDOM_FLIGHTS)
    {
        while (flights.size() < t)
        {
            addFlight(randNum(0, n), randNum(0, n));
        }
    }
    else if (fType == RANDOM_INC)
    {
        while (flights.size() < t)
        {
            int from = randNum(0, n - 1);
            addFlight(from, randNum(from + 1, n));
        }
    }
    else if (fType == PATH)
    {
        int curr = randNum(0, n);
        while (flights.size() < t)
        {
            int from = curr;
            auto& es = edgesFrom[from];
            if (!oneIn(10) && !es.empty()) from = es[randNum(0, es.size())];
            int to = from;
            while (to == from) to = randNum(0, n);
            addFlight(from, to, false);
            curr = to;
        }
    }
    else if (fType == TWO_PATHS)
    {
        int currs[2];
        currs[0] = randNum(0, n);
        currs[1] = randNum(0, n);
        while (flights.size() < t)
        {
            int pick = randNum(0, 2);
            int from = currs[pick];
            auto& es = edgesFrom[from];
            if (!oneIn(10) && !es.empty()) from = es[randNum(0, es.size())];
            int to = from;
            while (to == from) to = randNum(0, n);
            addFlight(from, to, false);
            currs[pick] = to;
        }
    }
    else if (fType == ALL_FROM_ZERO)
    {
        while (flights.size() < t)
        {
            addFlight(0, randNum(1, n));
        }
    }
    else if (fType == ALL_TO_ZERO)
    {
        while (flights.size() < t)
        {
            addFlight(randNum(1, n), 0);
        }
    }
    else if (fType == ALL_TO_FROM_ZERO)
    {
        while (flights.size() < t)
        {
            if (oneIn(2)) addFlight(randNum(1, n), 0);
            else addFlight(0, randNum(1, n));
        }
    }
    else if (fType == ZERO_TO_ONE)
    {
        while (flights.size() < t)
        {
            addFlight(0, 1);
        }
    }
    else if (fType == ZERO_FROM_ONE)
    {
        while (flights.size() < t)
        {
            addFlight(1, 0);
        }
    }
    else if (fType == ZERO_TO_FROM_ONE)
    {
        while (flights.size() < t)
        {
            if (oneIn(2)) addFlight(0, 1);
            else addFlight(1, 0);
        }
    }
    else if (fType == ZERO_TO_FROM_ONE_RANDOM)
    {
        while (flights.size() < t)
        {
            if (!oneIn(10))
            {
                if (oneIn(2)) addFlight(0, 1);
                else addFlight(1, 0);
            }
            else addFlight(randNum(2, n), randNum(2, n));
        }
    }
    else if (fType == FROM_ZERO_TO_ONE)
    {
        while (flights.size() < t)
        {
            if (oneIn(2)) addFlight(0, randNum(2, n));
            else addFlight(randNum(2, n), 1);
        }
    }
    else if (fType == RANDOM_CLUSTER)
    {
        int cs = std::min(n - 1, (int) sqrt(m));
        while (flights.size() < t)
        {
            if (!oneIn(10)) addFlight(randNum(0, cs), randNum(0, cs));
            else if (oneIn(4)) addFlight(cs, randNum(0, n));
            else if (oneIn(3)) addFlight(randNum(0, n), cs);
            else addFlight(randNum(0, n), randNum(0, n));
        }
    }
    else if (fType == PATH_CLUSTER)
    {
        int cs = std::min(n - 1, (int) sqrt(m));
        int curr = randNum(0, n);
        while (flights.size() < t)
        {
            int from = curr;
            auto& es = edgesFrom[from];
            if (!oneIn(10) && !es.empty()) from = es[randNum(0, es.size())];
            int to = from;
            while (to == from)
            {
                if (!oneIn(10)) to = randNum(0, cs);
                else if (oneIn(2)) to = cs;
                else to = randNum(0, n);
            }
            addFlight(from, to, false);
            curr = to;
        }
    }
}

void genTest(std::ofstream& outfile)
{
    edges.clear();
    used.clear();
    flights.clear();
    for (int i = 0; i < n; ++i)
    {
        edgesFrom[i].clear();
    }
    renumber.resize(n);
    std::iota(renumber.begin(), renumber.end(), 0);
    std::shuffle(renumber.begin(), renumber.end(), generator);

    genGraph();
    genFlights();

    if (edges.size() > m)
    {
        std::cerr << "ERROR: Too many edges: " << edges.size() << " / " << m << std::endl;
    }
    if (edges.size() < m)
    {
        std::cerr << "Warning: Too few edges: " << edges.size() << " / " << m << std::endl;
    }

    if (flights.size() > t)
    {
        std::cerr << "ERROR: Too many flights: " << flights.size() << " / " << t << std::endl;
    }
    if (flights.size() < t)
    {
        std::cerr << "Warning: Too few flights: " << flights.size() << " / " << t << std::endl;
    }

    outfile << n << ' ' << edges.size() << ' ' << flights.size() << '\n';
    for (int i = 0; i < n; ++i)
    {
        outfile << edgesFrom[i].size();
        for (int other : edgesFrom[i])
        {
            if (other < 0 || other >= n || other == i)
            {
                std::cerr << "ERROR: Out of bounds edge: " << i << " " << other << " / " << n << std::endl;
            }
            outfile << ' ' << other;
        }
        outfile << '\n';
    }
    for (Edge& flight : flights)
    {
        if (flight.from < 0 || flight.from >= n || flight.to < 0 || flight.to >= n || flight.from == flight.to)
        {
            std::cerr << "ERROR: Out of bounds flight: " << flight.from << " " << flight.to << " / " << n << std::endl;
        }
        outfile << flight.from << ' ' << flight.to << '\n';
    }
}

int main()
{
    int num;
    while (true)
    {
        std::cout << "Test num? ";
        std::cin >> num;
        std::cout << "MAX_N MAX_T? ";
        std::cin >> n >> t;
        m = 2 * n;

        if (n < 10)
        {
            for (int i = 0; i < 5; ++i)
            {
                if (i % 3 == 0)
                {
                    gType = RANDOM;
                    fType = RANDOM_FLIGHTS;
                }
                else if (i % 3 == 1)
                {
                    gType = RANDOM;
                    fType = RANDOM_INC;
                }
                else if (i % 3 == 2)
                {
                    gType = RANDOM;
                    fType = PATH;
                }

                std::string nums = "";
                nums += (num / 10 + '0');
                nums += (num % 10 + '0');
                std::ofstream outfile("flights." + nums + ".in");
                genTest(outfile);
                outfile.close();

                ++num;
            }
        }
        else
        {
            for (int i = 0; i < 15; ++i)
            {
                if (i == 0)
                {
                    gType = RANDOM;
                    fType = RANDOM_FLIGHTS;
                }
                else if (i == 1)
                {
                    gType = RANDOM;
                    fType = RANDOM_INC;
                }
                else if (i == 2)
                {
                    gType = RANDOM;
                    fType = PATH;
                }
                else if (i == 3)
                {
                    gType = ALL_ZERO;
                    fType = ALL_FROM_ZERO;
                }
                else if (i == 4)
                {
                    gType = ALL_ZERO;
                    fType = ALL_TO_ZERO;
                }
                else if (i == 5)
                {
                    gType = ALL_ZERO;
                    fType = ALL_TO_FROM_ZERO;
                }
                else if (i == 6)
                {
                    gType = ALL_ZERO_ONE;
                    fType = ZERO_TO_ONE;
                }
                else if (i == 7)
                {
                    gType = ALL_ZERO_ONE;
                    fType = ZERO_TO_FROM_ONE;
                }
                else if (i == 8)
                {
                    gType = ALL_ZERO_ONE;
                    fType = ZERO_TO_FROM_ONE_RANDOM;
                }
                else if (i == 9)
                {
                    gType = ALL_ZERO_SOLO_ONE;
                    fType = ZERO_TO_ONE;
                }
                else if (i == 10)
                {
                    gType = ALL_ZERO_SOLO_ONE;
                    fType = ZERO_FROM_ONE;
                }
                else if (i == 11)
                {
                    gType = ALL_ZERO_SOLO_ONE;
                    fType = ZERO_TO_FROM_ONE;
                }
                else if (i == 12)
                {
                    gType = ALL_ZERO_ONE;
                    fType = FROM_ZERO_TO_ONE;
                }
                else if (i == 13)
                {
                    gType = CLUSTER;
                    fType = RANDOM_CLUSTER;
                }
                else if (i == 14)
                {
                    gType = CLUSTER;
                    fType = PATH_CLUSTER;
                }

                std::string nums = "";
                nums += (num / 10 + '0');
                nums += (num % 10 + '0');
                std::ofstream outfile("flights." + nums + ".in");
                genTest(outfile);
                outfile.close();

                ++num;
            }
        }
    }
    return 0;
}
