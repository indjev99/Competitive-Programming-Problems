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
std::vector<int> perm;
std::vector<int> unkPoss;
std::vector<int> unkVals;
std::vector<std::pair<int, int>> known;
std::vector<std::pair<int, int>> swaps;

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

void setKnown(int pos)
{
    int val = perm[pos];

    int idx = -1;
    for (int i = 0; i < (int) unkPoss.size(); i++)
    {
        if (unkPoss[i] == pos)
        {
            idx = i;
        }
    }

    for (auto& [a, b] : swaps)
    {
        if (a > idx) a--;
        if (b > idx) b--;
    }

    unkPoss.erase(std::remove(unkPoss.begin(), unkPoss.end(), pos), unkPoss.end());
    unkVals.erase(std::remove(unkVals.begin(), unkVals.end(), val), unkVals.end());

    known.push_back({pos, val});
}

void genSwaps()
{
    std::vector<int> idxs(unkPoss.size());
    std::iota(idxs.begin(), idxs.end(), 0);
    std::shuffle(idxs.begin(), idxs.end(), rng);

    int numSwaps = idxs.size() / 2;
    if (numSwaps % 2 == 0)
    {
        numSwaps--;
    }

    swaps.clear();
    for (int i = 0, s = 0; s < numSwaps; i += 2, s++)
    {
        swaps.push_back({idxs[i], idxs[i + 1]});
    }
}

void applySwaps(int from = 0, int to = -1)
{
    if (to == -1) to = swaps.size();

    for (int k = from; k < to; k++)
    {
        auto [i, j] = swaps[k];
        std::swap(unkPoss[i], unkPoss[j]);
    }

    makePerm();
}

std::vector<int> findPerm(int _n)
{
    rng.seed(0);

    n = _n;
    perm.resize(n);
    unkPoss.resize(n);
    unkVals.resize(n);
    known.clear();
    std::iota(unkPoss.begin(), unkPoss.end(), 0);
    std::iota(unkVals.begin(), unkVals.end(), 0);

    std::shuffle(unkPoss.begin(), unkPoss.end(), rng);
    std::shuffle(unkVals.begin(), unkVals.end(), rng);
    makePerm();

    int m = numMatches(perm) - (int) known.size();

    while (true)
    {
        int prevM = -1;
        while (m < n - (int) known.size() && (prevM == -1 || (prevM > 0 && m > 0) || (prevM == 0 && m == 0)))
        {
            prevM = m;
            genSwaps();
            applySwaps();
            m = numMatches(perm) - (int) known.size();
        }

        if (m == n - (int) known.size())
        {
            return perm;
        }

        if (m > 0)
        {
            applySwaps();
            std::swap(m, prevM);
        }

        while (prevM > 0)
        {
            int l = 0;
            int r = swaps.size();
            int swapM = prevM;
            int toRem = swaps.size();
            while (r - l > 1)
            {
                int mid = (l + r) / 2;

                applySwaps(0, mid);
                int newM = numMatches(perm) - (int) known.size();
                applySwaps(0, mid);

                if (newM == 0) l = mid;
                else
                {
                    r = mid;
                    if (newM < swapM) toRem = r;
                    swapM = newM;
                }
            }

            assert(swapM == 1 || swapM == 2);

            applySwaps(l, l + 1);

            auto [i, j] = swaps[l];

            swaps.erase(swaps.begin(), swaps.begin() + toRem);
            prevM -= swapM;

            int pi = unkPoss[i];
            int pj = unkPoss[j];

            if (swapM == 2)
            {
                setKnown(pi);
                setKnown(pj);
            }
            else if (!known.empty())
            {
                std::swap(perm[pi], perm[known[0].first]);
                int newM = numMatches(perm) - (int) known.size() + 1;
                std::swap(perm[pi], perm[known[0].first]);

                if (newM == 0) setKnown(pi);
                else setKnown(pj);
            }
            else
            {
                while (true)
                {
                    int pk = randNum(0, n);
                    if (pk == pi || pk == pj) continue;

                    int toSwap, other;
                    if (randNum(0, 2) == 0)
                    {
                        toSwap = pi;
                        other = pj;
                    }
                    else
                    {
                        toSwap = pj;
                        other = pi;
                    }

                    std::swap(perm[toSwap], perm[pk]);
                    int newM = numMatches(perm) - (int) known.size();
                    std::swap(perm[toSwap], perm[pk]);

                    if (newM == 0)
                    {
                        setKnown(toSwap);
                        break;
                    }
                    else if (newM > 1)
                    {
                        setKnown(other);
                        break;
                    }
                }
            }
        }

        m = 0;
    }

    return perm;
}
