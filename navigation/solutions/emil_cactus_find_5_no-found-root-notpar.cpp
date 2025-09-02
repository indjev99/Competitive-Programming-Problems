#include "navigation.h"

#include <cassert>

constexpr int DONE_COLOR = 1;
constexpr int DFS_COLOR = 2;
constexpr int PARSEEK_COLOR = 3;
constexpr int DFS_PAR_COLOR = 4;
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
    assert(adjsByCol[PARSEEK_COLOR].size() <= 1);

    if (!adjsByCol[PARSEEK_COLOR].empty())
    {
        assert(currColor == DFS_COLOR);
        assert(adjsByCol[DFS_PAR_COLOR].empty());

        if (adjsByCol[DFS_COLOR].size() >= 2)
        {
            return {DFS_COLOR, adjsByCol[PARSEEK_COLOR].front()};
        }

        return {DFS_PAR_COLOR, adjsByCol[PARSEEK_COLOR].front()};
    }

    assert(adjsByCol[DFS_COLOR].size() + adjsByCol[DFS_PAR_COLOR].size() <= 2);

    if (currColor == PARSEEK_COLOR)
    {
        assert(adjsByCol[INIT_COLOR].empty());

        if (!adjsByCol[DFS_PAR_COLOR].empty())
        {
            return {DONE_COLOR, adjsByCol[DFS_PAR_COLOR].front()};
        }

        assert(adjsByCol[DFS_COLOR].size() == 2);

        return {DONE_COLOR, adjsByCol[DFS_COLOR].back()};
    }

    assert(adjsByCol[DFS_PAR_COLOR].empty());

    if (!adjsByCol[INIT_COLOR].empty())
    {
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

    return {TERMINATE_CODE, TERMINATE_CODE};
}
