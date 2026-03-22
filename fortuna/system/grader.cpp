#include "fortuna.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <chrono>

static std::ofstream graderTimeFile;

static std::chrono::steady_clock::time_point startT;
static std::chrono::steady_clock::time_point endT;

bool timing;
static double totalGraderTime = 0;

static void startTime()
{
    startT = std::chrono::steady_clock::now();

    timing = true;
}

static void writeTime()
{
    graderTimeFile << "io-time: " << totalGraderTime << std::endl;
}

static void endTime(bool write = true, bool dontEnd = false)
{
    if (!timing) return;

    endT = std::chrono::steady_clock::now();

    double time = std::chrono::duration_cast<std::chrono::duration<double>>(endT - startT).count();

    totalGraderTime += time;

    if (write)
    {
        writeTime();
    }

    if (dontEnd)
    {
        startT = endT;
    }
    else
    {
        timing = false;
    }
}

static void logTime()
{
    endTime(true, true);
}

static const std::string PASSWORD = "ch3e9v47slxyu54io2jud";

static void myAssert(bool cond, const char* msg)
{
    if (cond) return;

    endTime();

    std::cout << PASSWORD << std::endl;
    std::cout << -1 << std::endl;
    std::cout << msg << std::endl;
    std::cout << totalGraderTime << std::endl;
    exit(0);
}

static const int MAX_N = 30;
static const int MAX_M = 30;
static const int MAX_CODES = 500;

static const int REPS = 1e6;
static const int MAX_COST = 100;

static int n;
static int m;

static int currCode;
static int newCode[MAX_CODES];
static bool vis[MAX_CODES];
static std::pair<bool, int> acts[MAX_CODES][MAX_N];

static void DFS(int curr)
{
    if (vis[curr]) return;
    vis[curr] = true;
    newCode[curr] = currCode;
    currCode++;

    for (int r = 0; r < n; r++)
    {
        auto [done, next] = proc(curr, r);

        if (done) myAssert(next >= 0 && next < m, "Invalid output number");
        else myAssert(next >= 0 && next < MAX_CODES, "Invalid next value");

        if (!done) DFS(next);

        acts[newCode[curr]][r] = {done, done ? next : newCode[next]};
    }
}

static int len;
static int origMatrix[MAX_CODES][MAX_CODES + MAX_M + 1];
static long long matrix[MAX_CODES][MAX_CODES + MAX_M + 1];

static long long modQPow(long long a, long long p, long long mod)
{
    if (p == 0) return 1;
    long long next = modQPow(a, p / 2, mod);
    next = (next * next) % mod;
    if (p % 2) next = (next * a) % mod;
    return next;
}

static long long modInv(long long a, long long mod)
{
    return modQPow(a, mod - 2, mod);
}

static bool gaussMod(long long mod)
{
    for (int i = 0; i < currCode; i++)
    {
        for (int j = 0; j < len; j++)
        {
            matrix[i][j] = origMatrix[i][j];
        }
    }

    for (int i = 0; i < currCode; i++)
    {
        int pivot = -1;
        for (int j = i; j < currCode; j++)
        {
            if (matrix[j][i])
            {
                pivot = j;
                break;
            }
        }

        if (pivot == -1)
        {
            return false;
        }

        for (int k = i; k < len; k++)
        {
            std::swap(matrix[i][k], matrix[pivot][k]);
        }

        int mul = modInv(matrix[i][i], mod);

        for (int k = i; k < len; k++)
        {
            matrix[i][k] = (matrix[i][k] * mul) % mod;
        }

        for (int j = 0; j < currCode; j++)
        {
            if (j == i) continue;

            long long mul = matrix[j][i];

            for (int k = i; k < len; k++)
            {
                matrix[j][k] = ((matrix[j][k] - matrix[i][k] * mul) % mod + mod) % mod;
            }
        }
    }

    long long target = modInv(m, mod);

    for (int i = 0; i < m; i++)
    {
        myAssert(matrix[0][currCode + i] == target, "Not uniform");
    }

    return true;
}

static std::mt19937 gen(150749617);

static int totalCost;

static int genNumber()
{
    std::uniform_int_distribution<int> distr(0, n - 1);

    int c = 0;
    while (true)
    {
        totalCost++;

        myAssert(totalCost <= REPS * MAX_COST, "Too many iterations");

        if (totalCost % (int) 1e7 == 0)
        {
            logTime();
        }

        int r = distr(gen);

        auto [done, next] = acts[c][r];

        if (done) return next;

        c = next;
    }
}

static const int NUM_PRIMES = 5;
static const long long PRIMES[NUM_PRIMES] = {
    680899007, 759078491, 864349663, 955387067, 990303059
};

int main(int argc, char* argv[])
{
    graderTimeFile.open(argv[1]);

    std::cin >> n >> m;

    setup(n, m);

    DFS(0);

    startTime();

    len = currCode + m + 1;

    for (int c = 0; c < currCode; c++)
    {
        origMatrix[c][c] += n;
        origMatrix[c][currCode + m] += n;
        for (int r = 0; r < n; r++)
        {
            auto [done, next] = acts[c][r];
            if (done)
            {
                origMatrix[c][currCode + next] += 1;
            }
            else
            {
                origMatrix[c][next] -= 1;
            }
        }
    }

    int succCnt = 0;

    for (long long p : PRIMES)
    {
        if (gaussMod(p)) succCnt++;

        logTime();
    }

    myAssert(succCnt >= 1, "Does not terminate");

    myAssert(succCnt >= NUM_PRIMES / 2, "WARNING: Unexpected degeneracy");

    for (int t = 0; t < REPS; t++)
    {
        genNumber();
    }

    endTime();

    std::cout << PASSWORD << std::endl;
    std::cout << std::fixed << std::setprecision(6) << (double) totalCost / REPS << std::endl;
    std::cout << totalGraderTime << std::endl;

    return 0;
}
