#include "navigation.h"

#include <cassert>

constexpr int DONE_COLOR = 1;
constexpr int DFS_COLOR = 2;
constexpr int DFS_ROOT_COLOR = 3;
constexpr int PARSEEK_COLOR = 4;
constexpr int NUM_COLORS = 6;

std::pair<int, int> navigate(int currColor, std::vector<int> adjColors)
{
    std::vector<std::vector<int>> adjsByCol(NUM_COLORS);
    for (int i = 0; i < (int) adjColors.size(); i++)
    {
        int col = adjColors[i];
        adjsByCol[col].push_back(i);
    }

    assert(currColor != DONE_COLOR);

    if (!adjsByCol[PARSEEK_COLOR].empty())
    {
        assert(currColor == DFS_COLOR);

        if (adjsByCol[DFS_COLOR].size() + adjsByCol[DFS_ROOT_COLOR].size() >= 2)
        {
            return {DFS_COLOR, adjsByCol[PARSEEK_COLOR].back()};
        }
    }

    assert(adjsByCol[DFS_COLOR].size() + adjsByCol[DFS_ROOT_COLOR].size() <= 2);
    assert(adjsByCol[DFS_ROOT_COLOR].size() <= 1);

    if (currColor == PARSEEK_COLOR)
    {
        assert(adjsByCol[INIT_COLOR].empty());
        assert(adjsByCol[DFS_ROOT_COLOR].empty());
        assert(!adjsByCol[DFS_COLOR].empty());
        assert(adjsByCol[DFS_COLOR].size() <= 2);

        return {DONE_COLOR, adjsByCol[DFS_COLOR].back()};
    }

    assert(currColor == INIT_COLOR || currColor == DFS_ROOT_COLOR || !adjsByCol[DFS_COLOR].empty() || !adjsByCol[DFS_ROOT_COLOR].empty());

    if (!adjsByCol[INIT_COLOR].empty())
    {
        if (adjsByCol[DFS_COLOR].empty() && adjsByCol[DFS_ROOT_COLOR].empty())
        {
            return {DFS_ROOT_COLOR, adjsByCol[INIT_COLOR].front()};
        }

        return {DFS_COLOR, adjsByCol[INIT_COLOR].front()};
    }

    if (adjsByCol[DFS_COLOR].size() >= 2)
    {
        return {PARSEEK_COLOR, adjsByCol[DFS_COLOR].front()};
    }

    if (adjsByCol[DFS_COLOR].size() == 1)
    {
        return {DONE_COLOR, adjsByCol[DFS_COLOR].front()};
    }

    if (!adjsByCol[DFS_ROOT_COLOR].empty())
    {
        return {DONE_COLOR, adjsByCol[DFS_ROOT_COLOR].front()};
    }

    assert(currColor == INIT_COLOR || currColor == DFS_ROOT_COLOR);

    return {TERMINATE_CODE, TERMINATE_CODE};
}
