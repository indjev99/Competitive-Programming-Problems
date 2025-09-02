#include "navigation.h"

#include <cassert>

constexpr int DONE_COLOR = 1;
constexpr int NUM_COLORS = 2;

std::pair<int, int> navigate(int currColor, std::vector<int> adjColors)
{
    std::vector<std::vector<int>> adjsByCol(NUM_COLORS);
    for (int i = 0; i < (int) adjColors.size(); i++)
    {
        int col = adjColors[i];
        adjsByCol[col].push_back(i);
    }

    assert(currColor != DONE_COLOR);

    if (adjColors.size() == 1)
    {
        assert(adjsByCol[INIT_COLOR].size() == 1);

        return {DONE_COLOR, adjsByCol[INIT_COLOR].front()};
    }

    if (!adjsByCol[INIT_COLOR].empty())
    {
        return {INIT_COLOR, adjsByCol[INIT_COLOR].front()};
    }

    return {TERMINATE_CODE, TERMINATE_CODE};
}
