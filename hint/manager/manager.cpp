#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <fstream>
#include <vector>

#define HINT 0
#define SOLVE 1

static int n, m, k, k2, bits;
static std::vector<int> a, b, sol, maybeSol;
static std::vector<bool> hint;

int main()
{
    signal(SIGPIPE, SIG_IGN);

    std::ofstream out1("pipe_in1");
    std::ofstream out2("pipe_in2");

    std::ifstream in1("pipe_out1");
    std::ifstream in2("pipe_out2");

    out1 << HINT << "\n";
    out2 << SOLVE << "\n";

    std::cin >> n >> m;

    out1 << n << " " << m << "\n";
    out2 << n << " " << m << "\n";

    a.resize(n);
    b.resize(m);

    for (int i = 0; i < n; ++i)
    {
        std::cin >> a[i];

        if (i > 0)
        {
            out1 << " ";
            out2 << " ";
        }
        out1 << a[i];
        out2 << a[i];
    }

    out1 << "\n";
    out2 << "\n";

    for (int i = 0; i < m; ++i)
    {
        std::cin >> b[i];

        if (i > 0)
        {
            out1 << " ";
            out2 << " ";
        }
        out1 << b[i];
        out2 << b[i];
    }

    out1 << "\n";
    out2 << "\n";

    std::cin >> k;

    out1 << k << "\n";

    sol.resize(k);

    for (int i = 0; i < k; ++i)
    {
        std::cin >> sol[i];

        if (i > 0) out1 << " ";
        out1 << sol[i];
    }

    out1 << std::endl;

    in1 >> bits;

    std::cout << bits << "\n";

    out2 << bits << "\n";

    hint.resize(bits);

    for (int i = 0; i < bits; ++i)
    {
        int bit;
        in1 >> bit;
        hint[i] = bit;

        if (i > 0) out2 << " ";
        out2 << hint[i];
    }

    out2 << std::endl;

    in2 >> k2;

    std::cout << k2 << "\n";

    maybeSol.resize(k2);

    for (int i = 0; i < k2; ++i)
    {
        in2 >> maybeSol[i];

        if (i > 0) std::cout << " ";
        std::cout << maybeSol[i];
    }

    std::cout << std::endl;

    return 0;
}
