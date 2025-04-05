#include "pointsort.h"
#include <algorithm>
#include <numeric>
#include <map>
#include <cmath>

using PointT = std::pair<int, std::vector<int>>;

std::vector<std::map<int, int>> mergeImpl(
    const std::vector<PointT>& left,
    const std::vector<PointT>& right,
    int k)
{
    if (left.empty()) return {};

    int n = left.size();
    int m = right.size();

    std::vector<std::map<int, int>> l2rLB(k);
    std::vector<std::map<int, int>> l2rUB(k);

    for (int d = 0; d < k; d++)
    {
        l2rLB[d].insert({0, -1});
        l2rUB[d].insert({n - 1, m});
    }

    auto getLB = [&l2rLB](int x, int d)
    {
        return *std::prev(l2rLB[d].upper_bound(x));
    };

    auto getUB = [&l2rUB](int x, int d)
    {
        return *l2rUB[d].lower_bound(x);
    };

    auto addLB = [&l2rLB, &getLB](int x, int y, int d)
    {
        if (getLB(x, d).second >= y) return;

        auto [it, _] = l2rLB[d].insert({x, y});
        it->second = y;

        it = std::next(it);
        while (it != l2rLB[d].end() && it->second <= y)
        {
            it = l2rLB[d].erase(it);
        }
    };

    auto addUB = [&l2rUB, &getUB](int x, int y, int d)
    {
        if (getUB(x, d).second <= y) return;

        auto [it, _] = l2rUB[d].insert({x, y});
        it->second = y;

        while (it != l2rUB[d].begin() && std::prev(it)->second >= y)
        {
            l2rUB[d].erase(std::prev(it));
        }
    };

    auto findBestSplit = [k, m, &left, &right, &getLB, &getUB](int leftI)
    {
        std::vector<std::pair<int, int>> lbs(k);
        std::vector<std::pair<int, int>> ubs(k);

        for (int d = 0; d < k; d++)
        {
            int x = left[leftI].second[d];
            lbs[d] = getLB(x, d);
            ubs[d] = getUB(x, d);
        }

        int bestI = -1;
        double bestScore = -1;
        for (int i = 0; i < m; i++)
        {
            double score = 0;
            for (int d = 0; d < k; d++)
            {
                int x = left[leftI].second[d];

                int y = right[i].second[d];

                if (y <= lbs[d].second || y >= ubs[d].second) continue;

                int rL = y - lbs[d].second;
                int rR = ubs[d].second - y;

                int lL = x - lbs[d].first + 1;
                int lR = ubs[d].first - x + 1;

                int l = rL * lR;
                int r = rR * lL;

                int t = l + r;

                double pL = (double) l / t;
                double pR = (double) r / t;

                score -= pL * log(pL) + pR * log(pR);
            }

            if (score > bestScore)
            {
                bestI = i;
                bestScore = score;
            }
        }

        return std::make_pair(bestI, bestScore);
    };

    while (true)
    {
        bool certain = true;
        for (int d = 0; d < k; d++)
        {
            for (int x = 0; x < n; x++)
            {
                if (getUB(x, d).second - getLB(x, d).second > 1)
                {
                    certain = false;
                }
            }
        }
        if (certain) break;

        int leftI = -1;
        int rightI = -1;
        double bestScore = -1;

        for (int i = 0; i < n; i++)
        {
            auto [currRightI, score] = findBestSplit(i);

            if (score > bestScore)
            {
                leftI = i;
                rightI = currRightI;
                bestScore = score;
            }
        }

        std::vector<bool> cmp = compare(left[leftI].first, right[rightI].first);

        for (int d = 0; d < k; d++)
        {
            if (cmp[d])
            {
                addUB(left[leftI].second[d], right[rightI].second[d], d);
            }
            else
            {
                addLB(left[leftI].second[d], right[rightI].second[d], d);
            }
        }
    }

    std::vector<std::map<int, int>> l2r(k);

    for (int d = 0; d < k; d++)
    {
        for (int x = 0; x < n; x++)
        {
            l2r[d].insert({x, getUB(x, d).second});
        }
    }

    return l2r;
}

std::vector<PointT> merge(
    std::vector<PointT>& left,
    std::vector<PointT>& right,
    int k)
{
    if (left.size() > right.size())
    {
        std::swap(left, right);
    }

    std::vector<std::map<int, int>> leftToRight = mergeImpl(left, right, k);

    std::vector<PointT> points(left.size() + right.size(), {-1, std::vector<int>(k)});

    for (int d = 0; d < k; d++)
    {
        std::vector<std::vector<std::pair<int, int>>> buckets(right.size() + 1);

        for (const PointT& point : right)
        {
            buckets[point.second[d]].push_back({left.size(), point.first});
        }

        for (const PointT& point : left)
        {

            buckets[leftToRight[d][point.second[d]]].push_back({point.second[d], point.first});
        }

        int val = 0;
        std::vector<std::pair<int, int>> pointVals;

        for (auto& bucket : buckets)
        {
            std::sort(bucket.begin(), bucket.end());

            for (auto [_, i] : bucket)
            {
                pointVals.push_back({i, val});
                val++;
            }
        }

        std::sort(pointVals.begin(), pointVals.end());

        for (int i = 0; i < (int) pointVals.size(); i++)
        {
            points[i].first = pointVals[i].first;
            points[i].second[d] = pointVals[i].second;
        }
    }

    return points;
}

std::vector<PointT> mergeSort(const std::vector<int>& is, int k)
{
    if (is.size() == 0) return {};
    if (is.size() == 1) return {{is[0], std::vector<int>(k, 0)}};

    std::vector<int> left(is.begin(), is.begin() + is.size() / 2);
    std::vector<int> right(is.begin() + is.size() / 2, is.end());

    std::vector<PointT> leftPoints = mergeSort(left, k);
    std::vector<PointT> rightPoints = mergeSort(right, k);

    std::vector<PointT> points = merge(leftPoints, rightPoints, k);

    return points;
}

std::vector<std::vector<int>> pointSort(int n, int k)
{
    std::vector<int> is(n);
    std::iota(is.begin(), is.end(), 0);

    std::vector<std::vector<int>> points(n);

    std::vector<PointT> pointsVals = mergeSort(is, k);

    for (int i = 0; i < n; i++)
    {
        points[pointsVals[i].first] = pointsVals[i].second;
    }

    return points;
}
