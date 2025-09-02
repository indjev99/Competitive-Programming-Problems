#include "navigation.h"

#include <cassert>

constexpr int MOD = 7;

constexpr int DONE_COLOR = 1;
constexpr int DFS_COLORS = 2;
constexpr int DFS_FRONT_COLORS = DFS_COLORS + MOD;
constexpr int NUM_COLORS = 2 + 2 * MOD;

std::pair<int, int> navigate(int currColor, std::vector<int> adjColors)
{
    std::vector<std::vector<int>> adjsByCol(NUM_COLORS);
    for (int i = 0; i < (int) adjColors.size(); i++)
    {
        int col = adjColors[i];
        adjsByCol[col].push_back(i);
    }

    assert(currColor != DONE_COLOR);

    int adjFrontSeq = -1;
    for (int s = 0; s < MOD; s++)
    {
        if (!adjsByCol[DFS_FRONT_COLORS + s].empty())
        {
            adjFrontSeq = s;
            break;
        }
    }

    int seq = -1;
    if (currColor != INIT_COLOR)
    {
        seq = (currColor - DFS_COLORS) % MOD;
    }
    else if (adjsByCol[INIT_COLOR].size() == adjColors.size())
    {
        seq = 0;
    }
    else
    {
        assert(adjFrontSeq != -1);
        seq = (adjFrontSeq + 1) % MOD;
        return {DFS_FRONT_COLORS + seq, adjsByCol[DFS_FRONT_COLORS + adjFrontSeq].front()};
    }

    if (adjFrontSeq != -1)
    {
        assert(currColor >= DFS_FRONT_COLORS && currColor < DFS_FRONT_COLORS + MOD);
        return {DFS_COLORS + seq, adjsByCol[DFS_FRONT_COLORS + adjFrontSeq].front()};
    }

    if (!adjsByCol[INIT_COLOR].empty())
    {
        return {DFS_FRONT_COLORS + seq, adjsByCol[INIT_COLOR].front()};
    }

    int parSeq = (MOD + seq - 1) % MOD;

    if (!adjsByCol[DFS_COLORS + parSeq].empty())
    {
        return {DONE_COLOR, adjsByCol[DFS_COLORS + parSeq].front()};
    }

    return {TERMINATE_CODE, TERMINATE_CODE};
}
