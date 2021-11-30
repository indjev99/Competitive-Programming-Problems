#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

const int TARGET = 640;

int n, m, k, k2, bits;
static std::vector<int> a, b, maybeSol;

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]);
    std::ifstream out(argv[3]);

    if (!in)
    {
        std::cout << "0\n";
        std::cerr << "In-file " << argv[1] << " not found.\n";
        return 0;
    }
    if (!out)
    {
        std::cout << "0\n";
        std::cerr << "Out-file " << argv[3] << " not found.\n";
        return 0;
    }

    in >> n >> m;

    a.resize(n);
    b.resize(m);

    for (int& num : a)
    {
        in >> num;
    }

    for (int& num : b)
    {
        in >> num;
    }

    in >> k;

    out >> bits >> k2;

    maybeSol.resize(k2);

    for (int& num : maybeSol)
    {
        out >> num;
    }

    if (k2 < k)
    {
        std::cout << "0\n";
        std::cerr << "Wrong answer.\n";
        return 0;
    }

    int i = 0, j = 0;
    for (int num : maybeSol)
    {
        while (i < n && a[i] != num) ++i;
        while (j < m && b[j] != num) ++j;

        if (i == n || j == m)
        {
            std::cout << "0\n";
            std::cerr << "Wrong answer.\n";
            return 0;
        }

        ++i;
        ++j;
    }

    std::cout << std::min(pow((double) TARGET / (bits + 1), 0.3), 1.0) << "\n";

    return 0;
}
