#include "navigation.h"

#include <cassert>

constexpr int NUM_LAYERS = 3;
constexpr int MODE_EXPANDING = 0;
constexpr int MODE_EXPANDING_UPD = 3;
constexpr int NUM_MODES = 3;
constexpr int NUM_REAL_MODES = 4;
constexpr int NUM_COLORS = NUM_REAL_MODES * NUM_LAYERS + 1;

std::pair<int, int> navigate(int currColor, std::vector<int> adjColors)
{
    std::vector<std::vector<int>> adjsByCol(NUM_COLORS);
    for (int i = 0; i < (int) adjColors.size(); i++)
    {
        int col = adjColors[i];
        adjsByCol[col].push_back(i);
    }

    if (currColor == INIT_COLOR && adjsByCol[INIT_COLOR].size() == adjColors.size())
    {
        return {1 + MODE_EXPANDING_UPD * NUM_LAYERS, 0};
    }

    int layer = -1;
    if (currColor > INIT_COLOR)
    {
        layer = (currColor - 1) % NUM_LAYERS;
    }
    else
    {
        for (int i = 0; i < NUM_LAYERS; i++)
        {
            bool hasPrev = false;
            bool hasOther = false;
            for (int j = 0; j < (int) adjColors.size(); j++)
            {
                int col = adjColors[j];
                if (col == INIT_COLOR) continue;
                int adjLayer = (col - 1) % NUM_LAYERS;
                if ((adjLayer + 1) % NUM_LAYERS == i) hasPrev = true;
                else if (adjLayer != i) hasOther = true;
            }

            if (hasPrev && !hasOther)
            {
                layer = i;
                break;
            }
        }
    }

    assert(layer != -1);

    int realMode = -1;
    if (currColor > INIT_COLOR)
    {
        realMode = (currColor - 1) / NUM_LAYERS;
    }
    else
    {
        realMode = MODE_EXPANDING_UPD;
    }

    int mode = realMode % NUM_MODES;

    int par = -1;
    int newMode = mode;
    for (int i = 0; i < (int) adjColors.size(); i++)
    {
        int col = adjColors[i];
        if (col == INIT_COLOR) continue;
        int adjLayer = (col - 1) % NUM_LAYERS;
        if ((adjLayer + 1) % NUM_LAYERS != layer) continue;
        int adjMode = (col - 1) / NUM_LAYERS % NUM_MODES;
        if (adjMode == (mode + 1) % NUM_MODES) newMode = adjMode;
        if (adjMode == newMode) par = i;
    }

    bool isRoot = par == -1;

    if (currColor == INIT_COLOR)
    {
        assert(!isRoot);

        return {1 + layer + newMode * NUM_LAYERS, par};
    }

    if (newMode == MODE_EXPANDING && !adjsByCol[INIT_COLOR].empty())
    {
        return {1 + layer + MODE_EXPANDING_UPD * NUM_LAYERS, adjsByCol[INIT_COLOR].front()};
    }

    for (int i = 0; i < (int) adjColors.size(); i++)
    {
        int col = adjColors[i];
        if (col == INIT_COLOR) continue;
        int adjLayer = (col - 1) % NUM_LAYERS;
        if (adjLayer != (layer + 1) % NUM_LAYERS) continue;
        int adjMode = (col - 1) / NUM_LAYERS % NUM_MODES;
        if (adjMode == newMode) continue;
        assert((adjMode + 1) % NUM_MODES == newMode);
        return {1 + layer + newMode * NUM_LAYERS, i};
    }

    int newRealMode = newMode;
    if (newMode == MODE_EXPANDING)
    {
        if (realMode == MODE_EXPANDING_UPD)
        {
            newRealMode = MODE_EXPANDING_UPD;
        }

        for (int i = 0; i < (int) adjColors.size() && newRealMode != MODE_EXPANDING_UPD; i++)
        {
            int col = adjColors[i];
            if (col == INIT_COLOR) continue;
            int adjLayer = (col - 1) % NUM_LAYERS;
            if (adjLayer != (layer + 1) % NUM_LAYERS) continue;
            int adjRealMode = (col - 1) / NUM_LAYERS;
            if (adjRealMode == MODE_EXPANDING_UPD)
            {
                newRealMode = MODE_EXPANDING_UPD;
            }
        }
    }

    if (!isRoot)
    {
        return {1 + layer + newRealMode * NUM_LAYERS, par};
    }

    if (newRealMode == MODE_EXPANDING)
    {
        return {TERMINATE_CODE, TERMINATE_CODE};
    }

    return {1 + layer + (newMode + 1) % NUM_MODES * NUM_LAYERS, 0};
}
