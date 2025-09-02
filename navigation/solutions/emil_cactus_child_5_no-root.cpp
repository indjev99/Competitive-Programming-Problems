#include "navigation.h"

#include <cassert>

constexpr int DONE_COLOR = 1;
constexpr int DFS_COLORS = 2;
constexpr int DFS_MULTI_COLOR = 4;
constexpr int NUM_COLORS = 5;

std::pair<int, int> navigate(int currColor, std::vector<int> adjColors)
{
    std::vector<std::vector<int>> adjsByCol(NUM_COLORS);
    for (int i = 0; i < (int) adjColors.size(); i++)
    {
        int col = adjColors[i];
        adjsByCol[col].push_back(i);
    }

    assert(currColor != DONE_COLOR);

    int numDfs = adjsByCol[DFS_COLORS].size() + adjsByCol[DFS_COLORS + 1].size() + adjsByCol[DFS_MULTI_COLOR].size();

    assert(numDfs <= 2);

    auto getChildIdx = [&](int child)
    {
        int currIdx = 0;
        for (int i = 0; i < child; i++)
        {
            int col = adjColors[i];
            if (col >= DFS_COLORS)
            {
                currIdx++;
            }
        }

        assert(currIdx <= 1);

        return currIdx;
    };

    auto getChildFromIdx = [&](int idx)
    {
        assert(idx <= 1);

        int currIdx = -1;
        for (int i = 0; i < (int) adjColors.size(); i++)
        {
            int col = adjColors[i];
            if (col >= DFS_COLORS)
            {
                currIdx++;
                if (currIdx == idx)
                {
                    return i;
                }
            }
        }

        assert(false);
    };

    if (currColor >= DFS_COLORS && currColor != DFS_MULTI_COLOR && numDfs == 2)
    {
        return {currColor, getChildFromIdx(currColor - DFS_COLORS)};
    }

    if (!adjsByCol[INIT_COLOR].empty())
    {
        int child = adjsByCol[INIT_COLOR].front();
        if (numDfs < 2)
        {
            return {DFS_COLORS + getChildIdx(child), child};
        }
        else
        {
            return {DFS_MULTI_COLOR, child};
        }
    }

    if (!adjsByCol[DFS_COLORS].empty() || !adjsByCol[DFS_COLORS + 1].empty())
    {
        if (!adjsByCol[DFS_COLORS].empty())
        {
            return {DONE_COLOR, adjsByCol[DFS_COLORS].front()};
        }

        return {DONE_COLOR, adjsByCol[DFS_COLORS + 1].front()};
    }

    if (!adjsByCol[DFS_MULTI_COLOR].empty())
    {
        return {DONE_COLOR, adjsByCol[DFS_MULTI_COLOR].front()};
    }

    return {TERMINATE_CODE, TERMINATE_CODE};
}
