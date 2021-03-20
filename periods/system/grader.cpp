#include <iostream>
#include "periods.h"

static int n;
static std::vector<int> array;
static std::vector<int> retPeriods;
static std::vector<int> realPeriods;

static const long long BASE = 792753497ll;
static const long long MOD = 1185914819ll;
static long long hashWithInput(long long hsh)
{
    for (long long elem : array)
    {
        hsh = (hsh * BASE + elem) % MOD;
    }
    return hsh;
}

static void setResult(int res)
{
    std::cout << res << " " << hashWithInput(res) << std::endl;
    exit(0);
}

static void input()
{
    std::cin >> n;
    array.resize(n);
    for (int& elem : array)
    {
        std::cin >> elem;
    }
}

static void calcRealPeriods()
{
    realPeriods.resize(n);
    if (n == 0) return;

    std::vector<int> failLink(n);
    failLink[0] = -1;
    for (int i = 1; i < n; ++i)
    {
        int cand = failLink[i - 1];
        while (array[i] != array[cand + 1])
        {
            if (cand == -1)
            {
                cand = -2;
                break;
            }
            cand = failLink[cand];
        }
        failLink[i] = cand + 1;
    }

    for (int i = 0; i < n; ++i)
    {
        int period = i - failLink[i];
        if ((i + 1) % period == 0) realPeriods[i] = period;
        else realPeriods[i] = i + 1;
    }
}

static int numQs;
bool hasPeriod(int len, int period)
{
    ++numQs;

    if (len <= 0 || len > n || period <= 0)
    {
        setResult(-2);
    }

    if (len % period != 0) return false;

    for (int i = period; i < len; ++i)
    {
        if (array[i] != array[i - period]) return false;
    }

    return true;

    return len % period == 0 && period % realPeriods[len - 1] == 0;
}

int main()
{
    std::ios::sync_with_stdio(false);

    input();
    calcRealPeriods();

    numQs = 0;
    retPeriods = findPeriods(n);

    if ((int) retPeriods.size() != n) setResult(-1);

    for (int i = 0; i < n; ++i)
    {
        if (retPeriods[i] != realPeriods[i]) setResult(-1);
    }

    setResult(numQs);

    return 0;
}
