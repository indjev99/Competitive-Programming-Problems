#include "navigation.h"

#include <cassert>

constexpr int NUM_LAYERS = 3;
constexpr int MODE_CLEARED = 0;
constexpr int MODE_EXPANDED = 1;
constexpr int MODE_CLEARING = 2;
constexpr int MODE_EXPANDING = 3;
constexpr int MODE_EXPANDING_UPD = 4;
constexpr int NUM_MODES = 5;
constexpr int NUM_COLORS = NUM_MODES * NUM_LAYERS + 1;

std::pair<int, int> navigate(int currColor, std::vector<int> adjColors)
{
    std::vector<std::vector<int>> adjsByCol(NUM_COLORS);
    std::vector<std::vector<int>> adjsByMode(NUM_MODES);
    for (int i = 0; i < (int) adjColors.size(); i++)
    {
        int col = adjColors[i];
        adjsByCol[col].push_back(i);
        if (col > INIT_COLOR)
        {
            adjsByMode[(col - 1) / NUM_LAYERS].push_back(i);
        }
    }

    while (true)
    {
        int mode = -1;
        if (currColor > INIT_COLOR)
        {
            mode = (currColor - 1) / NUM_LAYERS;
        }
        else
        {
            mode = MODE_CLEARED;
        }

        assert(adjsByMode[MODE_EXPANDING].size() + adjsByMode[MODE_EXPANDING_UPD].size() + adjsByMode[MODE_CLEARING].size() <= 1);

        bool expanding = false;
        bool clearing = false;

        int par = -1;
        if (!adjsByMode[MODE_EXPANDING].empty())
        {
            par = adjsByMode[MODE_EXPANDING].front();
            expanding = true;
        }
        if (!adjsByMode[MODE_EXPANDING_UPD].empty())
        {
            par = adjsByMode[MODE_EXPANDING_UPD].front();
            expanding = true;
        }
        if (!adjsByMode[MODE_CLEARING].empty())
        {
            par = adjsByMode[MODE_CLEARING].front();
            clearing = true;
        }

        bool isRoot = par == -1;

        int layer = -1;
        if (currColor > INIT_COLOR)
        {
            layer = (currColor - 1) % NUM_LAYERS;
        }
        else if (isRoot)
        {
            layer = 0;
        }
        else
        {
            int parLayer = (adjColors[par] - 1) % NUM_LAYERS;
            layer = (parLayer + 1) % NUM_LAYERS;
        }

        assert(layer != -1);

        if (mode == MODE_EXPANDING && !adjsByCol[INIT_COLOR].empty())
        {
            currColor = 1 + layer + MODE_EXPANDING_UPD * NUM_LAYERS;
            continue;
        }
        if (mode == MODE_EXPANDING || mode == MODE_EXPANDING_UPD)
        {
            if (!adjsByCol[INIT_COLOR].empty())
            {
                return {currColor, adjsByCol[INIT_COLOR].front()};
            }

            int nextClearedCol = 1 + (layer + 1) % NUM_LAYERS + MODE_CLEARED * NUM_LAYERS;
            if (!adjsByCol[nextClearedCol].empty())
            {
                return {currColor, adjsByCol[nextClearedCol].front()};
            }

            if (isRoot && mode == MODE_EXPANDING)
            {
                return {TERMINATE_CODE, TERMINATE_CODE};
            }

            if (!isRoot && mode == MODE_EXPANDING_UPD && adjColors[par] != MODE_EXPANDING_UPD)
            {
                return {INIT_COLOR, par};
            }

            currColor = 1 + layer + MODE_EXPANDED * NUM_LAYERS;
            continue;
        }
        if (mode == MODE_CLEARING)
        {
            int nextExpandedCol = 1 + (layer + 1) % NUM_LAYERS + MODE_EXPANDED * NUM_LAYERS;
            if (!adjsByCol[nextExpandedCol].empty())
            {
                return {currColor, adjsByCol[nextExpandedCol].front()};
            }

            currColor = 1 + layer + MODE_CLEARED * NUM_LAYERS;
            continue;
        }
        if (expanding && mode == MODE_CLEARED)
        {
            if (currColor == INIT_COLOR)
            {
                currColor = 1 + layer + MODE_EXPANDED * NUM_LAYERS;
                continue;
            }

            currColor = 1 + layer + MODE_EXPANDING * NUM_LAYERS;
            continue;
        }
        if (clearing && mode == MODE_EXPANDED)
        {
            currColor = 1 + layer + MODE_CLEARING * NUM_LAYERS;
            continue;
        }
        if ((expanding && mode == MODE_EXPANDED) || (clearing && mode == MODE_CLEARED))
        {
            return {currColor, par};
        }
        if (isRoot && mode == MODE_CLEARED)
        {
            currColor = 1 + layer + MODE_EXPANDING * NUM_LAYERS;
            continue;
        }
        if (isRoot && mode == MODE_EXPANDED)
        {
            currColor = 1 + layer + MODE_CLEARING * NUM_LAYERS;
            continue;
        }

        assert(false);

        return {TERMINATE_CODE, TERMINATE_CODE};
    }
}
