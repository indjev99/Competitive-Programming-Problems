#include "fortuna.h"

#include <iostream>
#include <iomanip>

static void myAssert(bool cond, const char* msg)
{
    if (cond) return;

    std::cout << msg << std::endl;
    exit(0);
}

static const int MAX_N = 30;
static const int MAX_M = 30;
static const int MAX_CODES = 500;

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
static long double matrixReal[MAX_CODES][MAX_CODES + MAX_M + 1];

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

static const long double EPS = 1e-6;

static double gausReal()
{
    for (int i = 0; i < currCode; i++)
    {
        for (int j = 0; j < len; j++)
        {
            matrixReal[i][j] = origMatrix[i][j];
        }
    }

    for (int i = 0; i < currCode; i++)
    {
        int pivot = i;
        for (int j = i + 1; j < currCode; j++)
        {
            if (std::abs(matrixReal[j][i]) > std::abs(matrixReal[pivot][i]))
            {
                pivot = j;
                break;
            }
        }

        myAssert(std::abs(matrixReal[pivot][i]) > EPS, "Unexpected real degeneracy");

        for (int k = i; k < len; k++)
        {
            std::swap(matrixReal[i][k], matrixReal[pivot][k]);
        }

        long double mul = 1 / matrixReal[i][i];

        for (int k = i; k < len; k++)
        {
            matrixReal[i][k] = matrixReal[i][k] * mul;
        }

        for (int j = 0; j < currCode; j++)
        {
            if (j == i) continue;

            long double mul = matrixReal[j][i];

            for (int k = i; k < len; k++)
            {
                matrixReal[j][k] = matrixReal[j][k] - matrixReal[i][k] * mul;
            }
        }
    }

    return matrixReal[0][currCode + m];
}

static const int NUM_PRIMES = 10;
static const long long PRIMES[NUM_PRIMES] = {
    528244273, 582428591, 613372093, 633345899, 680899007, 759078491, 864349663, 955387067, 987829027, 990303059
};

int main(int argc, char* argv[])
{
    std::cin >> n >> m;

    setup(n, m);

    DFS(0);

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
    }

    myAssert(succCnt >= 1, "Does not terminate");

    myAssert(succCnt >= NUM_PRIMES / 2, "WARNING: Unexpected degeneracy");

    std::cout << std::fixed << std::setprecision(6) << gausReal() << std::endl;

    return 0;
}
