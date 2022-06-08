#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>
#include <functional>

const int MAX_N = 1e6;
const long long MAX_VAL = 1e12;
const long long MAX_VAL_USE = MAX_VAL - 100;

std::mt19937 generator;
long long randNum(long long lb, long long ub)
{
    std::uniform_int_distribution<long long> distribution(lb, ub - 1);
    return distribution(generator);
}
bool oneIn(long long chances)
{
    if (chances == 0) return false;
    return randNum(0, chances) == 0;
}

#define C_ONE 1
#define C_FEW 2
#define C_SQRT 3
#define C_MANY 4
#define C_HALF 5
#define C_MOST 6
#define C_N 7

#define T_RAND 1
#define T_EQUALS 2
#define T_SLOPES 3
#define T_PARABOLA 4
#define T_SQRT 5
#define T_LOG 6
#define T_FLAT_SLOPE_RAND 7

#define V_FIXED 1
#define V_VARY 2
#define V_VARY_ALOT 3

int n;
int type;
int kCt;
int ct;
int vt;

long long r[MAX_N];

bool oneInRec(long long cnt)
{
    if (cnt == 1) return false;
    return oneIn(n / (cnt - 1));
}

int getCnt(int ct)
{
    if (ct == C_ONE) return 1;
    else if (ct == C_FEW) return 10;
    else if (ct == C_SQRT) return sqrt(n);
    else if (ct == C_MANY) return n / 10;
    else if (ct == C_HALF) return n / 3;
    else if (ct == C_MOST) return 3 * n / 4;
    else if (ct == C_N) return n;
    else exit(0);
}

bool genTest(std::ofstream& outfile)
{
    int k = getCnt(kCt);

    if (type == T_RAND)
    {
        for (int i = 0; i < n; ++i)
        {
            r[i] = randNum(0, MAX_VAL_USE);
        }
    }
    else if (type == T_EQUALS)
    {
        int eqCnt = getCnt(ct);

        r[0] = randNum(0, MAX_VAL_USE);

        for (int i = 1; i < n; ++i)
        {
            if (oneInRec(eqCnt)) r[i] = randNum(0, MAX_VAL_USE);
            else r[i] = r[i - 1];

            if (vt == V_VARY) r[i] = r[i] + randNum(0, 3) - 1;
            else if (vt == V_VARY_ALOT) r[i] = r[i] + randNum(0, 30) - 15;
        }

    }
    else if (type == T_SLOPES)
    {
        int slopeCnt = getCnt(ct);

        long long change = randNum(1, MAX_VAL_USE / n);
        r[0] = 0;

        for (int i = 1; i < n; ++i)
        {
            if (oneInRec(slopeCnt)) change = randNum(1, MAX_VAL_USE / n);

            if (vt == V_FIXED) r[i] = r[i - 1] + change;
            else if (vt == V_VARY) r[i] = r[i - 1] + change + randNum(0, 3) - 1;
            else if (vt == V_VARY_ALOT) r[i] = r[i - 1] + randNum(1, change + 1);
        }
    }
    else if (type == T_PARABOLA)
    {
        long double max = (long double) n * n;
        long double scale = MAX_VAL_USE / max;

        for (int i = 0; i < n; ++i)
        {
            r[i] = roundl((long double) i * i * scale);

            if (vt == V_VARY) r[i] = r[i] + randNum(0, 3) - 1;
            else if (vt == V_VARY_ALOT) r[i] = r[i] + randNum(0, 30) - 15;
        }
    }
    else if (type == T_SQRT)
    {
        long double max = sqrtl(n);
        long double scale = MAX_VAL_USE / max;

        for (int i = 0; i < n; ++i)
        {
            r[i] = roundl(sqrtl(i) * scale);

            if (vt == V_VARY) r[i] = r[i] + randNum(0, 3) - 1;
            else if (vt == V_VARY_ALOT) r[i] = r[i] + randNum(0, 30) - 15;
        }
    }
    else if (type == T_LOG)
    {
        long double max = sqrtl(n + 1);
        long double scale = MAX_VAL_USE / max;

        for (int i = 0; i < n; ++i)
        {
            r[i] = roundl(sqrtl(i + 1) * scale);

            if (vt == V_VARY) r[i] = r[i] + randNum(0, 3) - 1;
            else if (vt == V_VARY_ALOT) r[i] = r[i] + randNum(0, 30) - 15;
        }
    }
    else if (type == T_FLAT_SLOPE_RAND)
    {
        int len;
        if (k <= n / 2 && oneIn(2)) len = n - k - randNum(0, n / 10);
        else len = randNum(n / 3, 5 * n / 6);
        long long min = randNum(0, MAX_VAL_USE - 2 * len);

        r[0] = min;

        for (int i = 1; i < len; ++i)
        {
            r[i] = r[i - 1] + 1;

            if (vt != V_FIXED) r[i] += randNum(0, 2);
        }
    
        long long max = r[len - 1];

        for (int i = len; i < n; ++i)
        {
            r[i] = randNum(0, MAX_VAL_USE);

            if (r[i] >= min && r[i] <= max)
            {
                --i;
                continue;
            }
        }
    }

    long long min = r[0];
    long long max = r[0];

    for (int i = 0; i < n; ++i)
    {
        min = std::min(min, r[i]);
        max = std::max(max, r[i]);
    }

    if (max - min > MAX_VAL) return false;

    long long shift = (MAX_VAL - (max + min)) / 2;
    bool flip = oneIn(2);

    for (int i = 0; i < n; ++i)
    {
        r[i] += shift;
        if (flip) r[i] = MAX_VAL - r[i];
    }

    std::shuffle(r, r + n, generator);

    // std::sort(r, r + n);

    outfile << n << ' ' << k << '\n';
    for (int i = 0; i < n; ++i)
    {
        assert(0 <= r[i] && r[i] <= MAX_VAL);
        if (i > 0) outfile << ' ';
        outfile << r[i];
    }

    outfile << "\n";
    outfile << std::flush;

    return true;
}

void setParams(int i)
{
    type = 0;
    kCt = 0;
    ct = 0;
    vt = 0;

    if (i == 0)
    {
        type = T_RAND;
        kCt = C_ONE;
    }
    else if (i == 1)
    {
        type = T_RAND;
        kCt = C_FEW;
    }
    else if (i == 2)
    {
        type = T_RAND;
        kCt = C_SQRT;
    }
    else if (i == 3)
    {
        type = T_RAND;
        kCt = C_MANY;
    }
    else if (i == 4)
    {
        type = T_RAND;
        kCt = C_MOST;
    }
    else if (i == 5)
    {
        type = T_RAND;
        kCt = C_N;
    }
    else if (i == 6)
    {
        type = T_EQUALS;
        kCt = C_HALF;
        ct = C_ONE;
        vt = V_FIXED;
    }
    else if (i == 7)
    {
        type = T_EQUALS;
        kCt = C_SQRT;
        ct = C_SQRT;
        vt = V_FIXED;
    }
    else if (i == 8)
    {
        type = T_EQUALS;
        kCt = C_FEW;
        ct = C_SQRT;
        vt = V_VARY_ALOT;
    }
    else if (i == 9)
    {
        type = T_SLOPES;
        kCt = C_SQRT;
        ct = C_ONE;
        vt = V_FIXED;
    }
    else if (i == 10)
    {
        type = T_SLOPES;
        kCt = C_HALF;
        ct = C_ONE;
        vt = V_VARY;
    }
    else if (i == 11)
    {
        type = T_SLOPES;
        kCt = C_FEW;
        ct = C_FEW;
        vt = V_VARY_ALOT;
    }
    else if (i == 12)
    {
        type = T_SLOPES;
        kCt = C_SQRT;
        ct = C_FEW;
        vt = V_VARY;
    }
    else if (i == 13)
    {
        type = T_SLOPES;
        kCt = C_MOST;
        ct = C_FEW;
        vt = V_VARY;
    }
    else if (i == 14)
    {
        type = T_SLOPES;
        kCt = C_FEW;
        ct = C_SQRT;
        vt = V_VARY;
    }
    else if (i == 15)
    {
        type = T_SLOPES;
        kCt = C_SQRT;
        ct = C_SQRT;
        vt = V_VARY;
    }
    else if (i == 16)
    {
        type = T_SLOPES;
        kCt = C_HALF;
        ct = C_SQRT;
        vt = V_FIXED;
    }
    else if (i == 17)
    {
        type = T_SLOPES;
        kCt = C_SQRT;
        ct = C_MANY;
        vt = V_FIXED;
    }
    else if (i == 18)
    {
        type = T_SLOPES;
        kCt = C_MANY;
        ct = C_MANY;
        vt = V_VARY_ALOT;
    }
    else if (i == 19)
    {
        type = T_SLOPES;
        kCt = C_MOST;
        ct = C_MANY;
        vt = V_FIXED;
    }
    else if (i == 20)
    {
        type = T_PARABOLA;
        kCt = C_SQRT;
        vt = V_VARY;
    }
    else if (i == 21)
    {
        type = T_PARABOLA;
        kCt = C_MANY;
        vt = V_FIXED;
    }
    else if (i == 22)
    {
        type = T_PARABOLA;
        kCt = C_MOST;
        vt = V_VARY_ALOT;
    }
    else if (i == 23)
    {
        type = T_SQRT;
        kCt = C_SQRT;
        vt = V_FIXED;
    }
    else if (i == 24)
    {
        type = T_SQRT;
        kCt = C_HALF;
        vt = V_VARY_ALOT;
    }
    else if (i == 25)
    {
        type = T_SQRT;
        kCt = C_MOST;
        vt = V_VARY;
    }
    else if (i == 26)
    {
        type = T_LOG;
        kCt = C_SQRT;
        vt = V_FIXED;
    }
    else if (i == 27)
    {
        type = T_LOG;
        kCt = C_MANY;
        vt = V_VARY_ALOT;
    }
    else if (i == 28)
    {
        type = T_LOG;
        kCt = C_HALF;
        vt = V_VARY;
    }
    else if (i == 29)
    {
        type = T_FLAT_SLOPE_RAND;
        kCt = C_SQRT;
        vt = V_FIXED;
    }
    else if (i == 30)
    {
        type = T_FLAT_SLOPE_RAND;
        kCt = C_SQRT;
        vt = V_VARY_ALOT;
    }
    else if (i == 31)
    {
        type = T_FLAT_SLOPE_RAND;
        kCt = C_MANY;
        vt = V_VARY;
    }
    else if (i == 32)
    {
        type = T_FLAT_SLOPE_RAND;
        kCt = C_MANY;
        vt = V_FIXED;
    }
    else if (i == 33)
    {
        type = T_FLAT_SLOPE_RAND;
        kCt = C_HALF;
        vt = V_FIXED;
    }
    else if (i == 34)
    {
        type = T_FLAT_SLOPE_RAND;
        kCt = C_MOST;
        vt = V_FIXED;
    }
    else if (i == 35)
    {
        type = T_FLAT_SLOPE_RAND;
        kCt = C_MOST;
        vt = V_VARY_ALOT;
    }
    else
    {
        std::cerr << "UNKNOWN " << i << std::endl;
        exit(0);
    }
}

void manyGen(int _n, int num, int tests)
{
    n = _n;

    std::vector<int> testNums(36);
    std::iota(testNums.begin(), testNums.end(), 0);

    if (tests < 15)
    {
        std::shuffle(testNums.begin(), testNums.end(), generator);
    }
    else
    {
        std::reverse(testNums.begin() + 6, testNums.end());
        std::shuffle(testNums.begin() + 13, testNums.end(), generator);
        std::shuffle(testNums.begin(), testNums.begin() + 30, generator);
    }

    for (int i = 0; i < tests; ++i)
    {
        std::string nums = "";
        nums += (num / 10 + '0');
        nums += (num % 10 + '0');
        std::string name = "ram." + nums + ".in";
        std::ofstream outfile(name);

        setParams(testNums[i]);

        std::cerr << name << " " << type << " " << ct << " " << vt << " " << kCt << std::endl;

        genTest(outfile);
        outfile.close();

        ++num;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);

    generator.seed(time(nullptr));

    manyGen(10, 1, 4); // 1 - 4
    manyGen(1000, 5, 4); // 5 - 8
    manyGen(8000, 9, 28); // 9 - 36
    manyGen(12000, 37, 27); // 37 - 63
    manyGen(1000000, 64, 36); // 66 - 99

    // int n, num, tests;
    // while (true)
    // {
    //     num = -1;
    //     std::cout << "Test num? ";
    //     std::cin >> num;
    //     if (num < 0) return 0;
    //     std::cout << "n tests (max 36)? ";
    //     std::cin >> n >> tests;
    //     manyGen(num, tests);
    // }

    return 0;
}
