#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <math.h>
#include <random>
using namespace std;

mt19937 mt(1337);
const int TESTS = 100;
vector< pair<int, int> > ratios;

int mtlr(int L, int R)
{
    return mt() % (R - L + 1) + L;
}

bool sr(pair<int, int> a, pair<int, int> b)
{
    return (double)a.first / (double)a.second < (double)b.first / (double)b.second;
}

int gcd(int a, int b)
{
    int r;

    while(b != 0)
    {
        r = a % b;
        a = b;
        b = r;
    }

    return a;
}

string to3num(int test)
{
    string s = "";

    while(test > 0)
    {
        s.push_back(test % 10 + '0');
        test /= 10;
    }

    while(s.length() < 3)
        s.push_back('0');

    reverse(s.begin(), s.end());

    return s;
}

string inputName(int test)
{
    return "playlists." + to3num(test) + ".in";
}

string outputName(int test)
{
    return "playlists." + to3num(test) + ".sol";
}

int main()
{
    int test;
    int i, j;

    for (i=1;i<=100;i++)
    {
        for (j=i;j<=100;j++)
        {
            if (gcd(i, j) == 1 && (double)i / (double)j > 0.02)
                ratios.push_back(make_pair(i, j));
        }
    }

    sort(ratios.begin(), ratios.end(), sr);

    int step = (int)ratios.size() / TESTS;
    for (test=1;test<=TESTS;test++)
    {
        printf("Test #%d\n", test);

        int curId = (test - 1) * step;
        if (test == TESTS)
            curId = (int)ratios.size() - 1;

        double rat = (double)ratios[curId].first / (double)ratios[curId].second;

        int maxS = floor(100.0 * rat);

        int s = mtlr(2, maxS);
        int n = min(100, (int)round((double)s / rat));
        int k = mtlr(3, 100);
        double p = (double)mtlr(20000, 95000) / 100000.0;
        int seed = mtlr(1, 1000000000);

        FILE *f = fopen(inputName(test).c_str(), "w");
        fprintf(f, "%d %d %d %.5f %d\n", n, k, s, p, seed);
        printf("%d %d %d %.5f %d\n", n, k, s, p, seed);
        fclose(f);

        f = fopen(outputName(test).c_str(), "w");
        fclose(f);
    }
}
