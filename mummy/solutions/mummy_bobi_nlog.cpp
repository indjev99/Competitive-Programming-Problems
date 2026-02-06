#include "mummy.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cassert>
#include <vector>

typedef long long llong;
const int MAXN = 512;

int n;
int cntMatches[MAXN];
int position[MAXN];
int prefix[MAXN];
bool found[MAXN];
int value[MAXN];

std::vector <int> findPerm(int N)
{
    n = N;
    std::fill(found, found + MAXN, false);
    std::fill(cntMatches, cntMatches + MAXN, false);
    std::fill(prefix, prefix + MAXN, false);
    std::fill(position, position + MAXN, false);
    std::fill(value, value + MAXN, false);

    std::vector <std::vector <int>> shifts;
    shifts.resize(n);
    shifts[0].resize(n);
    std::iota(shifts[0].begin(), shifts[0].end(), 0);

    for (int i = 1 ; i < n ; ++i)
    {
        shifts[i] = shifts[i - 1];
        shifts[i].insert(shifts[i].begin(), shifts[i].back());
        shifts[i].pop_back();
    }

    int cnt = 0;
    std::vector <int> zeroMatch;
    for (int i = 0 ; i < n ; ++i)
    {
        cntMatches[i] = numMatches(shifts[i]);
        if (cntMatches[i] == 0)
        {
            zeroMatch = shifts[i];
            break;
        }
    }

    bool foundSol = false;
    std::vector <int> cpy;
    std::vector <int> candidates;
    int valueOfZero = -1;
    int stayingAtZero = zeroMatch[0];

    for (int i = 1 ; i < n && candidates.size() < 2 ; ++i)
    {
        cpy = zeroMatch;
        std::swap(cpy[i], cpy[0]);
        int res = numMatches(cpy);

        if (res == 0)
        {
            continue;
        }

        if (res == 1)
        {
            candidates.push_back(i);
        } else
        {
            assert(res == 2);
            foundSol = true;
            position[zeroMatch[0]] = i;
            position[zeroMatch[i]] = 0;
            valueOfZero = zeroMatch[i];
            found[0] = true;
            value[0] = zeroMatch[i];
            value[i] = zeroMatch[0];
        }
    }

    if (!foundSol)
    {
        assert(candidates.size() == 2);
        cpy = zeroMatch;
        cpy[0] = zeroMatch[candidates[0]];
        cpy[candidates[0]] = zeroMatch[candidates[1]];
        cpy[candidates[1]] = zeroMatch[0];
    
        // std::cout << "here: " << shifts[0][candidates[0]] << ' ' << shifts[0][candidates[1]] << '\n';

        int res = numMatches(cpy);
        if (res == 0)
        {
            position[zeroMatch[0]] = candidates[0];
            position[zeroMatch[candidates[1]]] = 0;
            valueOfZero = zeroMatch[candidates[1]];
            found[0] = true;
            value[0] = valueOfZero;
            value[candidates[0]] = zeroMatch[0];
            found[candidates[0]] = true;
        } else
        {
            position[zeroMatch[0]] = candidates[1];
            position[zeroMatch[candidates[0]]] = 0;
            valueOfZero = zeroMatch[candidates[0]];
            value[zeroMatch[candidates[0]]] = 0;
            value[candidates[1]] = zeroMatch[0];
            found[0] = true;
            found[candidates[1]] = true;
            value[0] = valueOfZero;
        }
    }


    shifts.clear();
    shifts.resize(n);
    int cntMatched = 2;
    shifts[0] = zeroMatch;
    shifts[0].erase(shifts[0].begin());
    for (int i = 1 ; i < n - 1 ; ++i)
    {
        shifts[i] = shifts[i - 1];
        shifts[i].insert(shifts[i].begin(), shifts[i].back());
        shifts[i].pop_back();
    }
    
    for (int i = 0 ; i < n - 1 ; ++i)
    {
        shifts[i].insert(shifts[i].begin(), zeroMatch[0]);
    }
    
    
    std::fill(prefix, prefix + n + 1, 0);
    for (int i = 1 ; i < n - 1 ; ++i)
    {
        // for (int j = 0 ; j < n ; ++j) std::cout << shifts[i][j] << ' '; std::cout << '\n';
        std::vector <int> viablePositions;
        int cntRemaining = numMatches(shifts[i]);
        std::vector <int> foundInRun;
        for (int j = 0 ; j < n ; ++j)
        {
            if (!found[j])
            {
                viablePositions.push_back(j);
            } else
            {
                if (value[j] == shifts[i][j])
                {
                    cntRemaining--;
                    if (j != 0)
                    {
                        foundInRun.push_back(j);
                    }
                }
            }
        }
        

        int last = -1;
        while (cntRemaining > 0)
        {
            int l = last, r = viablePositions.size(), mid;
            while (l < r - 1)
            {
                mid = l + r >> 1;
                cpy = shifts[i];
                
                for (int i = 0 ; i < viablePositions[mid] ; ++i)
                {
                    cpy[i] = cpy[i + 1];
                }

                cpy[viablePositions[mid]] = stayingAtZero;
                int toBeRemoved = prefix[viablePositions[mid] - 1] + (valueOfZero == cpy[0]) + (position[stayingAtZero] == viablePositions[mid]); 

                for (int j = viablePositions[mid] + 1 ; j < n ; ++j)
                {
                    if (found[j]) toBeRemoved += (value[j] == shifts[i][j]);
                }

                int res = numMatches(cpy) - toBeRemoved;
                assert(res <= cntRemaining);
                if (res == cntRemaining) l = mid;
                else r = mid;
            }

            assert(r < viablePositions.size());
            position[shifts[i][viablePositions[r]]] = viablePositions[r];
            value[viablePositions[r]] = shifts[i][viablePositions[r]];
            foundInRun.push_back(viablePositions[r]);
            found[viablePositions[r]] = true;
            last = r;
            cntRemaining--;
        }

        std::fill(prefix, prefix + n + 1, 0);
        for (const int &pos : foundInRun)
        {
            prefix[pos]++;
        }

        for (int i = 1 ; i < n ; ++i)
        {
            prefix[i] += prefix[i - 1];
        }
    }

    std::vector <int> sol(n);
    for (int i = 0 ; i < n ; ++i)
    {
        sol[position[i]] = i;
    }

    return sol;
}
