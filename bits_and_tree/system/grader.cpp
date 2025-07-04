#include "bits_and_tree.h"

#include <iostream>
#include <algorithm>

static const std::string PASSWORD = "bfb2kjrb30ybk234m2b3";
static const std::string PASSWORD2 = "inv";

static void check_alive() {
    std::string pass;
    std::cin >> pass;

    if (pass != PASSWORD2) {
        exit(0);
    }
}

static void encodeGrader()
{
    check_alive();

    int n;
    std::cin >> n;
    
    std::string bits;
    std::cin >> bits;

    std::vector<bool> data(bits.size());
    for (int i = 0; i < bits.size(); ++i) {
        data[i] = bits[i] - '0';
    }

    std::vector<std::pair<int, int>> edges = encode(n, data);

    std::cout << PASSWORD << "\n";

    std::cout << edges.size() << "\n";
    for (auto [u, v]: edges) {
        std::cout << u << " " << v << "\n";
    }
    std::cout << std::flush;
}

static void decodeGrader()
{
    check_alive();

    int n;
    std::cin >> n;

    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < n; ++i) {
        int u, v;
        std::cin >> u >> v;

        edges.emplace_back(u, v);
    }

    std::vector<bool> data = decode(n, edges);

    std::cout << PASSWORD << "\n";

    std::cout << data.size() << std::endl;
    for (bool bit: data) {
        std::cout << bit;
    }
    std::cout << std::endl;
}

int main()
{
    bool decoder;
    std::cin >> decoder;

    int t;
    std::cin >> t;

    for (int i = 0; i < t; i++)
    {
        if (!decoder) encodeGrader();
        else decodeGrader();
    }

    return 0;
}
