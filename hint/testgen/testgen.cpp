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

struct Distr
{
    std::vector<int> elems;

    Distr()
    {
        for (int i = 0; i < t; ++i)
        {
            elems.push_back(i);
        }

        for (int i = 0; i < t * 2; ++i)
        {
            elems.push_back(randNum(0, t));
        }
    }

    int sample()
    {
        return elems[randNum(0, elems.size())];
    }
};

std::vector<int> a, b, c1, c2;

void genTest(std::ostream& out)
{
    Distr aDistr1, aDistr2, bDistr1, bDistr2, cDistr, errDistr;

    a.clear();
    b.clear();

    for (int i = 0; i < s; ++i)
    {
        c1.push_back(cDistr.sample());
        c2.push_back(c1.back());

        if (oneIn(10))
        {
            if (oneIn(4)) c1.pop_back();
            else if (oneIn(3)) c2.pop_back();
            else if (oneIn(2)) c1.back() = errDistr.sample();
            else c2.back() = errDistr.sample();
        }
    }

    int aSwitch = randNum(0, n - c1.size());
    int bSwitch = randNum(0, m - c2.size());

    for (int i = 0; i < n - (int) c1.size(); ++i)
    {
        if (i < aSwitch) a.push_back(aDistr1.sample());
        else a.push_back(aDistr2.sample());
    }

    for (int i = 0; i < m - (int) c2.size(); ++i)
    {
        if (i < bSwitch) b.push_back(bDistr1.sample());
        else b.push_back(bDistr2.sample());
    }

    int aIns = randNum(0, n + 1 - c1.size());
    int bIns = randNum(0, m + 1 - c2.size());

    a.insert(a.begin() + aIns, c1.begin(), c1.end());
    b.insert(b.begin() + bIns, c2.begin(), c2.end());

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
        std::cout << "m n t s? ";
        std::cin >> n >> m >> t >> s;
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
