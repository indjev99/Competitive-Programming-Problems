#include "mummy.h"

#include <algorithm>
#include <numeric>
#include <random>

std::mt19937 rng;

int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distr(lb, ub - 1);
    return distr(rng);
}

int n;
std::vector<int> perm;

std::vector<int> findPerm(int _n)
{
    rng.seed(0);

    n = _n;
    perm.resize(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), rng);

    std::vector<bool> known(n, false);
    std::vector<int> knownPos;

    int m = numMatches(perm);

    for (int i = 0; i < n && m < n; i++)
    {
        if (known[i]) continue;

        for (int j = i + 1; j < n && m < n && !known[i]; j++)
        {
            if (known[j]) continue;

            std::swap(perm[i], perm[j]);
            int newM = numMatches(perm);
            std::swap(perm[i], perm[j]);

            if (newM == m + 2)
            {
                std::swap(perm[i], perm[j]);

                known[i] = true;
                known[j] = true;
                knownPos.push_back(i);
                knownPos.push_back(j);

                m = newM;
            }
            else if (newM == m - 2)
            {
                known[i] = true;
                known[j] = true;
                knownPos.push_back(i);
                knownPos.push_back(j);
            }
            else if (newM == m + 1)
            {
                std::swap(perm[i], perm[j]);

                if (!knownPos.empty())
                {
                    int p = knownPos[0];

                    std::swap(perm[j], perm[p]);
                    int newM2 = numMatches(perm);
                    std::swap(perm[j], perm[p]);

                    if (newM2 + 1 == newM)
                    {
                        known[i] = true;
                        knownPos.push_back(i);

                        m = newM;
                    }
                    else
                    {
                        known[j] = true;
                        knownPos.push_back(j);

                        m = newM;
                    }
                }
                else
                {
                    while (true)
                    {
                        int k = randNum(0, n);
                        if (k == i || k == j) continue;

                        std::swap(perm[i], perm[k]);
                        int newMI = numMatches(perm);
                        std::swap(perm[i], perm[k]);

                        std::swap(perm[j], perm[k]);
                        int newMJ = numMatches(perm);
                        std::swap(perm[j], perm[k]);

                        if (newMI < newMJ)
                        {
                            known[i] = true;
                            knownPos.push_back(i);

                            m = newM;

                            break;
                        }
                        else if (newMJ < newMI)
                        {
                            known[j] = true;
                            knownPos.push_back(j);

                            m = newM;

                            break;
                        }
                    }
                }
            }
            else if (newM == m - 1)
            {
                if (!knownPos.empty())
                {
                    int p = knownPos[0];

                    std::swap(perm[j], perm[p]);
                    int newM2 = numMatches(perm);
                    std::swap(perm[j], perm[p]);

                    if (newM2 + 1 == m)
                    {
                        known[i] = true;
                        knownPos.push_back(i);
                    }
                    else
                    {
                        known[j] = true;
                        knownPos.push_back(j);
                    }
                }
                else
                {
                    while (true)
                    {
                        int k = randNum(0, n);
                        if (k == i || k == j) continue;

                        std::swap(perm[i], perm[k]);
                        int newMI = numMatches(perm);
                        std::swap(perm[i], perm[k]);

                        std::swap(perm[j], perm[k]);
                        int newMJ = numMatches(perm);
                        std::swap(perm[j], perm[k]);

                        if (newMI < newMJ)
                        {
                            known[i] = true;
                            knownPos.push_back(i);

                            break;
                        }
                        else if (newMJ < newMI)
                        {
                            known[j] = true;
                            knownPos.push_back(j);

                            break;
                        }
                    }
                }
            }
        }

        known[i] = true;
        knownPos.push_back(i);
    }

    return perm;
}
