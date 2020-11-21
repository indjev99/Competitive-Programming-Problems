#include <iostream>
#include <fstream>
#include <random>
#include <numeric>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <assert.h>
#include <math.h>
using namespace std;

mt19937 generator;

int randNum(int lb, int ub)
{
    uniform_int_distribution<int> distribution(lb, ub);
    return distribution(generator);
}

struct Person
{
    int num;
    int target;
    int reward;
};

bool operator<(const Person& a, const Person& b)
{
    return a.num < b.num;
}

const int MAX_REWARD = 20000;
int maxN, maxLowerCnt;

int n, k;
vector<Person> people;

void verifyTest()
{
    assert(n > 1);
    assert(n <= maxN);
    assert(k >= 1);
    assert(k <= n / 2);
    assert(people.size() == n);

    unordered_set<int> taken;
    for (int i = 0; i < n; ++i)
    {
        const Person& curr = people[i];
        assert(i == curr.num);
        assert(curr.num != curr.target);
        assert(curr.target >= 0);
        assert(curr.target < n);
        assert(curr.reward >= 0);
        assert(curr.reward <= MAX_REWARD);
        assert(taken.find(curr.target) == taken.end());
        taken.insert(curr.target); 
    }
}

void printTest(ofstream& infile)
{
    infile << n << " " << k << "\n";
    for (const Person& person : people)
    {
        infile << person.target << " " << person.reward << "\n";
    }
}

void relabel()
{
    vector<int> labels(n);
    iota(labels.begin(), labels.end(), 0);
    shuffle(labels.begin(), labels.end(), generator);
    for (Person& person : people)
    {
        person.num = labels[person.num];
        person.target = labels[person.target];
    }
    sort(people.begin(), people.end());
}

#define G_ONE_PATH 1
#define G_ONE_CYCLE 2
#define G_RANDOM_CYCLES 3
#define G_SQRT_CYCLES 4
#define G_TWO_CYCLES 5
#define G_THREE_CYCLES 6
#define G_SMALL_CYCLES 7
#define G_MIXED_CYCLES 8

#define R_RANDOM 1
#define R_SMALL_BIG 2
#define R_VERY_SMALL_BIG 3
#define R_RARE_CONSEC_BIG 4
#define R_RARE2_CONSEC_VERY_BIG 5
#define R_RARE3_CONSEC_VERY_BIG 6
#define R_BY_CYCLE 7

#define K_SMALL 1
#define K_MEDIUM 2
#define K_BIG 3
#define K_MAX 4

int randomCycleLength()
{
    int total = n - people.size();
    if (total <= 1) return total;
    int length = 2;
    while (true)
    {
        int next = randNum(0, total - 1);
        if (next < length) return length;
        ++length;
    }
}

int generateCycleLength(int gType)
{
    int length;
    switch (gType)
    {
    case G_ONE_PATH:
    case G_ONE_CYCLE:
        length = n;
        break;
    
    case G_RANDOM_CYCLES:
        length = randomCycleLength();
        break;
    
    case G_SQRT_CYCLES:
        length = round(sqrt(n));
        break;
    
    case G_TWO_CYCLES:
        length = 2;
        break;
    
    case G_THREE_CYCLES:
        length = 3;
        break;
    
    case G_SMALL_CYCLES:
        length = randNum(2, 10);
        break;
    
    case G_MIXED_CYCLES:
        if (randNum(0, 1)) length = randomCycleLength();
        else length = 3 + randNum(0, 1) * 2;
        break;

    default:
        throw "unkown gType case";
    }

    length = std::min(length, n - (int) people.size());
    if (people.size() + length == n - 1) ++length;
    if (length == 1) throw "cycle of length 1";

    return length;
}

int generateSmallBigReward(int rType, bool big)
{
    int base, var;
    switch (rType)
    {
    case R_SMALL_BIG:
    case R_RARE_CONSEC_BIG:
        base = big * MAX_REWARD * 5 / 6;
        var = randNum(1, MAX_REWARD / 6);
        return base + var;

    case R_VERY_SMALL_BIG:
    case R_RARE2_CONSEC_VERY_BIG:
    case R_RARE3_CONSEC_VERY_BIG:
        base = big * (MAX_REWARD - 5);
        var = randNum(1, 5);
        return base + var;

    default:
        throw "unknown small big rType";
    }
}

int generateBaseReward()
{
    return randNum(1, MAX_REWARD * 9 / 10);
}

int generateReward(int rType, int baseReward)
{
    switch (rType)
    {
    case R_RANDOM:
        return randNum(1, MAX_REWARD);
    
    case R_SMALL_BIG:
    case R_VERY_SMALL_BIG:
        return generateSmallBigReward(rType, randNum(0, 1));
    
    case R_RARE_CONSEC_BIG:
    case R_RARE2_CONSEC_VERY_BIG:
    case R_RARE3_CONSEC_VERY_BIG:
        return generateSmallBigReward(rType, false);

    case R_BY_CYCLE:
        return baseReward + randNum(0, MAX_REWARD / 10);

    default:
        throw "unknown rType case";
    }
}

void generateCycle(int length, int rType)
{
    int base = people.size();
    int baseReward = generateBaseReward();
    for (int i = 0; i < length; ++i)
    {
        int num = base + i;
        int target = base + (i + 1) % length;
        int reward = generateReward(rType, baseReward);

        people.push_back({num, target, reward});
    }    
}

int generateK(int kType)
{
    switch (kType)
    {
    case K_SMALL:
        return randNum(2, 5);
    
    case K_MEDIUM:
        return randNum(6, n / 10);
    
    case K_BIG:
        return randNum(n / 10, n / 4);
    
    case K_MAX:
        return randNum(n / 2 - 20, n / 2);

    default:
        throw "unknown kType case";
    }
}

void makeCycleIntoPath(int start)
{
    for (Person& person : people)
    {
        if (person.target == start) person.reward = 0;
    }
}

int generateRareRewardLength(int rType)
{
    switch (rType)
    {
    case R_RARE_CONSEC_BIG:
        return randNum(2, 5);
    
    case R_RARE2_CONSEC_VERY_BIG:
        return 2;
    
    case R_RARE3_CONSEC_VERY_BIG:
        return 3;

    default:
        throw "uknown rare reward rType";
    }
}

void generateConsecRareRewards(int rType)
{
    int reps;
    if (rType == R_RARE_CONSEC_BIG) reps = randNum(3, 6);
    else reps = 1;
    for (int i = 0; i < reps; ++i)
    {
        int len = generateRareRewardLength(rType);
        int curr = randNum(0, n - 1);
        for (int j = 0; j < len; ++j)
        {
            people[curr].reward = generateSmallBigReward(rType, true);
            curr = people[curr].target;
        }
    }
}

void generateTest(int gType, int rType, int kType)
{
    people.clear();
    
    n = maxN;
    k = generateK(kType);

    while (people.size() < n)
    {
        int length = generateCycleLength(gType);
        generateCycle(length, rType);
    }

    relabel();

    if (rType == R_RARE_CONSEC_BIG || rType == R_RARE2_CONSEC_VERY_BIG || rType == R_RARE3_CONSEC_VERY_BIG)
    {
        generateConsecRareRewards(rType);
    }

    if (gType == G_ONE_PATH) makeCycleIntoPath(0);
}

const int SML_N = 10;
const int MED_N = 700;
const int BIG_N = 10000;

struct Test
{
    int maxN;
    int gType;
    int rType;
    int kType;
};

vector<Test> tests = {
    {SML_N, G_RANDOM_CYCLES, R_RANDOM, K_SMALL},
    {SML_N, G_ONE_CYCLE, R_RANDOM, K_SMALL}, // 1337, 42
    {SML_N, G_RANDOM_CYCLES, R_VERY_SMALL_BIG, K_SMALL}, // 42, 420
    {SML_N, G_ONE_CYCLE, R_VERY_SMALL_BIG, K_SMALL}, // 1337, 420

    {MED_N, G_ONE_PATH, R_RANDOM, K_SMALL},
    {MED_N, G_ONE_PATH, R_RANDOM, K_MEDIUM},
    {MED_N, G_ONE_PATH, R_SMALL_BIG, K_SMALL},
    {MED_N, G_ONE_PATH, R_VERY_SMALL_BIG, K_MEDIUM}, // 1337
    {MED_N, G_ONE_PATH, R_SMALL_BIG, K_BIG}, // 42
    {MED_N, G_ONE_PATH, R_RARE_CONSEC_BIG, K_BIG}, // 42, 1337
    {MED_N, G_ONE_PATH, R_RARE2_CONSEC_VERY_BIG, K_MAX},

    {BIG_N, G_ONE_PATH, R_RANDOM, K_SMALL},
    {BIG_N, G_ONE_PATH, R_RANDOM, K_MEDIUM}, // 420
    {BIG_N, G_ONE_PATH, R_SMALL_BIG, K_SMALL},
    {BIG_N, G_ONE_PATH, R_SMALL_BIG, K_MEDIUM},
    {BIG_N, G_ONE_PATH, R_VERY_SMALL_BIG, K_BIG},
    {BIG_N, G_ONE_PATH, R_RARE_CONSEC_BIG, K_BIG}, // 1337, 420
    {BIG_N, G_ONE_PATH, R_RARE3_CONSEC_VERY_BIG, K_MAX},

    ///
    {MED_N, G_ONE_CYCLE, R_RANDOM, K_SMALL},
    {MED_N, G_ONE_CYCLE, R_RANDOM, K_MEDIUM},
    {MED_N, G_ONE_CYCLE, R_SMALL_BIG, K_SMALL},
    {MED_N, G_ONE_CYCLE, R_VERY_SMALL_BIG, K_MEDIUM},
    {MED_N, G_ONE_CYCLE, R_SMALL_BIG, K_BIG}, // 42, 420, 555
    {MED_N, G_ONE_CYCLE, R_RARE_CONSEC_BIG, K_BIG}, // 42, 1337, 420
    {MED_N, G_ONE_CYCLE, R_RARE3_CONSEC_VERY_BIG, K_MAX},

    {BIG_N, G_ONE_CYCLE, R_RANDOM, K_SMALL},
    {BIG_N, G_ONE_CYCLE, R_RANDOM, K_MEDIUM},
    {BIG_N, G_ONE_CYCLE, R_SMALL_BIG, K_SMALL},
    {BIG_N, G_ONE_CYCLE, R_SMALL_BIG, K_MEDIUM},
    {BIG_N, G_ONE_CYCLE, R_VERY_SMALL_BIG, K_BIG},
    {BIG_N, G_ONE_CYCLE, R_RARE_CONSEC_BIG, K_BIG}, // 1337, 420
    {BIG_N, G_ONE_CYCLE, R_RARE2_CONSEC_VERY_BIG, K_MAX},

    {MED_N, G_RANDOM_CYCLES, R_RANDOM, K_MEDIUM},
    {MED_N, G_RANDOM_CYCLES, R_VERY_SMALL_BIG, K_SMALL},
    {MED_N, G_SQRT_CYCLES, R_SMALL_BIG, K_SMALL},
    {MED_N, G_SQRT_CYCLES, R_RARE_CONSEC_BIG, K_BIG}, // 42, 1337, 420, 555
    {MED_N, G_TWO_CYCLES, R_RANDOM, K_MEDIUM}, // 42, 1337, 420
    {MED_N, G_THREE_CYCLES, R_SMALL_BIG, K_BIG},
    {MED_N, G_THREE_CYCLES, R_RARE3_CONSEC_VERY_BIG, K_MAX},
    {MED_N, G_SMALL_CYCLES, R_RARE3_CONSEC_VERY_BIG, K_BIG},
    {MED_N, G_MIXED_CYCLES, R_RARE2_CONSEC_VERY_BIG, K_MAX},

    {BIG_N, G_RANDOM_CYCLES, R_RANDOM, K_SMALL},
    {BIG_N, G_RANDOM_CYCLES, R_VERY_SMALL_BIG, K_MEDIUM},
    {BIG_N, G_SQRT_CYCLES, R_SMALL_BIG, K_SMALL},
    {BIG_N, G_SQRT_CYCLES, R_RARE_CONSEC_BIG, K_MEDIUM},
    {BIG_N, G_TWO_CYCLES, R_RANDOM, K_SMALL},
    {BIG_N, G_THREE_CYCLES, R_SMALL_BIG, K_BIG},
    {BIG_N, G_THREE_CYCLES, R_RARE2_CONSEC_VERY_BIG, K_MAX},
    {BIG_N, G_SMALL_CYCLES, R_RARE2_CONSEC_VERY_BIG, K_BIG},
    {BIG_N, G_MIXED_CYCLES, R_RARE3_CONSEC_VERY_BIG, K_MAX} // 1337
};

int main()
{
    generator.seed(9000);

    ofstream infile;
    for (int num = 1; num <= tests.size(); ++num)
    {
        maxN = tests[num - 1].maxN;
        int gType = tests[num - 1].gType;
        int rType = tests[num - 1].rType;
        int kType = tests[num - 1].kType;

        cerr << "test " << num << " : " << maxN << " " << gType << " " << rType << " " << kType << endl;
        string digs = "";
        digs += (num / 10 + '0');
        digs += (num % 10 + '0');
        infile.open("hunterxhunter." + digs + ".in");

        try
        {
            generateTest(gType, rType, kType);
        }
        catch (char *const s)
        {
            cerr << "ERROR: " << s << endl;
            --num;
            continue;
        }

        verifyTest();
        printTest(infile);

        infile.close();
        //++num;
    }
    return 0;
}