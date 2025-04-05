#include "cactus.h"

#include <iostream>
#include <algorithm>

static void runTest()
{
    int n, m;
    std::vector<std::pair<int, int>> edges;

    std::cin >> n >> m;

    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        edges.push_back({u, v});
    }

    std::vector<bool> data = encode(n, edges);

    std::vector<std::pair<int, int>> edgesGuess = decode(n, data);

    for (auto& [u, v] : edges)
    {
        if (u > v) std::swap(u, v);
    }
    std::sort(edges.begin(), edges.end());

    for (auto& [u, v] : edgesGuess)
    {
        if (u > v) std::swap(u, v);
    }
    std::sort(edgesGuess.begin(), edgesGuess.end());

    if (edgesGuess == edges)
    {
        std::cout << data.size() << std::endl;
    }
    else
    {
        std::cerr << "Wrong answer" << std::endl;
    }
}

int main()
{
    int t;
    std::cin >> t;

    for (int i = 0; i < t; i++)
    {
        runTest();
    }

    return 0;
}
