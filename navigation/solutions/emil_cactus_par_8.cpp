#include "navigation.h"

#include <cassert>

constexpr int DONE_COLOR = 1;
constexpr int DFS_COLORS = 2;
constexpr int DFS_FRONT_COLORS = 4;
constexpr int DFS_NEW_COLORS = 6;
constexpr int NUM_COLORS = 8;

std::pair<int, int> navigate(int currColor, std::vector<int> adjColors)
{
    std::vector<std::vector<int>> adjsByCol(NUM_COLORS);
    for (int i = 0; i < (int) adjColors.size(); i++)
    {
        int col = adjColors[i];
        adjsByCol[col].push_back(i);
    }

    assert(currColor != DONE_COLOR);

    int par = -1;
    int idx = -1;

    auto getParIdx = [&]()
    {
        if (par == -1)
        {
            return 0;
        }

        int currIdx = 0;
        for (int i = 0; i < par; i++)
        {
            int col = adjColors[i];
            if (col >= DFS_COLORS && col < DFS_NEW_COLORS)
            {
                currIdx++;
            }
        }

        assert(currIdx <= 1);

        return currIdx;
    };

    auto getParFromIdx = [&]()
    {
        int currIdx = -1;
        for (int i = 0; i < (int) adjColors.size(); i++)
        {
            int col = adjColors[i];
            if (col >= DFS_COLORS && col < DFS_NEW_COLORS)
            {
                currIdx++;
                if (currIdx == idx)
                {
                    return i;
                }
            }
        }
        return -1;
    };

    if (currColor != INIT_COLOR)
    {
        idx = (currColor - DFS_COLORS) % 2;
    }

    if (currColor >= DFS_FRONT_COLORS && currColor < DFS_NEW_COLORS)
    {
        assert(adjsByCol[DFS_NEW_COLORS].size() + adjsByCol[DFS_NEW_COLORS + 1].size() == 1);

        int child = -1;
        if (!adjsByCol[DFS_NEW_COLORS].empty())
        {
            child = adjsByCol[DFS_NEW_COLORS].front();
        }
        else
        {
            child = adjsByCol[DFS_NEW_COLORS + 1].front();
        }

        return {DFS_COLORS + idx, child};
    }

    assert(adjsByCol[DFS_NEW_COLORS].empty() && adjsByCol[DFS_NEW_COLORS + 1].empty());
    assert(adjsByCol[DFS_COLORS].size() + adjsByCol[DFS_COLORS + 1].size() + adjsByCol[DFS_FRONT_COLORS].size() + adjsByCol[DFS_FRONT_COLORS + 1].size() <= 2);

    if (currColor == INIT_COLOR)
    {
        if (adjsByCol[INIT_COLOR].size() == adjColors.size())
        {
            return {DFS_FRONT_COLORS, 0};
        }

        assert(adjsByCol[DFS_FRONT_COLORS].size() + adjsByCol[DFS_FRONT_COLORS + 1].size() == 1);

        if (!adjsByCol[DFS_FRONT_COLORS].empty())
        {
            par = adjsByCol[DFS_FRONT_COLORS].front();
        }
        else
        {
            par = adjsByCol[DFS_FRONT_COLORS + 1].front();
        }

        idx = getParIdx();

        return {DFS_NEW_COLORS + idx, par};
    }

    assert(adjsByCol[DFS_FRONT_COLORS].empty() && adjsByCol[DFS_FRONT_COLORS + 1].empty());

    if (!adjsByCol[INIT_COLOR].empty())
    {
        return {DFS_FRONT_COLORS + idx, adjsByCol[INIT_COLOR].front()};
    }

    par = getParFromIdx();

    if (par != -1)
    {
        return {DONE_COLOR, par};
    }

    return {TERMINATE_CODE, TERMINATE_CODE};
}
