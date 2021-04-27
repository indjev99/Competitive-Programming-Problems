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

const int MAX_K = 3;
const int MAX_S = 2e6;
const int MAX_N = 5e5;
const long long MAX_POS = 1e8;

int s;
int n, k;
bool eq;

long long r[MAX_K];
long long pos[MAX_N];

long long sum;

void printTest(ostream& outfile)
{
    sum += n;
    assert(sum <= MAX_S);
    assert(k < n && n <= s / 4);
    assert(1 <= k && k <= MAX_K);
    outfile << n << " " << k << "\n";
    std::shuffle(r, r + k, generator);
    for (int i = 0; i < k; ++i)
    {
        assert(1 <= r[i] && r[i] <= MAX_POS);
        assert(!eq || i == 0 || r[i] == r[i - 1]);
        if (i > 0) outfile << " ";
        outfile << r[i];
    }
    outfile << "\n";
    for (int i = 0; i < n; ++i)
    {
        assert(0 <= pos[i] && pos[i] <= MAX_POS);
        assert(i == 0 || pos[i - 1] < pos[i]);
        if (i > 0) outfile << " ";
        outfile << pos[i];
    }
    outfile << "\n";
}

#define WHITE 0
#define GRAY 1
#define BLACK 2

#define GW 0
#define BW 1
#define GWW 2
#define BWW 3

const long long INF = 1e9;

const int NUM_STATES = 4;
const int MAX_BETWEEN_WHITE = 4;

const int MAX_M = MAX_N + 2;

int m;
int pos2[MAX_M];

bool possible[MAX_M][NUM_STATES];
int last[MAX_M][NUM_STATES][MAX_K - 1];

int possibleBruteforce(int idx, int iters, int whitePos, int firstGray, int firstBlack)
{
    if ((k <= GRAY && firstGray > -INF) || (k <= BLACK && firstBlack > -INF)) return false;
    if (idx < 0) return true;

    if (whitePos - pos2[idx] >= r[WHITE])
    {
        for (int j = 0; j < NUM_STATES; ++j)
        {
            if (possible[idx][j] && firstGray - last[idx][j][GRAY - 1] >= r[GRAY]
                && firstBlack - last[idx][j][BLACK - 1] >= r[BLACK]) return true;
        }
    }

    if (iters == 0) return false;

    if (firstGray - pos2[idx] >= r[GRAY]
        && possibleBruteforce(idx - 1, iters - 1, whitePos, pos2[idx], firstBlack))
        return true;

    if (firstBlack - pos2[idx] >= r[BLACK]
        && possibleBruteforce(idx - 1, iters - 1, whitePos, firstGray, pos2[idx]))
        return true;

    return false;
}

int lastBruteforce(int target, int idx, int iters, int whitePos, int firstOther)
{
    int ans = INF;
    int other = target == GRAY ? BLACK : GRAY;

    if (k <= other && firstOther > -INF) return INF;
    if (idx < 0) return -INF;

    if (whitePos - pos2[idx] >= r[WHITE])
    {
        for (int j = 0; j < NUM_STATES; ++j)
        {
            if (possible[idx][j] && firstOther - last[idx][j][other - 1] >= r[other])
                ans = std::min(ans, last[idx][j][target - 1]);
        }
    }

    if (iters == 0) return INF;

    if (firstOther - pos2[idx] >= r[other])
        ans = std::min(ans, lastBruteforce(target, idx - 1, iters - 1, whitePos, pos2[idx]));

    if (ans < INF) return ans;

    if ((target == GRAY && possibleBruteforce(idx - 1, iters - 1, whitePos, pos2[idx], firstOther))
        || (target == BLACK && possibleBruteforce(idx - 1, iters - 1, whitePos, firstOther, pos2[idx])))
        return pos2[idx];

    return INF;
}

bool check()
{
    m = 0;
    for (int i = 0; i < n; ++i)
    {
        if ((i > 0 && pos[i] - pos[i - 1] < r[WHITE])
            || (i < n - 1 && pos[i + 1] - pos[i] < r[WHITE]))
            pos2[m++] = pos[i];
    }
    pos2[m++] = INF;

    if (m == 0) return true;

    for (int i = k; i < MAX_K; ++i)
    {
        r[i] = 0;
    }

    possible[0][GW] = true;
    last[0][GW][GRAY - 1] = -INF;
    last[0][GW][BLACK - 1] = -INF;

    possible[0][BW] = false;
    possible[0][GWW] = false;
    possible[0][BWW] = false;

    for (int i = 1; i < m; ++i)
    {
        int lastGray = lastBruteforce(GRAY, i - 2, MAX_BETWEEN_WHITE - 1, pos2[i], pos2[i - 1]);
        int lastBlack = lastBruteforce(BLACK, i - 2, MAX_BETWEEN_WHITE - 1, pos2[i], pos2[i - 1]);

        possible[i][GW] = lastBlack < INF;
        last[i][GW][GRAY - 1] = pos2[i - 1];
        last[i][GW][BLACK - 1] = lastBlack;

        possible[i][BW] = lastGray < INF;
        last[i][BW][GRAY - 1] = lastGray;
        last[i][BW][BLACK - 1] = pos2[i - 1];

        if (pos2[i] - pos2[i - 1] >= r[WHITE])
        {
            possible[i][GWW] = possible[i - 1][GW];
            last[i][GWW][GRAY - 1] = last[i - 1][GW][GRAY - 1];
            last[i][GWW][BLACK - 1] = last[i - 1][GW][BLACK - 1];

            possible[i][BWW] = possible[i - 1][BW];
            last[i][BWW][GRAY - 1] = last[i - 1][BW][GRAY - 1];
            last[i][BWW][BLACK - 1] = last[i - 1][BW][BLACK - 1];
        }
        else
        {
            possible[i][GWW] = false;
            possible[i][BWW] = false;
        }
    }

    for (int i = 0; i < NUM_STATES; ++i)
    {
        if (possible[m - 2][i]) return true;
        if (possible[m - 1][i]) return true;
    }
    return false;
}

long long r2[MAX_K];

bool tryR2s(double s)
{
    copy(r2, r2 + k, r);
    for (int i = 0; i < k; ++i)
    {
        r[i] = round(s * r2[i]);
    }
    sort(r, r + k);
    return check();
}

const int MAX_SMALL_R2 = 2;
const int MAX_MID_R2 = 5;
const int MAX_BIG_R2 = 15;

void genRandR2()
{
    r2[0] = randNum(1, MAX_SMALL_R2);
    for (int i = 1; i < k; ++i)
    {
        if (oneIn(2)) r2[i] = randNum(1, MAX_MID_R2);
        else r2[i] = randNum(1, MAX_BIG_R2);
    }
    shuffle(r2, r2 + k, generator);
}

void genEqualR2()
{
    for (int i = 0; i < k; ++i)
    {
        r2[i] = 1;
    }
}

void genPercintileR2(double p)
{
    std::vector<long long> dists;
    for (int i = 1; i < n; ++i)
    {
        dists.push_back(pos[i] - pos[i - 1]);
    }
    std::sort(dists.begin(), dists.end());
    long long minR = dists[round((n - 1) * p)] + 1;

    for (int i = 0; i < k; ++i)
    {
        r2[i] = minR;
    }
}

bool useSeedR2;
bool finalR2;

bool genR()
{
    if (!useSeedR2)
    {
        if (oneIn(2) || eq) genEqualR2();
        else genRandR2();
    }

    if (finalR2)
    {
        copy(r2, r2 + k, r);
        std::sort(r, r + k);

        return true;
    }

    std::sort(r2, r2 + k);

    /*cerr << "\nTrying initial Rs:";
    for (int i = 0; i < k; ++i)
    {
        cerr << " " << r2[i];
    }
    cerr << endl;*/

    for (int i = 0; i < k; ++i)
    {
        if (r2[i] > pos[n - 1] - pos[0])
        {
            //std::cerr << "A color is used only once: " << i << std::endl;
            return false;
        }
    }

    if (!tryR2s(1))
    {
        //cerr << "No solution!" << std::endl;
        return false;
    }

    double left = 1;
    double right = (pos[n - 1] - pos[0]) / r2[k - 1];

    for (int i = 0; i < 32; ++i)
    {
        double mid = (left + right) / 2;
        if (tryR2s(mid)) left = mid;
        else right = mid;
    }

    for (int i = 0; i < k; ++i)
    {
        r2[i] = round(left * r2[i]);
    }

    /*cerr << "Same ratio Rs:";
    for (int i = 0; i < k; ++i)
    {
        cerr << " " << r2[i];
    }
    cerr << endl;*/

    if (!eq)
    {
        long long toAdd = 1;
        while (toAdd < pos[n - 1] - pos[0]) toAdd <<= 1;

        while (toAdd > 0)
        {
            for (int i = 0; i < k; ++i)
            {
                r2[i] += toAdd;
                if (!tryR2s(1)) r2[i] -= toAdd;
                if (r2[i] > pos[n - 1] - pos[0])
                {
                    //std::cerr << "A color is used only once: " << i << std::endl;
                    return false;
                }
            }
            toAdd >>= 1;
        }

        /*cerr << "Maximal Rs:";
        for (int i = 0; i < k; ++i)
        {
            cerr << " " << r2[i];
        }
        cerr << endl;*/

        assert(tryR2s(1));
        for (int i = 0; i < k; ++i)
        {
            ++r2[i];
            assert(!tryR2s(1));
            --r2[i];
        }

        if (oneIn(2))
        {
            std::shuffle(r2, r2 + k, generator);
            int failIdx;
            do
            {
                failIdx = randNum(0, k - 1);
            }
            while (r2[failIdx] > pos[n - 1] - pos[0]);
            ++r2[failIdx];
            //cerr << "Failed on: " << failIdx << endl;
        }
    }
    else
    {
        if (oneIn(2))
        {
            for (int i = 0; i < k; ++i)
            {
                ++r2[i];
            }
            //cerr << "Failed on all" << endl;
        }
    }

    copy(r2, r2 + k, r);
    std::sort(r, r + k);

    return true;
}

unordered_set<int> used;

void genRandPos()
{
    useSeedR2 = false;

    used.clear();
    for (int i = 0; i < n; ++i)
    {
        do
        {
            pos[i] = randNum(0, MAX_POS);
        }
        while (used.find(pos[i]) != used.end());
        used.insert(pos[i]);
    }
    sort(pos, pos + n);
}

void genContPos(bool tight)
{
    useSeedR2 = true;

    if (eq) genEqualR2();
    else genRandR2();

    long long currPos = 0;
    int numPossible;
    int possible[MAX_K];
    long long last[MAX_K] = {-INF, -INF, -INF};
    for (int i = 0; i < n; ++i)
    {
        int chosen = randNum(0, k - 1);
        currPos = std::max(currPos + 1, last[chosen] + r2[chosen]);
        if (!tight)
        {
            numPossible = 0;
            for (int j = 0; j < k; ++j)
            {
                if (currPos - last[j] >= r2[j]) possible[numPossible++] = j;
            }
            chosen = possible[randNum(0, numPossible - 1)];
        }
        last[chosen] = currPos;
        pos[i] = currPos;
    }
    assert(pos[n - 1] <= MAX_POS);

    long long ratio = MAX_POS / pos[n - 1];
    for (int i = 0; i < n; ++i)
    {
        pos[i] *= ratio;
    }

    for (int i = 0; i < k; ++i)
    {
        r2[i] *= ratio;
    }
    
    /*std::cerr << "\nIntended Rs:";
    for (int i = 0; i < k; ++i)
    {
        std::cerr << " " << r2[i];
    }
    std::cerr << std::endl;*/
}

bool genAntiBFPos(bool fail)
{
    assert(k >= 2 && !eq);

    useSeedR2 = true;
    finalR2 = true;

    r2[WHITE] = randNum(5, 10);
    r2[GRAY] = r2[WHITE] + 1;
    int base = n * r2[WHITE] / (k - 1) / 3;
    r2[k - 1] = randNum(base * 3 / 4, base * 5 / 4);

    bool failed = false;
    int failPos = randNum(n / 3, 2 * n / n);
    int currIdx = 0;
    long long currPos = 0;
    int numPossible;
    int possible[MAX_K];
    long long last[MAX_K] = {-INF, -INF, -INF};
    while (currIdx < n)
    {
        last[k - 1] = currPos;
        int nextBlack = currPos + r2[k - 1];
        if (fail && !failed && currPos >= failPos)
        {
            --nextBlack;
            failed = true;
        }
        pos[currIdx++] = currPos;

        while (currIdx < n)
        {
            int chosen = randNum(0, k - 2);
            if (currIdx == 2)
            {
                for (int j = 0; j < k - 1; ++j)
                {
                    if (last[j] == -INF) chosen = j;
                }
            }
            currPos = std::max(currPos + 1, last[chosen] + r2[chosen]);
            if (currPos + r2[k - 2] > nextBlack - k + 1) break;
            numPossible = 0;
            for (int j = 0; j < k - 1; ++j)
            {
                if (currPos - last[j] >= r2[j]) possible[numPossible++] = j;
            }
            chosen = possible[randNum(0, numPossible - 1)];
            last[chosen] = currPos;
            pos[currIdx++] = currPos;
        }

        currPos = nextBlack - k + 1;
        for (int i = 0; i < k - 1 && currIdx < n; ++i)
        {
            last[i] = currPos;
            pos[currIdx++] = currPos++;
        }
    }
    assert(pos[n - 1] <= MAX_POS);

    long long ratio = MAX_POS / pos[n - 1];
    for (int i = 0; i < n; ++i)
    {
        pos[i] *= ratio;
    }

    for (int i = 0; i < k; ++i)
    {
        r2[i] *= ratio;
    }

    /*std::cerr << "\nIntended Rs:";
    for (int i = 0; i < k; ++i)
    {
        std::cerr << " " << r2[i];
    }
    std::cerr << std::endl;*/

    if (fail && !failed)
    {
        //std::cerr << "Meant to fail but didn't." << std::endl;
        return false;
    }

    if (fail) assert(!tryR2s(1));
    else assert(tryR2s(1));

    return true;
}

int nonTrivialTowers()
{
    int m = 0;
    for (int i = 0; i < n; ++i)
    {
        if ((i > 0 && pos[i] - pos[i - 1] < r[0])
            || (i < n - 1 && pos[i + 1] - pos[i] < r[0])) ++m;
    }
    return m;
}

void genTest(ostream& outfile)
{
    int posMode = randNum(0, 4);
    if (k == 1 || eq || n <= 10) posMode = randNum(0, 2);
    do
    {
        if (posMode <= 0) genRandPos();
        else if (posMode <= 2) genContPos(oneIn(posMode % 2));
        else while (!genAntiBFPos(posMode % 2)) {}
    }
    while (!genR() || nonTrivialTowers() == 0);

    //std::cerr << "Non trivial towers: " << nonTrivialTowers() << std::endl;

    printTest(outfile);
}

void genBigTest(ostream& outfile, int idx)
{
    sum = 0;
    n = s / 4;
    if (idx % 3 == 1 && s >= 500)
    {
        n = 100;
    }
    int t = s / n;
    t = std::min(t, 1000);
    n = s / t;
    outfile << t << endl;
    for (int i = 0; i < t; ++i)
    {
        std::cerr << " " << i;
        genTest(outfile);
    }
    std::cerr << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        s = atoi(argv[1]);
        k = atoi(argv[2]);
        eq = atoi(argv[3]);
        generator.seed(atoi(argv[4]));
        genBigTest(cout, oneIn(3));
        return 0;
    }

    generator.seed(time(nullptr));
    //generator.seed(0);

    int num;
    int targetType;
    while (true)
    {
        cout << "\nTest num? ";
        cin >> num;
        if (num < 0) return 0;
        cout << "s k equal? ";
        cin >> s >> k >> eq;
        for (int i = 0; i < 10; ++i)
        {
            std::cerr << "Test " << num << std::endl;
            string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');
            ofstream outfile("towers." + nums + ".in");
            genBigTest(outfile, i);
            outfile.close();
            ++num;
        }
    }
    return 0;
}
