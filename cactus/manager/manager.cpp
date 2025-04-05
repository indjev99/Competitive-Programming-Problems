#include "manager.h"

#include <iostream>
#include <algorithm>
#include <vector>

const std::string PASSWORD = "password";

const int BITS_PER_SYMB = 6;

const int PROCESSES = 2;

char** fds;

void finish()
{
    for (int i = 0; i < 2 * PROCESSES; i++)
    {
        close(atoi(fds[i]));
    }

    exit(0);
}

void myAssert(bool cond, std::string msg)
{
    if (cond) return;

    std::cout << -1 << std::endl;
    std::cout << msg << std::endl;

    finish();
}

int maxBits = 0;

void runTest()
{
    int n, m, mGuess;
    std::vector<std::pair<int, int>> edges;
    std::vector<std::pair<int, int>> edgesGuess;

    std::string pass;

    std::cin >> n >> m;
    out(0) << n << " " << m << "\n";
    out(1) << n << "\n";

    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        edges.push_back({u, v});
        out(0) << u << " " << v << "\n";
    }
    out(0) << std::flush;

    in(0) >> pass;
    myAssert(pass == PASSWORD, "Your program wrote to stdout.");

    int bits;
    in(0) >> bits;

    std::vector<bool> data(bits);

    for (int i = 0; i < (bits + BITS_PER_SYMB - 1) / BITS_PER_SYMB; i++)
    {
        char c;
        in(0) >> c;
        int code = c - '0';

        for (int j = 0; j < BITS_PER_SYMB; j++)
        {
            bool b = (code >> j) & 1;
            int idx = i * BITS_PER_SYMB + j;
            if (idx < bits) data[idx] = b;
        }
    }

    out(1) << bits << "\n";
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
        out(1) << c;
    }
    out(1) << std::endl;

    in(1) >> pass;
    myAssert(pass == PASSWORD, "Your program wrote to stdout.");

    in(1) >> mGuess;
    for (int i = 0; i < mGuess; i++)
    {
        int u, v;
        in(1) >> u >> v;
        edgesGuess.push_back({u, v});
    }

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

    myAssert(edgesGuess == edges, "Incorrect edges");

    maxBits = std::max(bits, maxBits);
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    fds = argv + 1;

    fd_to_stream(fds, 2 * PROCESSES);

    out(0) << 0 << std::endl;
    out(1) << 1 << std::endl;

    int t;
    std::cin >> t;

    out(0) << t << std::endl;
    out(1) << t << std::endl;

    for (int i = 0; i < t; i++)
    {
        runTest();
    }

    std::cout << maxBits << std::endl;

    finish();

    return 0;
}
