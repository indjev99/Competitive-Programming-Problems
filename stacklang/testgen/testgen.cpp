#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
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

struct Edge
{
    int from;
    int to;
    int len;
};

#define ONLY_ZERO 0
#define ONLY_ONE 1
#define ZERO_ONE 2

int n;

int type;
bool tree;
bool connected;
bool sameEnd;
bool path;
bool disconnect;

vector<Edge> edges;

void addEdge(int from, int to)
{
    int len;
    if (type == ONLY_ZERO) len = 0;
    else if (type == ONLY_ONE) len = 1;
    else len = randNum(0, 1);
    edges.push_back({from, to, len});
}

void genTest(ofstream& outfile)
{
    edges.clear();

    int start = randNum(0, n - 1);
    int end = randNum(0, n - 1);

    if (path)
    {
        vector<int> perm;
        perm.resize(n);
        iota(perm.begin(), perm.end(), 0);
        shuffle(perm.begin(), perm.end(), generator);
        start = perm[0];
        end = perm[n - 1];
        
        for (int i = 1; i < n; ++i)
        {
            if (disconnect && i == n - 1) continue;
            addEdge(perm[i - 1], perm[i]);
            if (!tree) addEdge(perm[i], perm[i - 1]);
        }
    }

    if ((tree || connected) && !path)
    {
        vector<int> perm;
        perm.resize(n);
        iota(perm.begin(), perm.end(), 0);
        shuffle(perm.begin(), perm.end(), generator);
        start = perm[0];
        while (end == start) end = randNum(0, n - 1);
        for (int i = 1; i < n; ++i)
        {
            if (!connected && oneIn(13)) continue;
            if (disconnect && perm[i] == end) continue;
            int prev = perm[randNum(0, i - 1)];
            addEdge(prev, perm[i]);
            if (!tree) addEdge(perm[i], prev);
        }
    }

    if (!tree && !path)
    {
        while (edges.size() < 2.5 * n)
        {
            int from = randNum(0, n - 1);
            int to = randNum(0, n - 1);
            if (disconnect && (from == end || to == end)) continue;
            addEdge(from, to);
        }
    }

    if (sameEnd && !path) end = start;

    outfile << n << " " << edges.size() << "\n";
    outfile << start << " " << end << "\n";
    for (const Edge& edge : edges)
    {
        outfile << edge.from << " " << edge.to << " " << edge.len << "\n";
    }
}

void metaGenTest(ofstream& outfile)
{
    for (int i = 0; i < 4; ++i)
    {
        path = oneIn(3);
        connected = oneIn(3);
        sameEnd = oneIn(10);
        disconnect = oneIn(5);
        genTest(outfile);
    }
}

int main()
{
    ios::sync_with_stdio(false);

    int num;
    while (true)
    {
        cout << "Test num? ";
        cin >> num;
        if (num < 0) return 0;
        cout << "n type tree? ";
        cin >> n >> type >> tree;
        for (int i = 0; i < 10; ++i)
        {
            string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');
            ofstream outfile("stacklang." + nums + ".in");
            metaGenTest(outfile);
            outfile.close();

            ++num;
        }
    }
    return 0;
}
