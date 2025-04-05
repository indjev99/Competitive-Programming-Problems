#include "cactus.h"

#include <iostream>
#include <algorithm>

static const std::string PASSWORD = "password";

static const int BITS_PER_SYMB = 6;

static void encodeGrader()
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

    std::cout << PASSWORD << "\n";

    int bits = data.size();
    std::cout << bits << "\n";
    for (int i = 0; i < (bits + BITS_PER_SYMB - 1) / BITS_PER_SYMB; i++)
    {
        int code = 0;
        for (int j = 0; j < BITS_PER_SYMB; j++)
        {
            int idx = i * BITS_PER_SYMB + j;
            bool b = idx < bits ? data[idx] : 0;
            code |= b << j;
        }

        char c = '0' + code;
        std::cout << c;
    }
    std::cout << std::endl;
}

static void decodeGrader()
{
    int n;
    std::cin >> n;

    int bits;
    std::cin >> bits;

    std::vector<bool> data(bits);

    for (int i = 0; i < (bits + BITS_PER_SYMB - 1) / BITS_PER_SYMB; i++)
    {
        char c;
        std::cin >> c;
        int code = c - '0';

        for (int j = 0; j < BITS_PER_SYMB; j++)
        {
            bool b = (code >> j) & 1;
            int idx = i * BITS_PER_SYMB + j;
            if (idx < bits) data[idx] = b;
        }
    }

    std::vector<std::pair<int, int>> edgesGuess = decode(n, data);

    std::cout << PASSWORD << "\n";

    std::cout << edgesGuess.size() << "\n";
    for (int i = 0; i < (int) edgesGuess.size(); ++i)
    {
        std::cout << edgesGuess[i].first << " " << edgesGuess[i].second << "\n";
    }
    std::cout << std::flush;
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
