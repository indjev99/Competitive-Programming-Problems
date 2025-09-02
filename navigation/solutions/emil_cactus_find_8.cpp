#include "navigation.h"

#include <cassert>

constexpr int DONE_COLOR = 1;
constexpr int DFS_COLOR = 2;
constexpr int DFS_ROOT_COLOR = 3;
constexpr int PARSEEK_COLOR = 4;
constexpr int PARFOUND_COLOR = 5;
constexpr int DFS_PAR_COLOR = 6;
constexpr int DFS_NOTPAR_COLOR = 7;
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
    assert(adjsByCol[PARSEEK_COLOR].size() + adjsByCol[PARFOUND_COLOR].size() <= 1);

    if (!adjsByCol[PARSEEK_COLOR].empty())
    {
        assert(currColor == DFS_COLOR);
        assert(adjsByCol[DFS_PAR_COLOR].empty());

        if (adjsByCol[DFS_COLOR].size() + adjsByCol[DFS_ROOT_COLOR].size() + adjsByCol[DFS_NOTPAR_COLOR].size() >= 2)
        {
            return {DFS_NOTPAR_COLOR, adjsByCol[PARSEEK_COLOR].front()};
        }

        return {DFS_PAR_COLOR, adjsByCol[PARSEEK_COLOR].front()};
    }

    if (!adjsByCol[PARFOUND_COLOR].empty())
    {
        assert(currColor == DFS_NOTPAR_COLOR);
        assert(adjsByCol[DFS_NOTPAR_COLOR].empty());

        return {DFS_COLOR, adjsByCol[PARFOUND_COLOR].front()};
    }

    assert(currColor != DFS_NOTPAR_COLOR);
    assert(adjsByCol[DFS_COLOR].size() + adjsByCol[DFS_ROOT_COLOR].size() + adjsByCol[DFS_PAR_COLOR].size() + adjsByCol[DFS_NOTPAR_COLOR].size() <= 2);
    assert(adjsByCol[DFS_ROOT_COLOR].size() <= 1);

    if (currColor == PARSEEK_COLOR)
    {
        assert(adjsByCol[INIT_COLOR].empty());
        assert(adjsByCol[DFS_ROOT_COLOR].empty());

        if (adjsByCol[DFS_PAR_COLOR].empty())
        {
            assert(!adjsByCol[DFS_COLOR].empty());

            return {PARSEEK_COLOR, adjsByCol[DFS_COLOR].front()};
        }

        assert(adjsByCol[DFS_PAR_COLOR].size() == 1);

        if (!adjsByCol[DFS_NOTPAR_COLOR].empty())
        {
            return {PARFOUND_COLOR, adjsByCol[DFS_NOTPAR_COLOR].front()};
        }

        return {DONE_COLOR, adjsByCol[DFS_PAR_COLOR].front()};
    }

    if (currColor == PARFOUND_COLOR)
    {
        assert(adjsByCol[INIT_COLOR].empty());
        assert(adjsByCol[DFS_ROOT_COLOR].empty());
        assert(adjsByCol[DFS_PAR_COLOR].size() == 1);

        if (!adjsByCol[DFS_NOTPAR_COLOR].empty())
        {
            return {PARFOUND_COLOR, adjsByCol[DFS_NOTPAR_COLOR].front()};
        }

        return {DONE_COLOR, adjsByCol[DFS_PAR_COLOR].front()};
    }

    assert(adjsByCol[DFS_PAR_COLOR].empty());
    assert(adjsByCol[DFS_NOTPAR_COLOR].empty());
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
