#include "mummy.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <cassert>

std::mt19937 rng;

int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distr(lb, ub - 1);
    return distr(rng);
}

int n;

struct Opts
{
    int cnt;
    std::vector<bool> p;

    Opts():
        cnt(n),
        p(n, true)
    {}

    void eliminate(int a)
    {
        if (p[a]) cnt--;
        p[a] = false;
    }

    int get()
    {
        if (cnt > 1) return -1;

        for (int i = 0; i < n; i++)
        {
            if (p[i]) return i;
        }

        assert(false);
        return -1;
    }
};

std::vector<int> perm;
std::vector<int> unkPoss;
std::vector<int> unkVals;
std::vector<std::pair<int, int>> known;
std::vector<Opts> optsValByPos;
std::vector<Opts> optsPosByVal;
std::vector<bool> isPosKnown;
std::vector<bool> isValKnown;

void makePerm()
{
    perm.resize(n);

    for (auto [pos, val] : known)
    {
        perm[pos] = val;
    }

    for (int i = 0; i < n - (int) known.size(); i++)
    {
        perm[unkPoss[i]] = unkVals[i];
    }
}

void setKnownImpl(int pos, int val)
{
    isPosKnown[pos] = true;
    isValKnown[val] = true;

    unkPoss.erase(std::remove(unkPoss.begin(), unkPoss.end(), pos), unkPoss.end());
    unkVals.erase(std::remove(unkVals.begin(), unkVals.end(), val), unkVals.end());

    for (int i = 0; i < n; i++)
    {
        if (i != pos) optsValByPos[i].eliminate(val);
        if (i != val) optsPosByVal[i].eliminate(pos);
    }

    known.push_back({pos, val});
}

bool eliminate()
{
    for (int i = 0; i < n - (int) known.size(); i++)
    {
        int pos = unkPoss[i];
        int val = unkVals[i];

        optsValByPos[pos].eliminate(val);
        optsPosByVal[val].eliminate(pos);
    }

    bool found = false;
    bool currFound = true;

    while (currFound)
    {
        currFound = false;
        for (int i = 0; i < n; i++)
        {
            if (!isPosKnown[i])
            {
                int val = optsValByPos[i].get();
                if (val != -1)
                {
                    currFound = true;
                    setKnownImpl(i, val);
                }
            }

            if (!isValKnown[i])
            {
                int pos = optsPosByVal[i].get();
                if (pos != -1)
                {
                    currFound = true;
                    setKnownImpl(pos, i);
                }
            }
        }

        found = found || currFound;
    }

    return found;
}

std::vector<int> findPerm(int _n)
{
    rng.seed(0);

    n = _n;
    perm.resize(n);
    unkPoss.resize(n);
    unkVals.resize(n);
    known.clear();
    optsValByPos.assign(n, Opts());
    optsPosByVal.assign(n, Opts());
    isPosKnown.assign(n, false);
    isValKnown.assign(n, false);

    std::iota(unkPoss.begin(), unkPoss.end(), 0);
    std::iota(unkVals.begin(), unkVals.end(), 0);

    while ((int) known.size() < n)
    {
        std::shuffle(unkPoss.begin(), unkPoss.end(), rng);
        std::shuffle(unkVals.begin(), unkVals.end(), rng);
        makePerm();

        int m = numMatches(perm) - (int) known.size();

        if (m == n - (int) known.size())
        {
            return perm;
        }

        if (m == 0 && eliminate())
        {
            std::shuffle(unkPoss.begin(), unkPoss.end(), rng);
            std::shuffle(unkVals.begin(), unkVals.end(), rng);
            makePerm();
        }
    }

    makePerm();
    return perm;
}
