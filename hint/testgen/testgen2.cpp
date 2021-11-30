#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <time.h>

std::mt19937 generator;
int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(generator);
}

bool oneIn(int chances)
{
    return randNum(0, chances) == 0;
}

int n, m, t, s;

std::vector<int> a, b, c1, c2;

void genTest(std::ostream& out)
{
    a.clear();
    b.clear();

    a.resize(n, 0);
    b.resize(m, 1);

    a[1] = a[2] = a[3] = 2;

    b[0] = b[m - 3] = b[m - 2] = 2;

    a[0] = 1;

    b[m - 1] = 0;

    if (t % 2 == 1)
    {
        std::swap(n, m);
        std::swap(a, b);
    }

    out << n << " " << m << "\n";

    for (int i = 0; i < n; ++i)
    {
        if (i > 0) out << " ";
        out << a[i];
    }
    out << "\n";

    for (int i = 0; i < m; ++i)
    {
        if (i > 0) out << " ";
        out << b[i];
    }
    out << "\n";
}

int main()
{
    std::ios::sync_with_stdio(false);

    generator.seed(time(nullptr));

    int num;
    while (true)
    {
        std::cout << "Test num? ";
        std::cin >> num;
        if (num < 0) return 0;
        std::cout << "m n t? ";
        std::cin >> n >> m >> t;
        for (int i = 0; i < 1; ++i)
        {
            std::string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');

            std::cerr << "hint." << nums << ".in" << std::endl;

            std::ofstream outfile("hint." + nums + ".in");
            genTest(outfile);
            outfile.close();

            ++num;
        }
    }

    return 0;
}
