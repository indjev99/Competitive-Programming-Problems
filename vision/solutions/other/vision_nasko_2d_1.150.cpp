#include "vision.h"
#include <iostream>
#include <utility>
#include <cassert>
#include <optional>

std::vector<int> getVisionPattern1d()
{
    return {2, 2, 1, 1, 2, 1};
}

int getMove1d(std::vector<int> v)
{
    if (v.size() == 3) {
        if (v[0] == 2) {
            return 0;
        } else if (v[2] == 2) {
            return 2;
        }
        assert(false);
        return 0;
    }
    assert(v.size() == 5);

    int cnt = 0;
    for (const auto it : v) {
        cnt += it == 2;
    }

    if (v[1] == 2) {
        if (v[4] == 2) {
            return 0;
        } else {
            return 4;
        }
    } else if(v[3] == 2) {
        if (v[0] == 2) {
            return 4;
        } else {
            return 0;
        }
    } else if (v[0] == 2) {
        return 0;
    } else if (v[4] == 2) {
        return 4;
    }
    assert(false);

    return 0;
}

std::vector<std::vector<int>> getVisionPattern2d()
{
    return {
        { 1, 1, 1, 1 },
        { 2, 1, 1, 1 },
        { 1, 1, 2, 1 },
        { 1, 2, 1, 1 },
        { 1, 1, 1, 1 }
    };
}

// { 1, 1, 1, 1 },{ 1, 1, 1, 1 },
// { 2, 1, 1, 1 },{ 2, 1, 1, 1 },
// { 1, 1, 2, 1 },{ 1, 1, 2, 1 },
// { 1, 2, 1, 1 },{ 1, 2, 1, 1 },
// { 1, 1, 1, 1 },{ 1, 1, 1, 1 },
// { 1, 1, 1, 1 },{ 1, 1, 1, 1 },
// { 2, 1, 1, 1 },{ 2, 1, 1, 1 },
// { 1, 1, 2, 1 },{ 1, 1, 2, 1 },
// { 1, 2, 1, 1 },{ 1, 2, 1, 1 },
// { 1, 1, 1, 1 },{ 1, 1, 1, 1 },

std::pair<int, int> getMove2d(std::vector<std::vector<int>> v)
{
    const auto curr = v.size() / 2;

    if (curr == 1) {
        for (int i = 0; i < v.size(); i ++) {
            for (int j = 0; j < v.size(); j ++) {
                if (v[i][j] == 2) {
                    return {i, j};
                }
            }
        }

        return {0, 1};
        assert(false);
    }

    std::optional<std::pair<int, int>> close = std::nullopt;
    std::optional<std::pair<int, int>> far = std::nullopt;

    for (int i = 0; i < v.size(); i ++) {
        for (int j = 0; j < v.size(); j ++) {
            if (i * 2 + 1 == v.size() && j * 2 + 1 == v.size()) {
                continue;
            }

            if (v[i][j] != 2) {
                continue;
            }
            const auto dist = std::abs(2 - i) + std::abs(2 - j);

            if (dist == 2) {
                close = {i, j};
            } else {
                far = {i, j};
            }
        }
    }

    if (!close) {
        for (int i = 0; i < v.size(); i ++) {
            for (int j = 0; j < v.size(); j ++) {
                if (v[i][j] != 2) { continue; }

                for (int oi = 0; oi < v.size(); oi ++) {
                    for (int oj = 0; oj < v.size(); oj ++) {
                        if (v[oi][oj] != 2) { continue; }
                        if (std::abs(i - oi) == 1 && std::abs(j - oj) == 1) {
                            return {i, j};
                        }
                    }
                }
            }
        }
        assert(false);
    }

    std::pair<int, int> mx = {0, 0};
    for (int i = 0; i < v.size(); i += (int)v.size() - 1) {
        for (int j = 0; j < v.size(); j += (int)v.size() - 1) {
            if (
                (std::abs(mx.first - far.value().first) + std::abs(mx.second - far.value().second)) <
                (std::abs(i - far.value().first) + std::abs(j - far.value().second))
            ) {
                mx = {i, j};
            }
        }
    }

    return mx;
}
