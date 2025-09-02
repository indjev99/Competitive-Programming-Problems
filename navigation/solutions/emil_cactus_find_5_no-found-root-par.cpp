#include "navigation.h"

#include <cassert>

constexpr int DONE_COLOR = 1;
constexpr int DFS_COLOR = 2;
constexpr int PARSEEK_OR_PAR_COLOR = 3;
constexpr int DFS_NOTPAR_COLOR = 4;
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
    assert(adjsByCol[PARSEEK_OR_PAR_COLOR].size() <= 2);

    if (currColor != PARSEEK_OR_PAR_COLOR && !adjsByCol[PARSEEK_OR_PAR_COLOR].empty())
    {
        assert(currColor == DFS_COLOR || currColor == DFS_NOTPAR_COLOR);

        if (currColor == DFS_NOTPAR_COLOR)
        {
            return {DFS_COLOR, adjsByCol[PARSEEK_OR_PAR_COLOR].front()};
        }

        assert(adjsByCol[PARSEEK_OR_PAR_COLOR].size() == 1);

        if (adjsByCol[DFS_COLOR].size() + adjsByCol[DFS_NOTPAR_COLOR].size() >= 2)
        {
            return {DFS_NOTPAR_COLOR, adjsByCol[PARSEEK_OR_PAR_COLOR].front()};
        }

        return {PARSEEK_OR_PAR_COLOR, adjsByCol[PARSEEK_OR_PAR_COLOR].front()};
    }

    assert(currColor != DFS_NOTPAR_COLOR);
    assert(adjsByCol[PARSEEK_OR_PAR_COLOR].size() <= 1);
    assert(adjsByCol[DFS_COLOR].size() + adjsByCol[PARSEEK_OR_PAR_COLOR].size() + adjsByCol[DFS_NOTPAR_COLOR].size() <= 2);

    if (currColor == PARSEEK_OR_PAR_COLOR && (!adjsByCol[PARSEEK_OR_PAR_COLOR].empty() || !adjsByCol[DFS_NOTPAR_COLOR].empty()))
    {
        if (!adjsByCol[INIT_COLOR].empty())
        {
            assert(!adjsByCol[PARSEEK_OR_PAR_COLOR].empty());
            assert(adjsByCol[DFS_NOTPAR_COLOR].empty());

            return {PARSEEK_OR_PAR_COLOR, adjsByCol[PARSEEK_OR_PAR_COLOR].front()};
        }

        assert(adjsByCol[INIT_COLOR].empty());

        if (adjsByCol[PARSEEK_OR_PAR_COLOR].empty())
        {
            assert(!adjsByCol[DFS_COLOR].empty());

            return {PARSEEK_OR_PAR_COLOR, adjsByCol[DFS_COLOR].front()};
        }

        assert(adjsByCol[PARSEEK_OR_PAR_COLOR].size() == 1);

        if (!adjsByCol[DFS_NOTPAR_COLOR].empty())
        {
            return {PARSEEK_OR_PAR_COLOR, adjsByCol[DFS_NOTPAR_COLOR].front()};
        }

        return {DONE_COLOR, adjsByCol[PARSEEK_OR_PAR_COLOR].front()};
    }

    assert(adjsByCol[PARSEEK_OR_PAR_COLOR].empty());
    assert(adjsByCol[DFS_NOTPAR_COLOR].empty());

    if (!adjsByCol[INIT_COLOR].empty())
    {
        return {DFS_COLOR, adjsByCol[INIT_COLOR].front()};
    }

    if (adjsByCol[DFS_COLOR].size() >= 2)
    {
        return {PARSEEK_OR_PAR_COLOR, adjsByCol[DFS_COLOR].front()};
    }

    if (adjsByCol[DFS_COLOR].size() == 1)
    {
        return {DONE_COLOR, adjsByCol[DFS_COLOR].front()};
    }

    return {TERMINATE_CODE, TERMINATE_CODE};
}
