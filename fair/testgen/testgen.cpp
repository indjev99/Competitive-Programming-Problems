#include <iostream>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <queue>
#include <set>
#include <random>
#include <cassert>

std::mt19937 generator;

long long randInt(long long lb, long long ub)
{
    assert(ub > lb);
    std::uniform_int_distribution<long long> distribution(lb, ub - 1);
    return distribution(generator);
}

long long randInt(long long ub)
{
    return randInt(0, ub);
}

double randReal(double lb = 0, double ub = 1)
{
    std::uniform_real_distribution<double> distribution(lb, ub);
    return distribution(generator);
}

bool oneIn(long long chances)
{
    if (chances == 0) return false;
    return randInt(chances) == 0;
}

const int NUM_DICE = 100000;

struct TestParams
{
    int n;
    int mnRatio;
    double p;
    double penFair;
};

struct Die
{
    std::vector<double> cumuls;
};

struct RollResults
{
    std::vector<int> counts;
};

Die fairDie(int n)
{
    Die die;
    die.cumuls.resize(n);
    for (int i = 0; i < n; ++i)
    {
        die.cumuls[i] = static_cast<double>(i + 1) / n;
    }
    return die;
}

Die unfairDie(int n)
{
    // Die die;
    // die.cumuls.resize(n);
    // for (int i = 0; i < n - 1; ++i)
    // {
    //     die.cumuls[i] = randReal();
    // }
    // die.cumuls[n - 1] = 1;
    // std::sort(die.cumuls.begin(), die.cumuls.end());
    // return die;

    Die die;
    die.cumuls.resize(n);
    double prev = 0;
    for (int i = 0; i < n; ++i)
    {
        die.cumuls[i] = prev + randReal();
        prev = die.cumuls[i];
    }
    for (int i = 0; i < n; ++i)
    {
        die.cumuls[i] /= prev;
    }
    return die;
}

RollResults rollDie(const Die& die, int rolls)
{
    int n = die.cumuls.size();

    RollResults results;
    results.counts.resize(n, 0);

    for (int i = 0; i < rolls; ++i)
    {
        bool found = false;
        double rollVar = randReal();
        for (int j = 0; j < n; ++j)
        {
            if (rollVar < die.cumuls[j])
            {
                ++results.counts[j];
                found = true;
                break;
            }
        }

        if (!found) --i;
    }

    return results;
}

void generate(TestParams test, std::ofstream& infile, std::ofstream& solfile)
{
    int n = test.n;
    int m = round(sqrt(1) * test.mnRatio);
    double p = test.p;

    double penFair = test.penFair;
    double penUnfair = 1 - penFair;

    infile << n << " " << m << " " << p << " " << penFair << " " << penUnfair << "\n";
    infile << NUM_DICE << "\n";

    for (int i = 0; i < NUM_DICE; ++i)
    {
        double fairVar = randReal();
        bool fair = fairVar < p;

        solfile << fair << "\n";

        Die die = fair ? fairDie(n) : unfairDie(n);

        RollResults results = rollDie(die, m);

        for (int j = 0; j < n; ++j)
        {
            if (j > 0) infile << " ";
            infile << results.counts[j];
        }

        infile << "\n";
    }
}

std::vector<TestParams> tests = {
    {2, 37, 0.65, 0.50}, // 1
    {2, 23, 0.51, 0.61}, // 2
    {2, 15, 0.79, 0.40}, // 3
    {2, 21, 0.70, 0.55}, // 4
    {3, 31, 0.71, 0.34}, // 5
    {3, 22, 0.77, 0.66}, // 6
    {3, 18, 0.50, 0.49}, // 7
    {3, 25, 0.63, 0.37}, // 8
    {4, 38, 0.72, 0.46}, // 9
    {4, 40, 0.60, 0.59}, // 10
    {4, 23, 0.50, 0.35}, // 11
    {4, 15, 0.75, 0.56}, // 12
    {5, 20, 0.53, 0.39}, // 13
    {5, 35, 0.66, 0.65}, // 14
    {6, 28, 0.64, 0.35}, // 15
    {6, 17, 0.77, 0.60}, // 16
    {7, 16, 0.55, 0.61}, // 17
    {7, 25, 0.65, 0.53}, // 18
    {8, 20, 0.60, 0.46}, // 19
    {8, 40, 0.80, 0.40}, // 20
};

int main()
{
    generator.seed(42);

    int num = 1;
    for (TestParams test : tests)
    {
        std::string nums = "";
        nums += num / 10 + '0';
        nums += num % 10 + '0';

        std::string inName = "fair." + nums + ".in";
        std::string solName = "fair." + nums + ".sol";

        std::cerr << "Test " << nums << std::endl;

        std::ofstream infile(inName);
        std::ofstream solfile(solName);

        generate(test, infile, solfile);

        ++num;
    }

    return 0;
}
