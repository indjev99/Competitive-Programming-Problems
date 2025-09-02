#include "navigation.h"

#include <cassert>

constexpr int DONE_COLOR = 1;
constexpr int DFS_COLOR = 2;
constexpr int DFS_FRONT_COLOR = 3;
constexpr int NUM_COLORS = 4;

std::pair<int, int> navigate(int currColor, std::vector<int> adjColors)
{
    std::vector<std::vector<int>> adjsByCol(NUM_COLORS);
    for (int i = 0; i < (int) adjColors.size(); i++)
    {
        int col = adjColors[i];
        adjsByCol[col].push_back(i);
    }

    assert(currColor != DONE_COLOR);

    assert(adjsByCol[DFS_FRONT_COLOR].size() <= 1);

    int numDfs = adjsByCol[DFS_COLOR].size() + adjsByCol[DFS_FRONT_COLOR].size();

    assert(numDfs <= 3);

    if (currColor == INIT_COLOR && numDfs == 1 && !adjsByCol[DFS_FRONT_COLOR].empty())
    {
        return {INIT_COLOR, adjsByCol[DFS_FRONT_COLOR].front()};
    }

    if (!adjsByCol[INIT_COLOR].empty())
    {
        if (currColor != DFS_FRONT_COLOR)
        {
            return {DFS_FRONT_COLOR, adjsByCol[INIT_COLOR].front()};
        }
        else
        {
            return {DFS_COLOR, adjsByCol[INIT_COLOR].front()};
        }
    }

    if (!adjsByCol[DFS_FRONT_COLOR].empty())
    {
        assert(!adjsByCol[DFS_COLOR].empty());

        return {DONE_COLOR, adjsByCol[DFS_FRONT_COLOR].front()};
    }

    if (!adjsByCol[DFS_COLOR].empty())
    {
        return {DONE_COLOR, adjsByCol[DFS_COLOR].front()};
    }

    return {TERMINATE_CODE, TERMINATE_CODE};
}
