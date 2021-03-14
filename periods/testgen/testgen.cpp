#include <iostream>
#include <fstream>
#include <utility>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

using namespace std;

const int MAX_N = 1e5;

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

#define ALL_ONE 0
#define ALMOST_ONE 1
#define MOSTLY_ONE 2
#define HALF_ONE 3
#define HALF_ONE_P 4
#define HALF_ONE_N 5
#define RAND_HALF_ONE 6
#define RAND_HALF_ONE_SINGE 7
#define NO_PERIOD 8

#define HIGH_PERIOD 10
#define LOW_PERIOD 20
#define LOW_AND_HIGH 30
#define MANY_PERIODS 40
#define RAND_SINGLE 50

vector<pair<int, int>> types = {
    {ALL_ONE, 0},
    {ALL_ONE, 1},
    {ALMOST_ONE, 0},
    {ALMOST_ONE, 1},
    {ALMOST_ONE, 0},
    {ALMOST_ONE, 1},
    {MOSTLY_ONE, 0},
    {MOSTLY_ONE, 1},
    {MOSTLY_ONE, 0},
    {HALF_ONE, 0},
    {HALF_ONE, 1},
    {HALF_ONE_P, 0},
    {HALF_ONE_P, 1},
    {HALF_ONE_P, 0},
    {HALF_ONE_P, 1},
    {RAND_HALF_ONE, 0},
    {RAND_HALF_ONE, 1},
    {RAND_HALF_ONE_SINGE, 0},
    {RAND_HALF_ONE_SINGE, 1},
    {RAND_HALF_ONE_SINGE, 0},
    {RAND_HALF_ONE_SINGE, 1},
    {NO_PERIOD, 0},
    {NO_PERIOD, 1},
    {HIGH_PERIOD, 0},
    {HIGH_PERIOD, 1},
    {HIGH_PERIOD, 0},
    {HIGH_PERIOD, 1},
    {LOW_PERIOD, 0},
    {LOW_PERIOD, 1},
    {LOW_PERIOD, 0},
    {LOW_PERIOD, 1},
    {LOW_PERIOD, 0},
    {LOW_PERIOD, 1},
    {LOW_AND_HIGH, 0},
    {MANY_PERIODS, 1},
    {MANY_PERIODS, 0},
    {RAND_SINGLE, 0}
};

pair<int, int> typeP;

int n;

vector<int> genReps(int reps, const vector<int>& templ)
{
    vector<int> array;
    for (int i = 0; i < reps; ++i)
    {
        array.insert(array.end(), templ.begin(), templ.end());
    }
    return array;
}

vector<int> seedPeriod(int period, int& curr, bool same)
{
    vector<int> array;
    if (same)
    {
        array = genReps(period - 1, {curr++});
        array.push_back(curr++);
    }
    else
    {
        array.resize(period);
        iota(array.begin(), array.end(), curr);
        curr += array.size();
    }
    return array;
}

void fillToN(vector<int>& array, int& curr)
{
    if (array.size() >= n) return;
    int currSize = array.size();
    array.resize(n);
    iota(array.begin() + currSize, array.end(), curr);
    curr += n - currSize;
}

void genTest(ofstream& outfile)
{
    int type = typeP.first;
    n -= typeP.second;

    cerr << "gen " << type << " " << n << endl;

    int curr = 0;
    vector<int> array;

    if (type <= NO_PERIOD)
    {
        array.push_back(curr++);
        int garbage;
        if (type == ALL_ONE) garbage = randNum(2, 7);
        else if (type == MOSTLY_ONE) garbage = randNum(n / 20, n / 5);
        else if (type == HALF_ONE) garbage = n - ((n + 1) / 2);
        else if (type == HALF_ONE_N) garbage = n - ((n + 1) / 2 + 1);
        else if (type == HALF_ONE_P) garbage = n - ((n + 1) / 2 - randNum(1, 2));
        else if (type == RAND_HALF_ONE || type == RAND_HALF_ONE_SINGE) garbage = n - randNum(n / 4, n / 2 - 10);
        else if (type == NO_PERIOD) garbage = n;
        array = genReps(n - garbage, array);
        if (type == RAND_HALF_ONE_SINGE)
        {
            array.push_back(curr++);
            array = genReps(2, array);
        }
        fillToN(array, curr);
    }
    else if (type <= LOW_PERIOD)
    {
        int period = randNum(2, 7);
        if (type == HIGH_PERIOD) period = n / period;
        array = seedPeriod(period, curr, false);
        array = genReps(n / period, array);
        fillToN(array, curr);
    }
    else if (type == LOW_AND_HIGH)
    {
        int period = randNum(2, 7);
        int secPeriod = n / randNum(2, 7);
        secPeriod = 1 + (secPeriod / period) * period;
        array = seedPeriod(period, curr, false);
        array = genReps((secPeriod - 1) / period, array);
        array.push_back(curr++);
        array = genReps(n / secPeriod, array);
        fillToN(array, curr);
    }
    else if (type == MANY_PERIODS)
    {
        while (array.size() < n)
        {
            array.push_back(curr++);
            array = genReps(randNum(1, 5), array);
        }
    }
    else if (type == RAND_SINGLE)
    {
        int period = randNum(n / 20, n / 2 - n / 20);
        array = seedPeriod(period, curr, false);
        std::cerr << " " << array.size() << std::endl;
        array = genReps(2, array);
        fillToN(array, curr);
    }

    outfile << n << "\n";
    for (int i = 0 ; i < n; ++i)
    {
        if (i > 0) outfile << " ";
        outfile << array[i];
    }
    outfile << "\n";

    n += typeP.second;
}

int main()
{
    generator.seed(time(nullptr));
    ios::sync_with_stdio(false);

    int num;
    while (true)
    {
        cout << "Test num? ";
        cin >> num;
        if (num < 0) return 0;
        cout << "N? ";
        cin >> n;

        shuffle(types.begin(), types.end(), generator);
        
        for (int i = 0; i < types.size(); ++i)
        {
            typeP = types[i];

            string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');
            ofstream outfile("periods." + nums + ".in");
            std::cerr << num << std::endl;
            genTest(outfile);
            outfile.close();

            ++num;
        }
    }
    return 0;
}
