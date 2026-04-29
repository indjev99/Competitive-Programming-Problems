#include "triangle.h"
#include <numeric>
#include <random>
#include <assert.h>
using namespace std;

mt19937 mt(1337);
int valToId[1011];

vector<pair<int, int>> optSolve(const vector<pair<int, int>>& known, const vector<int>& ids)
{
    for (auto [id, val] : known)
    {
        assert(val >= 1 && val <= known.size());
        valToId[val] = id;
    }

    assert(!ids.empty());
    if (ids.size() == 1)
    {
        return {{ids[0], known.size() + ids.size()}};
    }

    vector<pair<int, int>> newKnown = known;

    int L = known.size() + 1;
    int R = known.size() + ids.size();
    int midGoal = (L + R + 1) / 2;

    int idA = valToId[known.size()];
    int idB = valToId[min(known.size(), midGoal - known.size())];
    int expectedLeftSize = (known.size() + min(known.size(), midGoal - known.size()) - 1) - L + 1;

    vector<int> leftIds;
    vector<int> rightIds;
    for (int i = 0; i < ids.size(); i++)
    {
        if (i == (int)ids.size() - 1)
        {
            if (leftIds.size() == expectedLeftSize)
                rightIds.push_back(ids[i]);
            else
                leftIds.push_back(ids[i]);
        }
        else
        {
            if (query(idA, idB, ids[i]))
                leftIds.push_back(ids[i]);
            else
                rightIds.push_back(ids[i]);
        }
    }

    assert(!leftIds.empty() && !rightIds.empty());

    auto leftResult = optSolve(known, leftIds);
    vector<pair<int, int>> result;
    for (auto& item : leftResult)
    {
        newKnown.push_back(item);
        result.push_back(item);
    }

    auto rightResult = optSolve(newKnown, rightIds);
    for (auto& item : rightResult)
    {
        result.push_back(item);
    }

    return result;
}

vector<pair<int, int>> fullSolve(const vector<int>& ids, const vector<int>& lastRightIds)
{
    assert(!ids.empty());
    if (ids.size() == 1)
    {
        return {{ids[0], 1}};
    }
    else if (ids.size() == 2)
    {
        if (query(ids[0], ids[0], ids[1]))
            return {{ids[0], 2}, {ids[1], 1}};
        else
            return {{ids[0], 1}, {ids[1], 2}};
    }
    else if (ids.size() == 3)
    {
        vector<int> res = ids;
        if (query(res[0], res[0], res[1]))
            swap(res[0], res[1]);
        if (query(res[0], res[0], res[2]))
            swap(res[0], res[2]);

        for (auto id : lastRightIds)
        {
            if (query(res[1], res[1], id))
            {
                swap(res[1], res[2]);
                break;
            }
        }

        return { {res[0], 1}, {res[1], 2}, {res[2], 3} };
    }

    int badStuff = 0;
    const int THRESHOLD = 20;
    const int EXTRA_OPT_BUNCH = 10;
    while(true)
    {
        int pick = mt() % ids.size();
        if (ids.size() >= THRESHOLD)
        {
            for (int i = 0; i < EXTRA_OPT_BUNCH; i++)
            {
                int newPick = mt() % ids.size();

                if (!query(ids[pick], ids[pick], ids[newPick]))
                {
                    pick = newPick;
                }
            }
        }

        vector<int> smallPart, bigPart;
        for (int i = 0; i < ids.size(); i++)
        {
            if (i == pick)
                bigPart.push_back(ids[i]);
            else
            {
                badStuff++;
                if (query(ids[pick], ids[i], ids[i]))
                    bigPart.push_back(ids[i]);
                else
                    smallPart.push_back(ids[i]);
            }
        }

        if (smallPart.size() <= 1)
            continue;

        vector<pair<int, int>> solveLeft = fullSolve(smallPart, bigPart);
        vector<pair<int, int>> solveRight = optSolve(solveLeft, bigPart);

        for (auto& item : solveRight)
        {
            solveLeft.push_back(item);
        }

        return solveLeft;
    }
}

vector<int> solve(int n)
{
    vector<int> ids(n);
    iota(ids.begin(), ids.end(), 0);

    vector<pair<int, int>> result = fullSolve(ids, {});
    vector<int> finalResult(n);

    for (auto [idx, val] : result)
    {
        finalResult[idx] = val;
    }

    return finalResult;
}
