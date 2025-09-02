#include "navigation.h"

#include <cassert>

constexpr int DONE_COLOR = 1;
constexpr int LEFT_COLOR = 2;
constexpr int NUM_COLORS = 3;

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
        if (!adjsByCol[DONE_COLOR].empty())
        {
            return {TERMINATE_CODE, TERMINATE_CODE};
        }

        assert(!adjsByCol[INIT_COLOR].empty() || !adjsByCol[LEFT_COLOR].empty());

        if (!adjsByCol[INIT_COLOR].empty())
        {
            return {DONE_COLOR, adjsByCol[INIT_COLOR].front()};
        }

        return {DONE_COLOR, adjsByCol[LEFT_COLOR].front()};
    }

    if (!adjsByCol[DONE_COLOR].empty())
    {
        assert(!adjsByCol[INIT_COLOR].empty() || !adjsByCol[LEFT_COLOR].empty());

        if (!adjsByCol[INIT_COLOR].empty())
        {
            return {DONE_COLOR, adjsByCol[INIT_COLOR].front()};
        }

        return {DONE_COLOR, adjsByCol[LEFT_COLOR].front()};
    }

    if (!adjsByCol[INIT_COLOR].empty())
    {
        return {LEFT_COLOR, adjsByCol[INIT_COLOR].front()};
    }

    return {TERMINATE_CODE, TERMINATE_CODE};
}
