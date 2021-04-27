#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

using namespace std;

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

int n;
bool onlyOdd;

int m;
int numYes;
bool shuf;
bool rev;

vector<bool> votes;

void genTest(ofstream& outfile)
{
    votes.clear();
    votes.resize(m);

    if (numYes >= 0)
    {
        fill(votes.begin(), votes.begin() + numYes, true);
        fill(votes.begin() + numYes, votes.end(), false);
    }
    else
    {
        for (int i = 0; i < m; i += 2)
        {
            bool first = true;
            bool second = false;
            if (oneIn(2)) swap(first, second);
            votes[i] = first;
            if (i + 1 < m) votes[i + 1] = second;
        }
    }

    if (shuf) shuffle(votes.begin(), votes.end(), generator);
    if (rev) reverse(votes.begin(), votes.end());

    outfile << m << "\n";
    for (int i = 0; i < m; ++i)
    {
        if (i > 0) outfile << " ";
        if (votes[i]) outfile << "Y";
        else outfile << "N";
    }
    outfile << "\n";
}

#define M_FULL false
#define M_SUB true

#define EVEN false
#define ODD true

#define NYT_RAND 0
#define NYT_NONE 1
#define NYT_ALL 2
#define NYT_HALF 3
#define NYT_HALF_SUB 4
#define NYT_HALF_ADD 5
#define NYT_ALT 6

#define OT_RAND 0
#define OT_DIR 1
#define OT_REV 2

struct Config
{
    bool subtr;
    bool odd;
    int numYesType;
    int orderType;
};

void genConfigTest(Config config, ofstream& outfile)
{
    m = n;
    if (n <= 10) m = randNum(1, n);
    else if (config.subtr) m -= randNum(1, 5) * 2;
    m = (m + 1) / 2 * 2;
    if (config.odd || onlyOdd) --m;

    if (config.numYesType == NYT_RAND) numYes = randNum(0, m);
    else if (config.numYesType == NYT_NONE) numYes = 0;
    else if (config.numYesType == NYT_ALL) numYes = m;
    else if (config.numYesType == NYT_HALF) numYes = m / 2;
    else if (config.numYesType == NYT_HALF_SUB) numYes = m / 2 - 1;
    else if (config.numYesType == NYT_HALF_ADD) numYes = m / 2 + 1;
    else if (config.numYesType == NYT_ALT) numYes = -1;

    shuf = false;
    rev = false;
    if (config.orderType == OT_RAND) shuf = true;
    else if (config.orderType == OT_REV) rev = true;
    genTest(outfile);
}

vector<Config> configs = {
    {M_FULL, ODD, NYT_NONE, OT_DIR},
    {M_FULL, ODD, NYT_ALL, OT_DIR},
    {M_FULL, ODD, NYT_HALF, OT_DIR},
    {M_FULL, ODD, NYT_HALF_ADD, OT_DIR},
    {M_FULL, ODD, NYT_HALF, OT_REV},
    {M_FULL, ODD, NYT_HALF_ADD, OT_REV},
    {M_FULL, ODD, NYT_HALF, OT_RAND},
    {M_FULL, ODD, NYT_HALF_ADD, OT_RAND},
    {M_FULL, ODD, NYT_HALF, OT_RAND},
    {M_FULL, ODD, NYT_HALF_ADD, OT_RAND},
    {M_FULL, ODD, NYT_HALF, OT_RAND},
    {M_FULL, ODD, NYT_HALF_ADD, OT_RAND},
    {M_FULL, ODD, NYT_RAND, OT_DIR},
    {M_FULL, ODD, NYT_RAND, OT_REV},
    {M_FULL, ODD, NYT_RAND, OT_RAND},
    {M_FULL, ODD, NYT_RAND, OT_RAND},
    {M_FULL, ODD, NYT_ALT, OT_DIR},
    {M_FULL, ODD, NYT_ALT, OT_REV},
    {M_FULL, ODD, NYT_ALT, OT_DIR},
    {M_FULL, ODD, NYT_ALT, OT_REV},
    {M_FULL, EVEN, NYT_NONE, OT_DIR},
    {M_FULL, EVEN, NYT_ALL, OT_DIR},
    {M_FULL, EVEN, NYT_HALF_SUB, OT_DIR},
    {M_FULL, EVEN, NYT_HALF, OT_DIR},
    {M_FULL, EVEN, NYT_HALF_ADD, OT_DIR},
    {M_FULL, EVEN, NYT_HALF_SUB, OT_REV},
    {M_FULL, EVEN, NYT_HALF, OT_REV},
    {M_FULL, EVEN, NYT_HALF_ADD, OT_REV},
    {M_FULL, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_FULL, EVEN, NYT_HALF, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_FULL, EVEN, NYT_HALF, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_FULL, EVEN, NYT_HALF, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_FULL, EVEN, NYT_HALF, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_FULL, EVEN, NYT_HALF, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_FULL, EVEN, NYT_HALF, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_FULL, EVEN, NYT_HALF, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_FULL, EVEN, NYT_HALF, OT_RAND},
    {M_FULL, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_FULL, EVEN, NYT_HALF, OT_RAND},
    {M_FULL, EVEN, NYT_RAND, OT_DIR},
    {M_FULL, EVEN, NYT_RAND, OT_DIR},
    {M_FULL, EVEN, NYT_RAND, OT_REV},
    {M_FULL, EVEN, NYT_RAND, OT_REV},
    {M_FULL, EVEN, NYT_RAND, OT_RAND},
    {M_FULL, EVEN, NYT_RAND, OT_RAND},
    {M_FULL, EVEN, NYT_RAND, OT_RAND},
    {M_FULL, EVEN, NYT_RAND, OT_RAND},
    {M_FULL, EVEN, NYT_RAND, OT_RAND},
    {M_FULL, EVEN, NYT_ALT, OT_DIR},
    {M_FULL, EVEN, NYT_ALT, OT_REV},
    {M_SUB, ODD, NYT_HALF, OT_DIR},
    {M_SUB, ODD, NYT_HALF_ADD, OT_DIR},
    {M_SUB, ODD, NYT_HALF, OT_REV},
    {M_SUB, ODD, NYT_HALF_ADD, OT_REV},
    {M_SUB, ODD, NYT_HALF, OT_RAND},
    {M_SUB, ODD, NYT_HALF_ADD, OT_RAND},
    {M_SUB, ODD, NYT_HALF, OT_RAND},
    {M_SUB, ODD, NYT_HALF_ADD, OT_RAND},
    {M_SUB, ODD, NYT_RAND, OT_DIR},
    {M_SUB, ODD, NYT_RAND, OT_REV},
    {M_SUB, ODD, NYT_RAND, OT_RAND},
    {M_SUB, ODD, NYT_ALT, OT_DIR},
    {M_SUB, ODD, NYT_ALT, OT_REV},
    {M_SUB, EVEN, NYT_NONE, OT_DIR},
    {M_SUB, EVEN, NYT_ALL, OT_DIR},
    {M_SUB, EVEN, NYT_HALF_SUB, OT_DIR},
    {M_SUB, EVEN, NYT_HALF, OT_DIR},
    {M_SUB, EVEN, NYT_HALF_ADD, OT_DIR},
    {M_SUB, EVEN, NYT_HALF_SUB, OT_REV},
    {M_SUB, EVEN, NYT_HALF, OT_REV},
    {M_SUB, EVEN, NYT_HALF_ADD, OT_REV},
    {M_SUB, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_SUB, EVEN, NYT_HALF, OT_RAND},
    {M_SUB, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_SUB, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_SUB, EVEN, NYT_HALF, OT_RAND},
    {M_SUB, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_SUB, EVEN, NYT_HALF_SUB, OT_RAND},
    {M_SUB, EVEN, NYT_HALF, OT_RAND},
    {M_SUB, EVEN, NYT_HALF_ADD, OT_RAND},
    {M_SUB, EVEN, NYT_RAND, OT_DIR},
    {M_SUB, EVEN, NYT_RAND, OT_REV},
    {M_SUB, EVEN, NYT_RAND, OT_RAND},
    {M_SUB, EVEN, NYT_RAND, OT_RAND},
    {M_SUB, EVEN, NYT_ALT, OT_DIR},
    {M_SUB, EVEN, NYT_ALT, OT_REV}
};

int currConfig;
void metaGenTest(ofstream& outfile)
{
    for (int i = 0; i < 10; ++i)
    {
        genConfigTest(configs[currConfig++], outfile);
    }
}

int main()
{
    generator.seed(time(nullptr));
    std::cerr << configs.size() << std::endl;

    int num;
    int targetType;
    while (true)
    {
        cout << "Test num? ";
        cin >> num;
        if (num < 0) return 0;
        cout << "n onlyOdd targetType? ";
        cin >> n >> onlyOdd >> targetType;
        n = n / 2 * 2;
        currConfig = 0;
        shuffle(configs.begin(), configs.end(), generator);
        for (int i = 0; i < 10; ++i)
        {
            string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');
            ofstream outfile("robots." + nums + ".in");
            outfile << targetType << std::endl;
            metaGenTest(outfile);
            outfile.close();
            ++num;
        }
    }
    return 0;
}
